#include "WindowsSocket.h"

#include "Core/Logger.h"
#include "Core/Time.h"

#include "Event/Event.h"

#include <iostream>

namespace Kezia
{
	F64 WindowsSocket::k_PingPeriod = 0.1;
	std::map<NetworkAddress, U32> WindowsSocket::k_PacketCounter;

	WindowsSocket::WindowsSocket()
		:	m_Socket(INVALID_SOCKET),
			m_PortBinding(-1),
			m_IsLogging(false)
	{
		static bool libInitSuccess = InitializeSockets();

		m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(m_Socket == INVALID_SOCKET)
		{
			LOG("could not create socket");
		}		

		//non-blocking mode on != 0
		//non-blocking modd off = 0
		u_long blockingMode = 1;

		ioctlsocket(m_Socket, FIONBIO, &blockingMode);
	}

	WindowsSocket::~WindowsSocket()
	{
		S32 errorResult = closesocket(m_Socket);

		if(errorResult == SOCKET_ERROR)
		{
			LOG("could not close socket");
		}
	}

	bool WindowsSocket::BindToPort(const U32 port)
	{
		S32 errorResult;

		sockaddr_in recvAddr;
		recvAddr.sin_family = AF_INET;
		recvAddr.sin_port = htons(port);
		recvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		errorResult = bind(m_Socket, (SOCKADDR *) & recvAddr, sizeof(recvAddr));

		bool bindSuccess = errorResult == 0;

		if(!bindSuccess)
		{
			LOG("could not bind socket to port " << port);
		}
		else
		{
			m_PortBinding = port;
		}

		return bindSuccess;
	}

	void WindowsSocket::Update(const F64 deltaTime)
	{
		F64 currentTime = GetLiveTimeSeconds();

		for(auto outer = m_GuaranteedPackets.begin(); outer != m_GuaranteedPackets.end(); ++outer)
		{
			GuaranteedMessageMap & unackedPackets = outer->second;

			for(auto it = unackedPackets.begin(); it != unackedPackets.end(); ++it)
			{
				if(currentTime > it->second.resendTime)
				{
					Packet & packet = it->second.packet;
					LOG(packet.GetPacketID() << " has not been acknowledged, resending");

					NamedProperties args;
					args.SetProperty("packet", &packet);

					FireEvent("packetResend", args);

					ResendPacket(packet, outer->first);
					it->second.resendTime = currentTime + 0.5;
				}
			}
		}

		for(auto peer = m_Peers.begin(); peer != m_Peers.end(); ++peer)
		{
			const NetworkAddress & peerAddress = peer->first;
			PeerInfo & peerInfo = peer->second;

			F64 currentTime = GetLiveTimeSeconds();
			if(currentTime - peerInfo.lastPingTime > k_PingPeriod)
			{
				SendPacketTo(Packet("Ping"), peerAddress);
				peerInfo.lastPingTime = currentTime;
			}
		}
	}

	void WindowsSocket::SendPacketTo(Packet & packet, const NetworkAddress & destination)
	{
		U32 packetID = k_PacketCounter[destination]++;

		packet.SetFieldValue("packetID", packetID);

		ResendPacket(packet, destination);

		if(packet.IsGuaranteed())
		{
			m_GuaranteedPackets[destination].insert(std::make_pair(packetID, packet));
		}

		m_Peers[destination];
	}

