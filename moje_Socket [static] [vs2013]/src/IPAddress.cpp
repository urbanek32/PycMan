#include "IPAddress.h"
#include "SocketImpl.h"

#include <string>


namespace moje
{
	const IPAddress IPAddress::None;
	const IPAddress IPAddress::LocalHost(127, 0, 0, 1);
	const IPAddress IPAddress::Broadcast(255, 255, 255, 255);

	UInt32 resolve(const std::string &address)
	{
		if (address == "255.255.255.255")
		{
			return INADDR_BROADCAST;
		}
		else
		{
			// próbuj konwertowaæ adress jako reprezentacje ("xxx.xxx.xxx.xxx")
			UInt32 ip = inet_addr(address.c_str());
			if (ip != INADDR_NONE)
				return ip;

			// próba konwertowania jako hostname np "localhost"
			addrinfo hints;
			std::memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			addrinfo *result = NULL;
			if (getaddrinfo(address.c_str(), NULL, &hints, &result) == 0)
			{
				if (result)
				{
					ip = reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr.s_addr;
					freeaddrinfo(result);
					return ip;
				}
			}

			// jakiœ syf i nic z tego nie wyjdzie
			return 0;
		}
	}


	IPAddress::IPAddress() : m_address(0)
	{

	}

	IPAddress::IPAddress(const std::string &address) : m_address(resolve(address))
	{

	}

	IPAddress::IPAddress(const char *address) : m_address(resolve(address))
	{

	}

	IPAddress::IPAddress(UInt8 byte0, UInt8 byte1, UInt8 byte2, UInt8 byte3) :
		m_address( htonl((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3 ))
	{

	}

	IPAddress::IPAddress(UInt32 address) : m_address(htonl(address))
	{

	}

	std::string IPAddress::toString() const
	{
		IN_ADDR address;
		address.s_addr = m_address;

		return inet_ntoa(address);
	}

	UInt32 IPAddress::toInteger() const
	{
		return ntohl(m_address);
	}

	bool IPAddress::operator ==(const IPAddress& left)
	{
		return left.toInteger() == this->toInteger();
	}

	bool IPAddress::operator !=(const IPAddress& left)
	{
		return !(left.toInteger() == this->toInteger());
	}

	bool IPAddress::operator <(const IPAddress& left)
	{
		return left.toInteger() < this->toInteger();
	}

	bool IPAddress::operator >(const IPAddress& left)
	{
		return !(left.toInteger() < this->toInteger());
	}
}