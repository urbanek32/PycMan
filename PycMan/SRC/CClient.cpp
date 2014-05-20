#include "headers.h"

using namespace moje;

//komendy serwer-klient
#define ENTERCOMMAND "IENTER"
#define LEFTCOMMAND "ILEFT"
#define PINGSENDCOMMAND "PINGSEND"
#define PINGRECEIVECOMMAND "PINGRECEIVE"

CClient::CClient(){}

void CClient::initClient(const string nick, const UInt16 serverPort, const IPAddress serverIP)
{
	serverOn = false;
	connectedToServer = false;
	ready = false;
	isWaitingForPingReply = false;

	this->nickClient = nick;
	this->serverPort = serverPort;
	this->serverIP = new IPAddress(serverIP);

	//bindowanie socketu z portem
	if (socket.bind(Socket::AnyPort) == socket.Done)
	{
		ready = true;
		socket.setBlocking(false);
	}	
}

bool CClient::isServerReady(double time)
{
	double t1, t2 = 0;
	if (socket.send(PINGSENDCOMMAND, strlen(PINGSENDCOMMAND), *serverIP, serverPort) == Socket::Done)
	{
		t1 = m_timer.getElapsedTime().asSeconds();
		while (t2 - t1 < time)
		{
			if (socket.receive(data, BUFLEN, bytesLength, ipSender, portSender) == Socket::Done)
			{
				odebrane = data; //niejawne rzutowanie na stringa
				if (phrasesPosition = odebrane.find(PINGRECEIVECOMMAND) != string::npos)
				{
					serverOn = true;
					return true;
				}
			}
			t2 = m_timer.getElapsedTime().asSeconds();
		}
		serverOn = false;
		return false;
	}
	else return false;
}

bool CClient::enterToServer()
{
	if (isServerReady(3))
	{
		socket.send(ENTERCOMMAND, strlen(ENTERCOMMAND), *serverIP, serverPort);
		connectedToServer = true;
		return true;
	}
	return false;
}

bool CClient::leaveServer()
{
	if (connectedToServer)
	{
		if (isServerReady(1))
		{
			socket.send(LEFTCOMMAND, strlen(LEFTCOMMAND), *serverIP, serverPort);
			connectedToServer = false;
			return true;
		}
	}
	return false;
}

//sprawdza czy otrzymano dan¹ wiadomoœæ
bool CClient::receiveMessage(string expectedMessage)
{
	if (connectedToServer)
	{
		if (isServerReady(1))
		{
			if (socket.receive(data, BUFLEN, bytesLength, ipSender, portSender) == Socket::Done)
			{
				odebrane = data; //niejawne rzutowanie na stringa
				std::cout << "\t" << odebrane;
				if (phrasesPosition = odebrane.find(expectedMessage) != string::npos) return true; 
			}
		}
	}
	return false;
}

