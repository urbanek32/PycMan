#include "UDP_Socket.h"
#include "SocketImpl.h"

namespace moje
{

	UDP_Socket::UDP_Socket() : Socket(Udp), m_buffer(MaxDatagramSize), m_errorWSAReporting(false)
	{

	}

	Socket::Status UDP_Socket::bind(unsigned short port)
	{
		// utwórz socket je¿eli nie istnieje
		create();

		// zbinduj socket
		sockaddr_in address = priv::SocketImpl::createAddress(INADDR_ANY, port);
		if (::bind(getHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
		{
			fprintf(stderr, "%s %d\n", "Failed to bind socket to port", port);
			return Error;
		}

		return Done;
	}

	void UDP_Socket::unbind()
	{
		close();
	}

	Socket::Status UDP_Socket::send(const void* data, std::size_t size, const IPAddress& remoteAddress, unsigned short remotePort)
	{
		// utwórz socket je¿eli nie istnieje
		create();

		// upewnij siê ¿e dane zmieszcz¹ siê w jednym datagramie
		if (size > MaxDatagramSize)
		{
			fprintf(stderr, "Cannot send data over the network\n(the number of bytes to send is greater than UDP_Socket::MaxDatagramSize)\n");
			return Error;
		}

		// stwórz adres odbiorcy
		sockaddr_in address = priv::SocketImpl::createAddress(remoteAddress.toInteger(), remotePort);

		// wyœlij wszystkie dane
		int sent = sendto(getHandle(), static_cast<const char*>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&address), sizeof(address));

		// sprawdŸ b³êdy
		if (sent < 0)
			return priv::SocketImpl::getErrorStatus(m_errorWSAReporting);

		return Done;
	}

	Socket::Status UDP_Socket::receive(void* data, std::size_t size, std::size_t& received, IPAddress& remoteAddress, unsigned short& remotePort)
	{
		// wyczyœæ wszystkie dane
		received = 0;
		remoteAddress = IPAddress();
		remotePort = 0;

		// sprawdŸ buffor przeznaczenia
		if (!data)
		{
			fprintf(stderr, "Cannot receive data from the network (the destination buffer is invalid)\n");
			return Error;
		}

		// zapisz adres nadawcy tego pakietu
		sockaddr_in address = priv::SocketImpl::createAddress(INADDR_ANY, 0);

		// odbierz czêœæ danych
		priv::SocketImpl::AddrLength addressSize = sizeof(address);
		int sizeReceived = recvfrom(getHandle(), static_cast<char*>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&address), &addressSize);

		// sprawdŸ b³êdy
		if (sizeReceived < 0)
		{
			return priv::SocketImpl::getErrorStatus(m_errorWSAReporting);
		}

		// uzupe³nij dane nadawcy
		received = static_cast<std::size_t>(sizeReceived);
		remoteAddress = IPAddress(ntohl(address.sin_addr.s_addr));
		remotePort = ntohs(address.sin_port);

		return Done;
	}

	void UDP_Socket::EnableWSAErrorCodes(bool enable)
	{
		this->m_errorWSAReporting = enable;
	}

	UInt16 UDP_Socket::getLocalPort() const
	{
		if (getHandle() != priv::SocketImpl::invalidSocket())
		{
			// weŸ informacje o lokalnym sokecie
			sockaddr_in address;
			priv::SocketImpl::AddrLength size = sizeof(address);
			if (getsockname(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
			{
				return ntohs(address.sin_port);
			}
		}

		// Nie uda³o siê wykryæ portu
		return 0;
	}

}