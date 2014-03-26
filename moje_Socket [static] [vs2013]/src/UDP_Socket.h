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
			MaxDatagramSize = 65507 // maksymalna iloœæ bajtów które mog¹ zostaæ przes³ane na raz w datagramie udp
		};


		//domyœlny konstruktor
		UDP_Socket();

		// zwraca port do którego socket jest zbindowany
		UInt16 getLocalPort() const;

		// zbinduj socket do okreœlonego portu
		Status bind(unsigned short port);

		// odbinduj socket
		void unbind();

		/// \param data          wskaŸnik na obszar danych do wys³ania
		/// \param size          iloœæ bajtów do wys³ania
		/// \param remoteAddress adres odbiorcy
		/// \param remotePort    na jaki port wys³aæ dane
		///
		/// \return Status code
		Status send(const void *data, std::size_t size, const IPAddress &ipaddress, unsigned short port);

		/// \param data          wskaŸnik na tablice do której wpisaæ odebrane dane
		/// \param size          ile maksymalnie bajtów mo¿e zostaæ zapisane
		/// \param received      ile aktualnie odebrano bajtów
		/// \param remoteAddress adres nadawcy
		/// \param remotePort    numer portu nadawcy
		///
		/// \return Status code
		Status receive(void *data, std::size_t size, std::size_t &received, IPAddress &remoteAddress, unsigned short &remotePort);
		

		// czy wywalaæ numery kodów b³êdów
		void EnableWSAErrorCodes(bool enable);


	private:
		std::vector<char> m_buffer; // tymczasowy buffer
		bool m_errorWSAReporting; // czy wywalaæ numery kodów b³êdów

	};

}
#endif