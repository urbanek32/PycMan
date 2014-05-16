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

class Klient
{
public:
	Klient()
	{

	}

	Klient(IPAddress adr, UInt16 po)
	{
		this->adres = adr;
		this->port = po;
		this->zyje = true;
	}

	IPAddress adres;
	UInt16 port;
	bool zyje;
};

#define ENTERCOMMAND "IENTER"
#define LEFTCOMMAND "ILEFT"
#define PINGCOMMAND "PINGYOLO"
#define PONGCOMMAND "PONGYOLO"
#define GRAMY "GRAMY"

UDP_Socket socket;

#define BUFLEN 200
char dane[2 * BUFLEN];
IPAddress ip_nadawcy;
UInt16 port_nadawcy;

string odebrane;
size_t ilosc_odebranych, gdzie;

string nick;

UInt16 port_serwera = 53000;

vector<Klient>klienci;


void broadcastuj(char* msg);

void dodajKlienta(IPAddress adres, UInt16 port);

void skasujKlienta(IPAddress adres, UInt16 port);

void wyslijPONG(IPAddress adres, UInt16 port);

void przyszedlPONG(IPAddress &adres, UInt16 &port);

void wyslijPINGI();


int main()
{
	socket.setBlocking(false);

	if (socket.bind(port_serwera) != moje::Socket::Done)
	{
		cout << "Brak polaczenia\n";
		return -1;
	}
	else
	{
		cout << "Serwer gotowy do dzialania\n";
	}

	while (true)
	{
		memset(dane, '\0', BUFLEN);
		if (socket.receive(dane, BUFLEN, ilosc_odebranych, ip_nadawcy, port_nadawcy) == moje::Socket::Done)
		{
			odebrane = dane;

			if (gdzie = odebrane.find(ENTERCOMMAND) != std::string::npos)
			{
				std::cout << "IENTER\t" << odebrane.erase(gdzie - 1, strlen(ENTERCOMMAND)) << "\n";
				dodajKlienta(ip_nadawcy, port_nadawcy);
			}
			else if (gdzie = odebrane.find(LEFTCOMMAND) != std::string::npos)
			{
				std::cout << "ILEFT\t" << odebrane.erase(gdzie - 1, strlen(LEFTCOMMAND)) << "\n";
				skasujKlienta(ip_nadawcy, port_nadawcy);
			}
			else if (gdzie = odebrane.find(PINGCOMMAND) != std::string::npos)
			{
				std::cout << "PING PRZYSZEDL" << "\n";
				wyslijPONG(ip_nadawcy, port_nadawcy);
			}
			else if (gdzie = odebrane.find(PONGCOMMAND) != std::string::npos)
			{
				std::cout << "PONG POSZEDL" << "\n";
				przyszedlPONG(ip_nadawcy, port_nadawcy);
			}
			else
			{
				std::cout << dane << "\n";
			}

			broadcastuj(dane);
			std::cout << "Klientow: " << klienci.size() << "\n";

		}
		if (klienci.size() == 2)
		{
			broadcastuj(GRAMY);
		}

	}
	system("pause");
	return 0;
}

void przyszedlPONG(IPAddress &adres, UInt16 &port)
{
	if (klienci.size() > 0)
	{
		for (std::vector<Klient>::iterator it = klienci.begin(); it < klienci.end(); it++)
		{
			if (it->adres == adres && it->port == port)
			{
				it->zyje = true;
				return;
			}

		}
		std::cout << "Nie dalo sie typa zrespic " << adres.toString() << ":" << port << "\n";
	}
	else
	{
		std::cout << "PONG po wuju\n";
	}
}

void wyslijPINGI()
{
	if (klienci.size() > 0)
	{
		for (UInt32 i = 0; i < klienci.size(); i++)
		{
			klienci[i].zyje = false;
			if (socket.send(PINGCOMMAND, strlen(PINGCOMMAND), klienci[i].adres, klienci[i].port) != moje::Socket::Done)
			{
				std::cout << "Nie pinga wyslano do " << klienci[i].adres.toString() << ":" << klienci[i].port << "\n";
			}
		}
	}
}

void wyslijPONG(IPAddress adres, UInt16 port)
{

	std::string msg = "PONGYOLO";
	if (socket.send(msg.c_str(), msg.length(), adres, port) != moje::Socket::Done)
	{
		std::cout << "Nie uda³o siê wys³aæ PONGA\n";
	}
}

void broadcastuj(char* msg)
{
	if (klienci.size() > 0)
	{
		for (UInt32 i = 0; i < klienci.size(); i++)
		{
			if (socket.send(msg, strlen(msg), klienci[i].adres, klienci[i].port) != moje::Socket::Done)
			{
				std::cout << "Nie wyslano do " << klienci[i].adres.toString() << ":" << klienci[i].port << "\n";
			}
		}
	}
}

void dodajKlienta(IPAddress adres, UInt16 port)
{
	klienci.push_back(Klient(adres, port));
	std::cout << "Dodano klienta\n";
}

void skasujKlienta(IPAddress adres, UInt16 port)
{
	if (klienci.size() > 0)
	{
		for (std::vector<Klient>::iterator it = klienci.begin(); it < klienci.end();)
		{
			if (it->adres == adres && it->port == port)
			{
				it = klienci.erase(it);
				return;
			}
			else
			{
				it++;
			}
		}
		std::cout << "Nie dalo sie typa skasowac " << adres.toString() << ":" << port << "\n";
	}
	else
	{
		std::cout << "Kasowanie bezsensu\n";
	}
}