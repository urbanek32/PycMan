#ifndef CKLIENT_H
#define CKLIENT_H


/*
INSTRUKCJA STEP-BY-STEP JAK TO MA DZIA�A� (uzupe�nij)

##SERWER##
-nieustannie odbiera pakiety
-filtruje dzia�ania na podstawie komunikat�w
-notuje wej�cia/wyj�cia na serwer
-wysy�a i odpowiada na pingi
-broadcastuje komunikaty do wszystkich klient�w



##KLIENT##
-zanim nast�pi jaka kolwiek wymiana komunikat�w z serwerem, wysy�a ping do niego i czeka na odpowied�
-gdy serwer nie odpowie w ci�gu [5sek], og�asza �e serwer jest nieaktywny
-gdy serwer odpowie, wysy�a do niego komunikat �e  do��czamy do serwera
-odpowiada na pingi serwera
-sprawdza czy ostatnio wys�any ping na serwer nie by� dawniej ni� [2sek]. Je�eli by� to si� roz��cza bo serwer pad�
-czeka na 4graczy do rozpocz�cia gry
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

	//z racji globalnego klienta, u�ywamy metody a nie konstruktora do inicjalizacji
	void initClient(const std::string nick, const UInt16 serverPort, const IPAddress serverIP);

	//metody dogaduj�ce si� z serwerem
	bool enterToServer();
	bool exitFromServer();
	void sendMessage(string komunikat);
	void receiveMessage();
	void sendPing(bool czyNadacOdp);
	void receivePing(bool nadanyPrzezSerwer, char dane[2 * BUFLEN]);
	bool commitConnectionWithServer();

};
#endif

