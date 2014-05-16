#include "headers.h"

using namespace moje;

//komendy serwer-klient
#define ENTERCOMMAND "IENTER"
#define LEFTCOMMAND "ILEFT"
#define PINGSENDCOMMAND "PINGSEND"
#define PINGRECEIVECOMMAND "PINGRECEIVE"

CClient::CClient()
{

}

CClient::CClient(string nick, UInt16 portSerwera, IPAddress ipSerwera)
{

}

void CClient::initClient(const std::string nick, const UInt16 serverPort, const IPAddress serverIP)
{
	connected = false;
	playerNick = nick;
	serwerPort = serverPort;
	serwerIP = new IPAddress("127.0.0.1");

	if (socket.bind(Socket::AnyPort) == socket.Done)
	{
		ready = true;
		socket.setBlocking(false);
	}
	m_timer.restart();
}

bool CClient::enterToServer()
{
	t1 = m_timer.getElapsedTime().asSeconds();
	t2 = 0.0;
	if (ready)
	{
		socket.send(ENTERCOMMAND, strlen(ENTERCOMMAND), *serwerIP, serwerPort); //próbujê ³¹czyæ
		sendPing(false); //sprawdzam czy po³¹czy³em
		while ((t2 - t1 < 5)) //jeœli w ci¹gu 5 sekund nie przyjdzie ping tzn ¿e serwer nie dzia³a = nie po³¹czy³em siê
		{
			commitConnectionWithServer();
			if (pingPoszedl == false)
			{
				connected = true;
				return true;
			}			
			t2 = m_timer.getElapsedTime().asSeconds();
		}
		connected = false;
		pingPoszedl = false;
		return false;
	}
	else return false;	
}

bool CClient::exitFromServer()
{
	//jeœli bla bla bla to siê roz³¹cz
	if (ready)
	{
		if (connected)
		{
			string dane = "ILEFT " + playerNick;
			if (socket.send(dane.c_str(), dane.length(), *serwerIP, serwerPort) == socket.Done)
			{
				connected = false;
				return true;
			}						
		}
		return false;
	}
	return false;
}

void CClient::sendMessage(string komunikat)
{
	socket.send(komunikat.c_str(), komunikat.length(), *serwerIP, serwerPort);
}

void CClient::receiveMessage()
{
	if (ready)
	{
		if (connected)
		{
			std::memset(dane, '\0', BUFLEN);
			if (socket.receive(dane, BUFLEN, ileOdebranych, ostatniKolegaIP, ostatniKolegaPort) == moje::Socket::Done)
			{
				odebrane = dane;
				receivePing(true, dane);
				receivePing(false, dane);
				/* tutaj odbieramy pozosta³e komunikaty */
			}				
		}
	}
}

void CClient::sendPing(bool czyNadacOdp)
{
	if (ready)
	{
		if (connected)
		{
			if (czyNadacOdp)
			{
				socket.send(PINGRECEIVECOMMAND, strlen(PINGRECEIVECOMMAND), *serwerIP, serwerPort);
			}
			else
			{
				cout << connected << "poszedl ping" << endl;
				if (socket.send(PINGSENDCOMMAND, strlen(PINGSENDCOMMAND), *serwerIP, serwerPort) == moje::Socket::Done)
				{
					serwerOn = false;
					pingPoszedl = true;
				}
			}
		}
		else if (socket.send(PINGSENDCOMMAND, strlen(PINGSENDCOMMAND), *serwerIP, serwerPort) == moje::Socket::Done)
		{
			cout << "ELSE poszedl ping" << endl;
			serwerOn = false;
			pingPoszedl = true;
		}
	}
}

void CClient::receivePing(bool odebranyPrzezSerwer, char *dane)
{
	odebrane = dane;
	if (odebranyPrzezSerwer)
	{
		if (gdzie = odebrane.find(PINGRECEIVECOMMAND) == std::string::npos)
		{
			serwerOn = true;
			pingPoszedl = false;
		}
	}
	else
	{
		if (gdzie = odebrane.find(PINGSENDCOMMAND) == std::string::npos)
		{
			sendPing(true);//nadajê odp do servera
		}
	}
	
}

bool CClient::commitConnectionWithServer()
{
	std::memset(dane, '\0', BUFLEN);
	if (socket.receive(dane, BUFLEN, ileOdebranych, ostatniKolegaIP, ostatniKolegaPort) == moje::Socket::Done)
	{
		cout << "odebralem receive" << endl;
		odebrane = dane;
		if (gdzie = odebrane.find(PINGRECEIVECOMMAND) == std::string::npos)
		{
			cout << "potwierdzenie" << endl;
			serwerOn = true;
			pingPoszedl = false;
			return true;
		}
		else
		{
			cout << "BRAK potwierdzenia" << endl;
			return false;
		}
	}
	else
	{
		return false;
	}
}