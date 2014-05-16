#ifndef CKLIENT_H
#define CKLIENT_H


/*
INSTRUKCJA STEP-BY-STEP JAK TO MA DZIA£AÆ (uzupe³nij)

##SERWER##
-nieustannie odbiera pakiety
-filtruje dzia³ania na podstawie komunikatów
-notuje wejœcia/wyjœcia na serwer
-wysy³a i odpowiada na pingi
-broadcastuje komunikaty do wszystkich klientów



##KLIENT##
-zanim nast¹pi jaka kolwiek wymiana komunikatów z serwerem, wysy³a ping do niego i czeka na odpowiedŸ
-gdy serwer nie odpowie w ci¹gu [5sek], og³asza ¿e serwer jest nieaktywny
-gdy serwer odpowie, wysy³a do niego komunikat ¿e  do³¹czamy do serwera
-odpowiada na pingi serwera
-sprawdza czy ostatnio wys³any ping na serwer nie by³ dawniej ni¿ [2sek]. Je¿eli by³ to siê roz³¹cza bo serwer pad³
-czeka na 4graczy do rozpoczêcia gry
-......


*/



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

