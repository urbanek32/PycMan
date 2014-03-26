#ifndef SOCKET_IMPL_H
#define SOCKET_IMPL_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "Typedefs.h"
#include "UDP_Socket.h"

namespace moje
{
	namespace priv
	{

		class SocketImpl
		{
		public:
			typedef int AddrLength;


			static sockaddr_in createAddress(UInt32 address, unsigned short port);

			static SocketHandle invalidSocket();

			static void close(SocketHandle sock);

			static void setBlocking(SocketHandle sock, bool block);

			static UDP_Socket::Status getErrorStatus(bool printWSAErrorCodes = false);
		};
	}
}
#endif