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
	//socket klienta
	UDP_Socket socket;

	//dane odebrane
	char data[2 * BUFLEN];
	string odebrane;
	size_t bytesLength, phrasesPosition;

	//dane nadawcy wiadomosci
	IPAddress ipSender; 
	UInt16 portSender;

	//dane klienta	
	string nickClient;

	//dane serwera
	UInt16 serverPort;
	IPAddress *serverIP;

	//kontrolki po³¹czenia
	bool serverOn;
	bool connectedToServer;
	bool ready;
	bool isWaitingForPingReply;

	//pomocnik
	sf::Clock m_timer; // jak sama nazwa wskazuje
	
public:
	
	CClient();

	//z racji globalnego klienta, u¿ywamy metody a nie konstruktora do inicjalizacji
	void initClient(const string nick, const UInt16 serverPort, const IPAddress serverIP);

	//metody dogaduj¹ce siê z serwerem
	bool isServerReady(double);
	bool enterToServer();
	bool leaveServer();
	bool receiveMessage(string);
};
#endif

