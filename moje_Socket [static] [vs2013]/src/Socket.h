#ifndef SOCKET_H
#define SOCKET_H

#include "Typedefs.h"
#include <vector>


namespace moje
{
	class Socket
	{
	public:

		// status kody zwracane przez funkcje socket�w
		enum Status
		{
			Done, // socket odebra�/wys�a� dane
			NotReady, // socket nie jest gotowy do wysy�ania/odbierania danych
			Disconnected, // socket zosta� roz��czony
			Error // nieoczekiwany b�ad si� zjawi�
		};

		enum
		{
			AnyPort = 0 // specjalna warto��, m�wi�ca �eby system sam sobie wybra� losowy, dostp�pny port
		};


		//destruktor
		virtual ~Socket();

		// ustawia stan blokowania socketu
		void setBlocking(bool blocking);

		// czy socket jest blokowalny czy nie
		bool isBlocking() const;


	protected:

		enum Type
		{
			Tcp, // tcp protok�
			Udp // udp protok�
		};

		// konstruktor
		Socket(Type type);


		// zwraca uchwyt na socket
		SocketHandle getHandle() const;

		// tworzy reprezentacje socketu
		void create();
		void create(SocketHandle handle);

		// zamyka socket
		void close();

	private:
		Type m_type; // typ socketu
		SocketHandle m_socket; // socket deskryptor
		bool m_isBlocking; // stan blokowania
	};
}
#endif