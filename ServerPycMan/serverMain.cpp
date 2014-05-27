/*
##SERWER##
- nieustannie odbiera pakiety !
- filtruje dzia³ania na podstawie komunikatów
- notuje wejœcia / wyjœcia na serwer
- wysy³a i odpowiada na pingi
- broadcastuje komunikaty do wszystkich klientów
*/


#define WIN32_LEAN_AND_MEAN  
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <algorithm>
#include <string>

#include <UDP_Socket.h>
#include <IPAddress.h>

#include <json/json.h>

using namespace moje;
using namespace std;

class ServerClient
{
public:
	IPAddress address;
	UInt16 port;
	bool isAlive;
	bool isMaster;

	ServerClient(){}

	ServerClient(IPAddress _address, UInt16 _port, bool _isMaster = false)
	{
		this->address = _address;
		this->port = _port;
		this->isAlive = true;
		this->isMaster = _isMaster;
	}	
};

#define BUFLEN 10000

//komendy serwera
#define ENTERCOMMAND "IENTER"
#define LEFTCOMMAND "ILEFT"
#define PINGSENDCOMMAND "PINGSEND"
#define PINGRECEIVECOMMAND "PINGRECEIVE"
#define PLAY "PLAY"

enum Typ
{
	UNKNOWN = -1, 
	ENTER, LEAVE, POS, START, STOP,
	SPECIAL = 10, PINGREQUEST, PONGRESPONSE, 
	MASTER = 20, NOTMASTER, MASTERLEFT
};

//funkcje serwera
void sendToAllClients(char* msg);

void addNewClient(IPAddress adres, UInt16 port);

void deleteClient(IPAddress adres, UInt16 port);

void replyToPing(IPAddress adres, UInt16 port);

void receiveReplyToPing(IPAddress &adres, UInt16 &port);

void pingAllClients();

void receiveMessages();

void broadcastPositionPacket(IPAddress adres, UInt16 port);

//wektor trzymaj¹cy pod³¹czonych klientów
vector<ServerClient> clients;

//socket servera
UDP_Socket socket;

//typ otrzymanego pakietu
Typ typ;

//do czytania w formacie JSON
Json::Reader reader;
//pakiet w formacie JSON
Json::Value pakiet;
//do zapisywania w formacie JSON
Json::FastWriter writer;

//sk³adowe serwera	
UInt16 serverPort = 53000;

//dane nadawcy
IPAddress ipClient;
UInt16 portClient;
string nickClient;

//dane odebrane
char data[2 * BUFLEN];
string odebrane;

//atrybuty odebranych danych
size_t bytesLength, phrasesPosition;

bool _wyslanoPlay = false;

int main()
{
	//blokowanie socketa...???
	//znaczy tyle ¿e funkcja receive nie czeka na pakiety tylko zwraca b³¹d gdy nie ma pakietów do odebrania
	socket.setBlocking(true);

	//bindowanie servera z portem
	if (socket.bind(serverPort) != moje::Socket::Done)
	{
		cout << "Cannot bind with port.\n";
		return -1;
	}
	else
	{
		cout << "Server is ready to running.\n";
	}	

	//g³ówna pêtla serwera
	while (true)
	{
		receiveMessages();

		//wysy³anie wiadomoœci które nie s¹ odpowiedziami na otrzymane komunikaty

		//jeœli przy³¹czono 2 graczy to rozeœlij ¿e gramy
		if (clients.size() == 2) 
		{
			if (!_wyslanoPlay)
			{
				pakiet.clear();
				pakiet["typ"] = Typ::START;
				odebrane = writer.write(pakiet);
				sendToAllClients(const_cast<char*>(odebrane.c_str()));
				std::cout << "PLAY\n";
				_wyslanoPlay = true;
			}
		}
		else
		{
			pakiet.clear();
			pakiet["typ"] = Typ::STOP;
			odebrane = writer.write(pakiet);
			sendToAllClients(const_cast<char*>( odebrane.c_str()) );
			_wyslanoPlay = false;
		}

	}
	return 0;
}

// odbiera pakiet wiadomoœci i ustala jego typ
void receiveMessages()
{
	std::memset(data, '\0', BUFLEN); //czyszczenie miejsca w pamiêci na odebranie danych

	if (socket.receive(data, BUFLEN, bytesLength, ipClient, portClient) == Socket::Done)
	{
		odebrane = data; //niejawne rzutowanie na stringa

		bool _parsingOK = reader.parse(odebrane, pakiet); // przeczytaj pakiet jako JSON

		if (!_parsingOK)
		{
			std::cout << "Failed to parse JSON packet\n" << reader.getFormattedErrorMessages();
		}

		typ = static_cast<Typ>( pakiet.get("typ", -1).asInt() ); // odczytaj typ pakietu i go zapisz

		switch (typ)
		{
			case Typ::ENTER:
			{
				std::cout << "New Client connected\n";
				addNewClient(ipClient, portClient);

				std::cout << "Count of clients: " << clients.size() << "\n";
				break;
			}

			case Typ::LEAVE:
			{
				std::cout << "Client left server\n";
				deleteClient(ipClient, portClient);
				std::cout << "Count of clients: " << clients.size() << "\n";
				break;
			}

			case Typ::POS:
			{
				//std::cout << "Packet with position:"<< pakiet.get("id", -1).asInt() <<"\n";
				//rozeœlij otrzyman¹ pozycje do reszty
				broadcastPositionPacket(ipClient, portClient);
				break;
			}
			
			case Typ::PINGREQUEST:
			{
				//std::cout << "Received ping..." << "\n";
				replyToPing(ipClient, portClient);
				break;
			}
		
			case Typ::PONGRESPONSE:
			{
				std::cout << "Received respons to ping..." << "\n";
				receiveReplyToPing(ipClient, portClient);
				break;
			}

			case Typ::UNKNOWN:
			{
				std::cout << "Received unknown packet... ERROR?!?!?!" << "\n";
				break;
			}
		}


		//tutaj odbiór kolejnych komunikatów....
	}
}

