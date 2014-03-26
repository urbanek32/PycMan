#include "SocketImpl.h"
#include <cstring>

namespace moje
{
	namespace priv
	{

		sockaddr_in SocketImpl::createAddress(UInt32 address, unsigned short port)
		{
			sockaddr_in addr;
			std::memset(&addr, 0, sizeof(addr));

			addr.sin_addr.s_addr = htonl(address);
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);

			return addr;
		}

		SocketHandle SocketImpl::invalidSocket()
		{
			return INVALID_SOCKET;
		}

		void SocketImpl::close(SocketHandle sock)
		{
			closesocket(sock);
		}

		void SocketImpl::setBlocking(SocketHandle sock, bool block)
		{
			u_long blocking = block ? 0 : 1;
			ioctlsocket(sock, FIONBIO, &blocking);
		}

		UDP_Socket::Status SocketImpl::getErrorStatus(bool printWSAErrorCodes)
		{
			int co = WSAGetLastError();

			if (printWSAErrorCodes)
				fprintf(stderr, "WSA Error code: %d\n", co);

			switch (co)
			{
			case WSAEWOULDBLOCK:  return UDP_Socket::NotReady;
			case WSAEALREADY:     return UDP_Socket::NotReady;
			case WSAECONNABORTED: return UDP_Socket::Disconnected;
			case WSAECONNRESET:   return UDP_Socket::Disconnected;
			case WSAETIMEDOUT:    return UDP_Socket::Disconnected;
			case WSAENETRESET:    return UDP_Socket::Disconnected;
			case WSAENOTCONN:     return UDP_Socket::Disconnected;
			case WSAEISCONN:      return UDP_Socket::Done; // when connecting a non-blocking socket
			default:              return UDP_Socket::Error;
			}
			
		}


		struct SocketInitializer
		{
			SocketInitializer()
			{
				WSADATA init;
				WSAStartup(MAKEWORD(2, 2), &init);
			}

			~SocketInitializer()
			{
				WSACleanup();
			}
		};

		SocketInitializer globalInitializer;
	} // namespace priv
} // namespace moje