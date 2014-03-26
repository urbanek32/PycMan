#ifndef SOCKET_H
#define SOCKET_H

#include "Typedefs.h"
#include <vector>


namespace moje
{
	class Socket
	{
	public:

		// status kody zwracane przez funkcje socketów
		enum Status
		{
			Done, // socket odebra³/wys³a³ dane
			NotReady, // socket nie jest gotowy do wysy³ania/odbierania danych
			Disconnected, // socket zosta³ roz³¹czony
			Error // nieoczekiwany b³ad siê zjawi³
		};

		enum
		{
			AnyPort = 0 // specjalna wartoœæ, mówi¹ca ¿eby system sam sobie wybra³ losowy, dostpêpny port
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
			Tcp, // tcp protokó³
			Udp // udp protokó³
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