void broadcastPositionPacket(IPAddress adres, UInt16 port)
{
	if (clients.size() > 0)
	{
		for (vector<ServerClient>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			// ma nie odsy³aæ tego samego pakietu do klienta który go wys³a³, ale nie wysy³a go wcale
			//std::cout << it->address.toString() << "\t" << adres.toString() << "\n";
			//std::cout << it->port << "\t" << port << "\n";
			if ( it->port != port)
			{
				std::cout << it->address.toString() << ":" << it->port << "\n";
				if (socket.send(odebrane.c_str(), odebrane.length(), it->address, it->port) != Socket::Done)
				{
					cout << "Cannot send position packet to " << it->address.toString() << ":" << it->port << "\n";
				}
			}
		}
	}
}

//funkcja wywo³ywana po tym jak serwer otrzyma odpowiedŸ na pinga wys³anego przez siebie do clienta
void receiveReplyToPing(IPAddress &adres, UInt16 &port)
{
	if (clients.size() > 0)
	{
		for (vector<ServerClient>::iterator it = clients.begin(); it < clients.end(); it++)
		{
			if (it->address == adres && it->port == port)
			{
				it->isAlive = true; //aktualizacja pola mówi¹cego o tym czy klient ¿yje
				return;
			}
		}
	}	
}

//wysy³anie pingu do wszystkich pod³¹czonych klientów
void pingAllClients()
{
	if (clients.size() > 0)
	{
		pakiet.clear();
		pakiet["typ"] = Typ::PINGREQUEST;
		odebrane = writer.write(pakiet);

		for (UInt32 i = 0; i < clients.size(); i++)
		{
			clients[i].isAlive = false;
			if (socket.send(odebrane.c_str(), odebrane.length(), clients[i].address, clients[i].port) != Socket::Done)
			{
				cout << "Cannot send ping to " << clients[i].address.toString() << ":" << clients[i].port << "\n";
			}
		}
	}
}

//odpowiada na otrzymanego od klienta pinga
void replyToPing(IPAddress adres, UInt16 port)
{
	pakiet.clear();
	pakiet["typ"] = Typ::PONGRESPONSE;
	odebrane = writer.write(pakiet);

	if (socket.send(odebrane.c_str(), odebrane.length(), adres, port) != Socket::Done)
	{
		cout << "Error: failed reply to ping.\n";
	}
}

//broacastuje wiadomoœæ do wszystkich klientów
void sendToAllClients(char* msg)
{
	if (clients.size() > 0)
	{
		for (UInt32 i = 0; i < clients.size(); i++)
		{
			if (socket.send(msg, strlen(msg), clients[i].address, clients[i].port) != Socket::Done)
			{
				cout << "Cannot send to " << clients[i].address.toString() << ":" << clients[i].port << "\n";
			}
		}
	}
}

//dodaje nowego klienta do wektora
void addNewClient(IPAddress adres, UInt16 port)
{
	pakiet.clear();

	if (clients.size() <= 0)
	{
		clients.push_back(ServerClient(adres, port, true));
		pakiet["typ"] = Typ::MASTER;
		std::cout << "MASTER FOUND\n";
	}
	else
	{
		clients.push_back(ServerClient(adres, port));
		std::cout << "NOT master\n";
		pakiet["typ"] = Typ::NOTMASTER;
	}

	std::cout << "ID: " << clients.size() << "\n";
	pakiet["id"] = clients.size();

	odebrane = writer.write(pakiet);
	std::cout << odebrane << "\n";
	if (socket.send(odebrane.c_str(), odebrane.length(), adres, port) != Socket::Done)
	{
		std::cout << "Error: failed to send Master Packet\n";
	}

	std::cout << "New client is added.\n";
	//std::cout << clients.back().address.toString() << ":" << clients.back().port << "\n\n";
}

//usuwa clienta z wektora
void deleteClient(IPAddress adres, UInt16 port)
{
	if (clients.size() > 0)
	{
		for (std::vector<ServerClient>::iterator it = clients.begin(); it < clients.end();)
		{
			if (it->address == adres && it->port == port)
			{
				it = clients.erase(it);
				return;
			}
			else
			{
				it++;
			}
		}
	}
	else
	{
		cout << "Server has not any clients now.\n";
	}
}