#ifndef CKLIENT_H
#define CKLIENT_H


#include "headers.h"

#ifndef gClient
#define gClient CClient::GetReference()
#endif

#define BUFLEN 200
using namespace std;
using namespace moje;

class CClient : public TSingleton<CClient>
{
private:
	char dane[2 * BUFLEN];
	bool ready, connected, serwerOn, pingPoszedl;
	float t1 = 0, t2 = 0; //czasy do pingowania
	sf::Clock m_timer; // jak sama nazwa wskazuje

	IPAddress ostatniKolegaIP; //ostatni kontrahent :p
	UInt16 ostatniKolegaPort;

	UDP_Socket socket;	
	string odebrane;
	size_t ileOdebranych, gdzie;
	string playerNick;

	UInt16 serwerPort;
	IPAddress *serwerIP;
	
public:

	CClient();
	CClient(string nick, UInt16 portSerwera, IPAddress ipSerwera);

	//z racji globalnego klienta, u¿ywamy metody a nie konstruktora do inicjalizacji
	void initClient(const std::string nick, const UInt16 serverPort, const IPAddress serverIP);

	//metody dogaduj¹ce siê z serwerem
	bool enterToServer();
	bool exitFromServer();
	void sendMessage(string komunikat);
	void receiveMessage();
	void sendPing(bool czyNadacOdp);
	void receivePing(bool nadanyPrzezSerwer, char dane[2 * BUFLEN]);
	bool commitConnectionWithServer();

};
#endif

