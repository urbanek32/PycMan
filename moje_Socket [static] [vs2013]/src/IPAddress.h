#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <string>
#include "Typedefs.h"

namespace moje
{
	class IPAddress
	{
	public:

		// pusty konstruktor tworzy pusty(z³y) adres
		IPAddress();

		//tworzy adres ze stringa np. "192.168.2.1" albo "localhost"
		IPAddress(const std::string &address);
		IPAddress(const char *address);

		IPAddress(UInt8 byte0, UInt8 byte1, UInt8 byte2, UInt8 byte3);
		IPAddress(UInt32 address);


		// zwraca adres w formie stringu
		std::string toString() const;

		// zwraca adres w formie 32bitowego inta
		UInt32 toInteger() const;

		static const IPAddress None; // reprezentacja pustego adresu
		static const IPAddress LocalHost; // reprezentacja samego siebie
		static const IPAddress Broadcast; // do wysy³ania udp do wszystkich w sieci

	private:
		UInt32 m_address; // adres trzymany jako 32bitowy int
	};
}
#endif