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

using namespace moje;
using namespace std;

class ServerClient
{
public:
	IPAddress address;
	UInt16 port;
	bool isLife;

	ServerClient(){}

	ServerClient(IPAddress address, UInt16 port)
	{
		this->address = address;
		this->port = port;
		this->isLife = true;
	}	
};

#define BUFLEN 200

//komendy serwera
#define ENTERCOMMAND "IENTER"
#define LEFTCOMMAND "ILEFT"
#define PINGSENDCOMMAND "PINGSEND"
#define PINGRECEIVECOMMAND "PINGRECEIVE"
#define PLAY "PLAY"

//funkcje serwera
void sendToAllClients(char* msg);

void addNewClient(IPAddress adres, UInt16 port);

void deleteClient(IPAddress adres, UInt16 port);

void replyToPing(IPAddress adres, UInt16 port);

void receiveReplyToPing(IPAddress &adres, UInt16 &port);

void pingAllClients();

//wektor trzymaj¹cy pod³¹czonych klientów
vector<ServerClient> clients;

//socket servera
UDP_Socket socket;

int main()
{
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

	//blokowanie socketa...???
	socket.setBlocking(false);

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
		memset(data, '\0', BUFLEN); //czyszczenie miejsca w pamiêci na odebranie danych

		if (socket.receive(data, BUFLEN, bytesLength, ipClient, portClient) == Socket::Done)
		{
			odebrane = data; //niejawne rzutowanie na stringa

			//Je¿eli fraza b¹dŸ znak nie zosta³ odnaleziony w przeszukiwanym tekœcie 
			//to wówczas metoda find zwraca wartoœæ std::string::npos
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
			//tutaj odbiór kolejnych komunikatów....
		}

	  //wysy³anie wiadomoœci które nie s¹ odpowiedziami na otrzymane komunikaty

		//jeœli przy³¹czono 2 graczy to rozeœlij ¿e gramy
		if (clients.size() == 2) 
		{
			sendToAllClients("PLAY");			
		}
		else
		{
			sendToAllClients("STOPPLAY");
		}

	}
	return 0;
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
				it->isLife = true; //aktualizacja pola mówi¹cego o tym czy klient ¿yje
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
		for (UInt32 i = 0; i < clients.size(); i++)
		{
			clients[i].isLife = false;
			if (socket.send(PINGSENDCOMMAND, strlen(PINGSENDCOMMAND), clients[i].address, clients[i].port) != Socket::Done)
			{
				cout << "Cannot send ping to " << clients[i].address.toString() << ":" << clients[i].port << "\n";
			}
		}
	}
}

//odpowiada na otrzymanego od klienta pinga
void replyToPing(IPAddress adres, UInt16 port)
{
	if (socket.send(PINGRECEIVECOMMAND, strlen(PINGRECEIVECOMMAND), adres, port) != Socket::Done)
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