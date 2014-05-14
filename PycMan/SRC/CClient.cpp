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
			sendPing();
			receivePing();
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
			if (socket.send(dane.c_str(), dane.length(), *serwerIP, serwerPort) == moje::Socket::Done)
			{
				connected = false;
				return true;
			}
		}
	}
	return false;
}

void CClient::sendMessage(string komunikat)
{

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
				
			}
		}
	}
}


//póki co do nadawania i odebrania moich zapytañ
void CClient::sendPing()
{
	if (ready)
	{
		if (connected)
		{
			//std::string msg = "PINGYOLO";
			if (socket.send(PINGSENDCOMMAND, 8, *serwerIP, serwerPort) != moje::Socket::Done)
			{
				//nie wyslano pingu
			}
			else
			{
				serwerOn = false;
				pingPoszedl = true;
			}
		}
	}
}

void CClient::receivePing()
{
	memset(dane, '\0', BUFLEN);
	if (socket.receive(dane, BUFLEN, ileOdebranych, ostatniKolegaIP, ostatniKolegaPort) == moje::Socket::Done)
	{
		odebrane = dane;
		if (gdzie = odebrane.find(PINGRECEIVECOMMAND) == std::string::npos)
		{
			serwerOn = true;
			pingPoszedl = false;
		}
	}
}