	bool WindowsSocket::ReceivePacket(Packet & outPacket, NetworkAddress & outSender)
	{
		static char inBuffer[Packet::k_PacketSize];
		static int lastError;

		bool validData = CheckForData();

		if(validData)
		{
			ZeroMemory(inBuffer, Packet::k_PacketSize);

			S32 errorResult;

			sockaddr_in senderAddr;
			S32 senderAddrSize = sizeof(senderAddr);

			errorResult = recvfrom(m_Socket, inBuffer, Packet::k_PacketSize, 0, (sockaddr *)&senderAddr, &senderAddrSize);
			if(errorResult == SOCKET_ERROR)
			{
				lastError = WSAGetLastError();
				validData = false;

				if(lastError != WSAEWOULDBLOCK && lastError != WSAECONNRESET)
				{
					LOG("error receiving messages: error # " << lastError);
				}
			}
			else
			{
				outPacket.Deserialize(inBuffer);
				outPacket.m_SenderAddress = NetworkAddress(senderAddr);

				outSender = NetworkAddress(senderAddr);

				if(m_IsLogging)
				{
					U32 packetID;
					outPacket.GetFieldValue("packetID", packetID);

					LOG("packet " << packetID << '(' << outPacket.GetTemplateName() << ')' << " received from " << outSender.GetFullIpAddress());
				}

				if(outPacket.IsGuaranteed())
				{
					Packet ack("ACK");

					ack.SetFieldValue("sourcePacketID", outPacket.GetPacketID());

					SendPacketTo(ack, outSender);
				}

				std::string templateName = outPacket.GetTemplateName();

				if(templateName == "ACK")
				{
					U32 sourcePacketID;

					outPacket.GetFieldValue("sourcePacketID", sourcePacketID);

					m_GuaranteedPackets[outSender].erase(sourcePacketID);
				}

				if(templateName == "Ping")
				{
					Packet pingReply("PingReply");

					TimeStamp sendTime;
					outPacket.GetFieldValue("timeStamp", sendTime);

					pingReply.SetFieldValue("sourcePacketSendTime", sendTime);

					SendPacketTo(pingReply, outSender);
				}

				if(templateName == "PingReply")
				{
					TimeStamp sendTime;
					outPacket.GetFieldValue("sourcePacketSendTime", sendTime);

					TimeStamp currentTime;

					F64 ping(currentTime.DifferenceSeconds(sendTime) * 0.5);

					m_Peers[outSender].pingHistory.Push(ping);

					outPacket.GetFieldValue("timeStamp", sendTime);

					F64 clockOffset(currentTime.DifferenceSeconds(sendTime) - ping);

					m_Peers[outSender].clockOffsetHistory.Push(clockOffset);
				}
			}

			m_Peers[outSender];
		}

		return validData;
	}

	const F64 WindowsSocket::GetPingToPeer(const NetworkAddress & peerAddress) const
	{
		F64 ping = 0.0;
		auto findResult = m_Peers.find(peerAddress);

		if(findResult != m_Peers.end())
		{
			ping = findResult->second.ComputePingAverage();
		}

		return ping;
	}

	const F64 WindowsSocket::GetClockOffset(const NetworkAddress & peerAddress) const
	{
		F64 offset = 0.0;
		auto findResult = m_Peers.find(peerAddress);

		if(findResult != m_Peers.end())
		{
			offset = findResult->second.GetClockOffset();
		}

		return offset;
	}

	void WindowsSocket::EnableTrafficLogging()
	{
		m_IsLogging = true;
	}

	void WindowsSocket::DisableTrafficLogging()
	{
		m_IsLogging = false;
	}

	bool WindowsSocket::InitializeSockets()
	{
		WSADATA wsaData;

		S32 result;

		result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if(result != 0)
		{
			LOG("Could not initialize sockets");
		}

		return result == 0;
	}

	bool WindowsSocket::CheckForData(const U32 millisecondsToWait) const
	{
		fd_set set;
		FD_ZERO(&set);
		FD_SET(m_Socket, &set);

		timeval waitDuration;
		waitDuration.tv_sec = 0;
		waitDuration.tv_usec = millisecondsToWait * 1000;

		return select(0, &set, nullptr, nullptr, &waitDuration) > 0;
	}

	void WindowsSocket::ResendPacket(Packet & packet, const NetworkAddress & destination)
	{
		static char buffer[Packet::k_PacketSize];

		packet.SetFieldValue("timeStamp", TimeStamp());

		U32 packetSize = packet.Serialize(buffer);

		const sockaddr_in & destinationAddr = destination.GetSocketAddress();

		S32 errorResult = sendto(m_Socket, buffer, packetSize, 0, (sockaddr *)&destinationAddr, sizeof(destinationAddr));

		if(errorResult == SOCKET_ERROR)
		{
			LOG("error sending message");
		}
		else if(m_IsLogging)
		{
			LOG("packet " << packet.GetPacketID() << '(' << packet.GetTemplateName() << ", " << packetSize << " bytes)" << " sent to " << destination.GetFullIpAddress());
		}
	}


}