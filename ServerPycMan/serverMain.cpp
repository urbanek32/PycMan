/*
##SERWER##
- nieustannie odbiera pakiety !
- filtruje dzia�ania na podstawie komunikat�w
- notuje wej�cia / wyj�cia na serwer
- wysy�a i odpowiada na pingi
- broadcastuje komunikaty do wszystkich klient�w
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

	ServerClient(){}

	ServerClient(IPAddress address, UInt16 port)
	{
		this->address = address;
		this->port = port;
		this->isAlive = true;
	}	
};

#define BUFLEN 500

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
	SPECIAL = 10, PINGREQUEST, PONGRESPONSE
};

//funkcje serwera
void sendToAllClients(char* msg);

void addNewClient(IPAddress adres, UInt16 port);

void deleteClient(IPAddress adres, UInt16 port);

void replyToPing(IPAddress adres, UInt16 port);

void receiveReplyToPing(IPAddress &adres, UInt16 &port);

void pingAllClients();

void receiveMessages();

//wektor trzymaj�cy pod��czonych klient�w
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

//sk�adowe serwera	
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

int main()
{
	//blokowanie socketa...???
	//znaczy tyle �e funkcja receive nie czeka na pakiety tylko zwraca b��d gdy nie ma pakiet�w do odebrania
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

	//g��wna p�tla serwera
	while (true)
	{
		receiveMessages();

		//wysy�anie wiadomo�ci kt�re nie s� odpowiedziami na otrzymane komunikaty

		//je�li przy��czono 2 graczy to roze�lij �e gramy
		if (clients.size() == 2) 
		{
			pakiet.clear();
			pakiet["typ"] = Typ::START;
			odebrane = writer.write(pakiet);
			sendToAllClients( const_cast<char*>( odebrane.c_str()) );
			//std::cout << "PLAY\n";
		}
		else
		{
			pakiet.clear();
			pakiet["typ"] = Typ::STOP;
			odebrane = writer.write(pakiet);
			sendToAllClients(const_cast<char*>( odebrane.c_str()) );
		}

	}
	return 0;
}

// odbiera pakiet wiadomo�ci i ustala jego typ
void receiveMessages()
{
	std::memset(data, '\0', BUFLEN); //czyszczenie miejsca w pami�ci na odebranie danych

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
				std::cout << "Packet with position\n";
				//roze�lij otrzyman� pozycje do reszty
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

		/*
		//Je�eli fraza b�d� znak nie zosta� odnaleziony w przeszukiwanym tek�cie 
		//to w�wczas metoda find zwraca warto�� std::string::npos
		if (phrasesPosition = odebrane.find(ENTERCOMMAND) != string::npos)
		{
			std::cout << "New Client connected.\t" << odebrane.erase(phrasesPosition - 1, strlen(ENTERCOMMAND)) << "\n";
			addNewClient(ipClient, portClient);
			cout << "Count of clients: " << clients.size() << "\n";
		}
		else if (phrasesPosition = odebrane.find(LEFTCOMMAND) != string::npos)
		{
			std::cout << "Client left server:\t" << odebrane.erase(phrasesPosition - 1, strlen(LEFTCOMMAND)) << "\n";
			deleteClient(ipClient, portClient);
			cout << "Count of clients: " << clients.size() << "\n";
		}
		else if (phrasesPosition = odebrane.find(PINGSENDCOMMAND) != string::npos)
		{
			//std::cout << "Received ping..." << "\n";
			replyToPing(ipClient, portClient);
		}
		else if (phrasesPosition = odebrane.find(PINGRECEIVECOMMAND) != string::npos)
		{
			std::cout << "Received respons to ping..." << "\n";
			receiveReplyToPing(ipClient, portClient);
		}
		*/

		//tutaj odbi�r kolejnych komunikat�w....
	}
}

//funkcja wywo�ywana po tym jak serwer otrzyma odpowied� na pinga wys�anego przez siebie do clienta
void receiveReplyToPing(IPAddress &adres, UInt16 &port)
{
	if (clients.size() > 0)
	{
		for (vector<ServerClient>::iterator it = clients.begin(); it < clients.end(); it++)
		{
			if (it->address == adres && it->port == port)
			{
				it->isAlive = true; //aktualizacja pola m�wi�cego o tym czy klient �yje
				return;
			}
		}
	}	
}

//wysy�anie pingu do wszystkich pod��czonych klient�w
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

//broacastuje wiadomo�� do wszystkich klient�w
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
	clients.push_back(ServerClient(adres, port));
	cout << "New client is added.\n";
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