#include "CKlient.h"
#include <UDP_Socket.h>
#include <IPAddress.h>
#include <Windows.h>
#include <time.h>
#include <iostream>

using namespace moje;
using namespace std;


//komendy serwer-klient
#define ENTERCOMMAND "IENTER"
#define LEFTCOMMAND "ILEFT"
#define PINGSENDCOMMAND "PINGSEND"
#define PINGRECEIVECOMMAND "PINGRECEIVE"

double GetTime()
{
	long long f, t;
	QueryPerformanceFrequency((PLARGE_INTEGER)&f);
	QueryPerformanceCounter((PLARGE_INTEGER)&t);
	return (double)t / (double)f;
}

CKlient::CKlient(string nick, UInt16 portSerwera, IPAddress ipSerwera)
{
	playerNick = nick;
	serwerPort = portSerwera;
	serwerIP = new IPAddress("127.0.0.1");

	if (socket.bind(Socket::AnyPort) == socket.Done)
	{
		ready = true;
		socket.setBlocking(false);
	}

}

bool CKlient::enterToServer()
{
	if (ready)
	{
		socket.send(ENTERCOMMAND, 6, *serwerIP, serwerPort);	
		sendPing();
		while ((t2 - t1 < 5 && pingPoszedl == true)) //jeœli w ci¹gu 5 sekund nie przyjdzie ping tzn ¿e serwer nie dzia³a
		{
			t2 = GetTime();
			receivePing(); cout << "ble";
		}
		if (pingPoszedl == false) return true;
		else return false;
	}
	else return false;
	
}

bool CKlient::exitFromServer()
{
	return false;
}

void CKlient::sendMessage(string komunikat)
{

}

void CKlient::receiveMessage()
{

}

void CKlient::sendPing()
{
	if (ready)
	{
		if (connected)
		{
			//std::string msg = "PINGYOLO";
			if (socket.send(PINGSENDCOMMAND, 8, *serwerIP, serwerPort) != moje::Socket::Done)
			{
				
			}
			else
			{
				t1 = GetTime();
				serwerOn = false;
				pingPoszedl = true;
			}
		}
	}
}

void CKlient::receivePing()
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