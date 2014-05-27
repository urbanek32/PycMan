#ifndef CKLIENT_H
#define CKLIENT_H

#include "headers.h"

#ifndef gClient
#define gClient CClient::GetReference()
#endif

#define BUFLEN 10000
using namespace std;
using namespace moje;

enum Typ
{
	UNKNOWN = -1,
	ENTER, LEAVE, POS, START, STOP,
	SPECIAL = 10, PINGREQUEST, PONGRESPONSE, 
	MASTER = 20, NOTMASTER, MASTERLEFT
};

class CClient : public TSingleton<CClient>
{
public:
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

	//typ odebranego pakietu
	Typ m_typ;

	//do czytania w formacie JSON
	Json::Reader m_reader;
	//pakiet w formacie JSON
	Json::Value m_pakiet, pakietPos;
	//do zapisywania w formacie JSON
	Json::FastWriter m_writer;

	//czy jesteœmy masterem
	bool m_master;
	//nasze ID na serwerze
	int m_clientID;
	
public:
	
	CClient();

	//z racji globalnego klienta, u¿ywamy metody a nie konstruktora do inicjalizacji
	void initClient(const string nick, const UInt16 serverPort, const IPAddress serverIP);

	//metody dogaduj¹ce siê z serwerem
	bool isServerReady(double);
	bool enterToServer();
	bool leaveServer();
	bool receiveMessage(std::string);
	bool receiveMessage(const Typ);
	void receiveMessageToVariable();

	Json::Value getReceivedPacket();
	Typ typeOfReceivedMessage();

	//czy to nasz klient jest g³ówno-dowodz¹cym gr¹
	bool isMasterClient();

	int getClientID();

	//wysy³a pakietPos do serwera
	void sendPacketPos();
};
#endif

