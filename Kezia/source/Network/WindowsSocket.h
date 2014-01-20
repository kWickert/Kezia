#pragma once

#include "Packet.h"
#include "NetworkAddress.h"

#include "Core/CircularBuffer.h"
#include "Core/Time.h"
#include "Core/Types.h"

#include <map>
#include <vector>

namespace Kezia
{
	class WindowsSocket
	{
	public:
		WindowsSocket();
		~WindowsSocket();

		bool BindToPort(const U32 port);

		void Update(const F64 deltaTime);
		void SendPacketTo(Packet & packet, const NetworkAddress & destination);
		bool ReceivePacket(Packet & outPacket, NetworkAddress & outSender);	

		const F64 GetPingToPeer(const NetworkAddress & peerAddress) const;
		const F64 GetClockOffset(const NetworkAddress & peerAddress) const;

		void EnableTrafficLogging();
		void DisableTrafficLogging();
	private:
		struct GuaranteedMesssageInfo
		{
			GuaranteedMesssageInfo(Packet packet)
				:	packet(packet),
					resendTime(GetLiveTimeSeconds() + 0.5)
			{}

			Packet packet;
			F64 resendTime;
		};

		struct PeerInfo
		{
			PeerInfo()
				:	lastPingTime(GetLiveTimeSeconds() + 0.5)
			{}

			const F64 ComputePingAverage() const
			{
				F64 totalNumerator = 0.0;
				F64 totalDenominator = 0.0;

				U32 i = 0;
				auto it = pingHistory.Begin();

				do 
				{
					if(pingHistory.Size() == 0)
						break;

					if(*it != 0)
					{
						F64 contributionFactor = pow(0.9, (int)i++);

						totalNumerator += *it * contributionFactor;
						totalDenominator += contributionFactor;
					}

					++it;
				} while (it != pingHistory.Begin());

				return totalDenominator == 0 ? 0 : totalNumerator / totalDenominator;
			}

			const F64 GetClockOffset() const
			{
				F64 total = 0.0;
				U32 validRecordCount = pingHistory.Size();

				auto it = clockOffsetHistory.Begin();

				do 
				{
					if(clockOffsetHistory.Size() == 0)
						break;

					if(*it == 0)
					{
						validRecordCount--;
					}

					total += *it;

					++it;
				} while (it != clockOffsetHistory.Begin());

				return validRecordCount == 0 ? 0 : total / validRecordCount;
			}

			F64 lastPingTime;
			CircularBuffer<F64, 10> clockOffsetHistory;
			CircularBuffer<F64, 10> pingHistory;
		};

		static bool InitializeSockets();

		static F64 k_PingPeriod;

		bool CheckForData(const U32 millisecondsToWait = 0) const;
		void ResendPacket(Packet & packet, const NetworkAddress & destination);

		static std::map<NetworkAddress, U32> k_PacketCounter;

		typedef std::map<U32, GuaranteedMesssageInfo> GuaranteedMessageMap;
		std::map<NetworkAddress, GuaranteedMessageMap> m_GuaranteedPackets;
		std::map<NetworkAddress, PeerInfo> m_Peers;

		SOCKET m_Socket;
		S32 m_PortBinding;
		bool m_IsLogging;
	};
}