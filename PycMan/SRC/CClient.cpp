#include "headers.h"

using namespace moje;
using namespace std;


//komendy serwer-klient
#define ENTERCOMMAND "IENTER"
#define LEFTCOMMAND "ILEFT"
#define PINGSENDCOMMAND "PINGSEND"
#define PINGRECEIVECOMMAND "PINGRECEIVE"

CClient::CClient(string nick, UInt16 portSerwera, IPAddress ipSerwera)
{
	connected = false;
	playerNick = nick;
	serwerPort = portSerwera;
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
	if (ready)
	{
		socket.send(ENTERCOMMAND, 6, *serwerIP, serwerPort); //próbujê ³¹czyæ
		while ((t2 - t1 < 5)) //jeœli w ci¹gu 5 sekund nie przyjdzie ping tzn ¿e serwer nie dzia³a = nie po³¹czy³em siê
		{
			sendPing(false);
			commitConnectionWithServer();
			if (pingPoszedl == false)
			{
				connected = true;
				return true;
			}			
			t2 = m_timer.getElapsedTime().asSeconds();
		}
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
			socket.send(dane.c_str(), dane.length(), *serwerIP, serwerPort);
			connected = false;
			return true;			
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
				socket.send(PINGRECEIVECOMMAND, 8, *serwerIP, serwerPort);
			}
			else
			{				
				if (socket.send(PINGSENDCOMMAND, 8, *serwerIP, serwerPort) == moje::Socket::Done)
				{
					serwerOn = false;
					pingPoszedl = true;
				}
			}
		}
		else if (socket.send(PINGSENDCOMMAND, 8, *serwerIP, serwerPort) == moje::Socket::Done)
		{
			serwerOn = false;
			pingPoszedl = true;
		}
	}
}

void CClient::receivePing(bool odebranyPrzezSerwer, char dane[2 * BUFLEN])
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
	memset(dane, '\0', BUFLEN);
	if (socket.receive(dane, BUFLEN, ileOdebranych, ostatniKolegaIP, ostatniKolegaPort) == moje::Socket::Done)
	{
		odebrane = dane;
		if (gdzie = odebrane.find(PINGRECEIVECOMMAND) == std::string::npos)
		{
			serwerOn = true;
			pingPoszedl = false;
			return true;
		}
		else return false;
	}
	else return false;
}