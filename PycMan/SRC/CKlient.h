#include <UDP_Socket.h>
#include <IPAddress.h>

#ifndef CKLIENT_H
#define CKLIENT_H

#define BUFLEN 200
using namespace std;
using namespace moje;

class CKlient
{
public:
	char dane[2 * BUFLEN];
	bool ready, connected, serwerOn, pingPoszedl;
	double t1 = 0, t2 = 0; //czasy do pingowania

	IPAddress ostatniKolegaIP; //ostatni kontrahent :p
	UInt16 ostatniKolegaPort;

	UDP_Socket socket;	
	string odebrane;
	size_t ileOdebranych, gdzie;
	string playerNick;

	UInt16 serwerPort;
	IPAddress *serwerIP;
	
	CKlient(string nick, UInt16 portSerwera, IPAddress ipSerwera);

	//metody dogaduj¹ce siê z serwerem
	bool enterToServer();
	bool exitFromServer();
	void sendMessage(string komunikat);
	void receiveMessage();
	void sendPing();
	void receivePing();

};
#endif

