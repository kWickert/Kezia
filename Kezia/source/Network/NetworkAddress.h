#pragma once

#include "Core/Types.h"
#include "Core/WindowsInclude.h"
#include "Core/SuperFastHash.h"

#include <string>
#include <sstream>

namespace Kezia
{
	class NetworkAddress
	{
	public:
		NetworkAddress(const sockaddr_in & socketAddress)
			:	m_SocketAddress(socketAddress),
				m_IpAddress(inet_ntoa(socketAddress.sin_addr)),
				m_PortNumber(ntohs(socketAddress.sin_port))
		{
			m_AddressHash = SuperFastHash(GetFullIpAddress());
		}

		NetworkAddress(const std::string & fullIpAddress = "127.0.0.1:0")
		{
			U32 delimeter = fullIpAddress.find_last_of(':');
			m_IpAddress = fullIpAddress.substr(0, delimeter);
			m_PortNumber = static_cast<U16>(atoi(fullIpAddress.substr(delimeter + 1).c_str()));

			m_SocketAddress.sin_family = AF_INET;
			m_SocketAddress.sin_port = htons(m_PortNumber);
			m_SocketAddress.sin_addr.S_un.S_addr = inet_addr(m_IpAddress.c_str());

			m_AddressHash = SuperFastHash(GetFullIpAddress());
		}

		NetworkAddress(const std::string & ipAddress, const U16 portNumber)
			:	m_IpAddress(ipAddress),
				m_PortNumber(portNumber)
		{
			m_SocketAddress.sin_family = AF_INET;
			m_SocketAddress.sin_port = htons(m_PortNumber);
			m_SocketAddress.sin_addr.S_un.S_addr = inet_addr(m_IpAddress.c_str());

			m_AddressHash = SuperFastHash(GetFullIpAddress());
		}

		~NetworkAddress()
		{}

		bool operator<(const NetworkAddress & rhs) const
		{
			return m_AddressHash < rhs.m_AddressHash;
		}

		const std::string & GetIpAddress() const
		{
			return m_IpAddress;
		}

		const std::string GetFullIpAddress() const
		{
			std::stringstream ss;

			ss << m_IpAddress << ':' << m_PortNumber;
			return ss.str();
		}

		const U16 & GetPortNumber() const
		{
			return m_PortNumber;
		}

		const sockaddr_in & GetSocketAddress() const
		{
			return m_SocketAddress;
		}
	private:
		std::string m_IpAddress;
		U16 m_PortNumber;
		U32 m_AddressHash;

		sockaddr_in m_SocketAddress;
	};
}