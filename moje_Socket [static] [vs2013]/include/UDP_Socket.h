#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H


#include "Typedefs.h"
#include "Socket.h"
#include "IPAddress.h"


namespace moje
{

	class UDP_Socket : public Socket
	{
	public:

		enum
		{
			MaxDatagramSize = 65507 // maksymalna ilo�� bajt�w kt�re mog� zosta� przes�ane na raz w datagramie udp
		};


		//domy�lny konstruktor
		UDP_Socket();

		// zwraca port do kt�rego socket jest zbindowany
		UInt16 getLocalPort() const;

		// zbinduj socket do okre�lonego portu
		Status bind(unsigned short port);

		// odbinduj socket
		void unbind();

		/// \param data          wska�nik na obszar danych do wys�ania
		/// \param size          ilo�� bajt�w do wys�ania
		/// \param remoteAddress adres odbiorcy
		/// \param remotePort    na jaki port wys�a� dane
		///
		/// \return Status code
		Status send(const void *data, std::size_t size, const IPAddress &ipaddress, unsigned short port);

		/// \param data          wska�nik na tablice do kt�rej wpisa� odebrane dane
		/// \param size          ile maksymalnie bajt�w mo�e zosta� zapisane
		/// \param received      ile aktualnie odebrano bajt�w
		/// \param remoteAddress adres nadawcy
		/// \param remotePort    numer portu nadawcy
		///
		/// \return Status code
		Status receive(void *data, std::size_t size, std::size_t &received, IPAddress &remoteAddress, unsigned short &remotePort);
		

		// czy wywala� numery kod�w b��d�w
		void EnableWSAErrorCodes(bool enable);


	private:
		std::vector<char> m_buffer; // tymczasowy buffer
		bool m_errorWSAReporting; // czy wywala� numery kod�w b��d�w

	};

}
#endif