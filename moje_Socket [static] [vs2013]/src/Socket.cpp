#include "Socket.h"
#include "SocketImpl.h"
#include <iostream>

namespace moje
{
	Socket::Socket(Type type) : m_type(type), m_socket(priv::SocketImpl::invalidSocket()), m_isBlocking(true)
	{

	}

	Socket::~Socket()
	{
		close();
	}

	void Socket::setBlocking(bool blocking)
	{
		// ustaw je�eli socket zosta� ju� stworzony
		if (m_socket != priv::SocketImpl::invalidSocket())
			priv::SocketImpl::setBlocking(m_socket, blocking);

		m_isBlocking = blocking;
	}

	bool Socket::isBlocking() const
	{
		return m_isBlocking;
	}

	SocketHandle Socket::getHandle() const
	{
		return m_socket;
	}

	void Socket::create()
	{
		// nie tw�rz je�eli socket ju� istnieje
		if (m_socket == priv::SocketImpl::invalidSocket())
		{
			SocketHandle handle = socket(PF_INET, m_type == Tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
			create(handle);
		}
	}

	void Socket::create(SocketHandle handle)
	{
		// nie tw�rz je�eli socket ju� istnieje
		if (m_socket == priv::SocketImpl::invalidSocket())
		{
			// przypisz uchwyt
			m_socket = handle;

			// ustaw stan blokowania
			setBlocking(m_isBlocking);

			if (m_type == Tcp)
			{
				// wy��cz algorytm Nagle'a (m.in: buforowanie pakiet�w tcp)
				int yes = 1;
				if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
				{
					 fprintf(stderr, "Failed to set socket option \"TCP_NODELAY\" ; ""all your TCP packets will be buffered\n") ;
				}

			}
			else
			{
				// w��cz domy�lny broadcast dla socket�w udp
				int yes = 1;
				if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
				{
					fprintf(stderr, "Failed to enable broadcast on UDP socket\n");
				}
			}
		}
	}

	void Socket::close()
	{
		// zamknij socket
		if (m_socket != priv::SocketImpl::invalidSocket())
		{
			priv::SocketImpl::close(m_socket);
			m_socket = priv::SocketImpl::invalidSocket();
		}
	}


}