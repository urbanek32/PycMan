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
	m_master = false;
	m_typ = Typ::UNKNOWN;

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

	//std::cout << "isServerReady\n";

	m_pakiet.clear();
	m_pakiet["typ"] = Typ::PINGREQUEST;
	odebrane = m_writer.write(m_pakiet);

	if (socket.send(odebrane.c_str(), odebrane.length(), *serverIP, serverPort) == Socket::Done)
	{
		t1 = m_timer.getElapsedTime().asSeconds();
		while (t2 - t1 < time)
		{
			if (socket.receive(data, BUFLEN, bytesLength, ipSender, portSender) == Socket::Done)
			{
				odebrane = data; //niejawne rzutowanie na stringa

				if (!m_reader.parse(odebrane, m_pakiet))
				{
					std::cout << "Failed to parse configuration\n" << m_reader.getFormattedErrorMessages();
					return false;
				}

				m_typ = static_cast<Typ>(m_pakiet.get("typ", "-1").asInt());

				if (m_typ == Typ::PONGRESPONSE)
				{
					//std::cout << "PONG RESPONSE\n";
					serverOn = true;
					return true;
				}
			}
			t2 = m_timer.getElapsedTime().asSeconds();
		}
		serverOn = false;
		return false;
	}
	else
	{
		return false;
	}

}

bool CClient::enterToServer()
{
	if (isServerReady(3))
	{
		m_pakiet.clear();
		m_pakiet["typ"] = Typ::ENTER;
		odebrane = m_writer.write(m_pakiet);

		socket.send(odebrane.c_str(), odebrane.length(), *serverIP, serverPort);
		connectedToServer = true;

		double t2 = 0, t1;
		t1 = m_timer.getElapsedTime().asSeconds();
		while (t2 - t1 < 5)
		{
			receiveMessageToVariable();
			if (typeOfReceivedMessage() == Typ::MASTER)
			{
				m_master = true;
				std::cout << "Im MASTER\n";
				break;
			}
			else if (typeOfReceivedMessage() == Typ::NOTMASTER)
			{
				m_master = false;
				std::cout << "Master already exist\n";
				break;
			}
			t2 = m_timer.getElapsedTime().asSeconds();
		}

		m_clientID = m_pakiet.get("id", -1).asInt();

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
			m_pakiet.clear();
			m_pakiet["typ"] = Typ::LEAVE;
			odebrane = m_writer.write(m_pakiet);

			socket.send(odebrane.c_str(), odebrane.length(), *serverIP, serverPort);
			connectedToServer = false;
			return true;
		}
	}
	return false;
}

//sprawdza czy otrzymano dany typ wiadomosci
bool CClient::receiveMessage(Typ typ)
{
	if (connectedToServer)
	{
			if (socket.receive(data, BUFLEN, bytesLength, ipSender, portSender) == Socket::Done)
			{
				odebrane = data; //niejawne rzutowanie na stringa

				bool _parsingOK = m_reader.parse(odebrane, m_pakiet); // przeczytaj pakiet jako JSON

				if (!_parsingOK)
				{
					std::cout << "Failed to parse JSON packet\n" << m_reader.getFormattedErrorMessages();
				}

				m_typ = static_cast<Typ>(m_pakiet.get("typ", -1).asInt()); // odczytaj typ pakietu i go zapisz

				if (m_typ == typ)
					return true;
				else
					return false;
			}
	}
	return false;
}

void CClient::receiveMessageToVariable()
{
	if (connectedToServer)
	{
		
		if (socket.receive(data, BUFLEN, bytesLength, ipSender, portSender) == Socket::Done)
		{
			odebrane = data; //niejawne rzutowanie na stringa

			bool _parsingOK = m_reader.parse(odebrane, m_pakiet); // przeczytaj pakiet jako JSON

			if (!_parsingOK)
			{
				std::cout << "Failed to parse JSON packet\n" << m_reader.getFormattedErrorMessages();
			}

			m_typ = static_cast<Typ>(m_pakiet.get("typ", -1).asInt()); // odczytaj typ pakietu i go zapisz			
		}
		else
		{
			m_typ = Typ::UNKNOWN; // nic nie odebrano, wiêc badziew
		}
	}

}

Typ CClient::typeOfReceivedMessage()
{
	return m_typ;
}

Json::Value CClient::getReceivedPacket()
{
	return m_pakiet;
}

bool CClient::isMasterClient()
{
	return m_master;
}

int CClient::getClientID()
{
	return m_clientID;
}

void CClient::sendPacketPos()
{
	if (pakietPos.empty())
	{
		return;
	}

	pakietPos["typ"] = Typ::POS;
	pakietPos["id"] = m_clientID;

	std::string nodebrane = m_writer.write(pakietPos);

	if (socket.send(nodebrane.c_str(), nodebrane.length(), *serverIP, serverPort) != Socket::Done)
	{
		std::cout << "PacketPos cannot be send to server\n";
	}

	pakietPos.clear();
}
