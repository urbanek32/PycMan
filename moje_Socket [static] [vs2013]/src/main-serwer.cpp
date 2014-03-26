/*
#include <stdio.h>
#include <fstream>

#include "Typedefs.h"
#include "UDP_Socket.h"

#pragma comment(lib,"ws2_32.lib")
#define PORT 53000
#define BUFLEN 512

int main(char** argv, int argc)
{

	moje::UDP_Socket socket;
	printf("Ustawiam sie\n");

	if (socket.bind(PORT) != moje::Socket::Done)
	{
		fprintf(stderr, "Wysypalo sie na bindowaniu\n");
	}

	char data[BUFLEN];
	std::size_t received;
	moje::IPAddress nadawca;
	unsigned short port;

	printf("Gotowy do odbierania\n");
	while (1)
	{
		if (socket.receive(data, BUFLEN, received, nadawca, port) != moje::Socket::Done)
		{
			fprintf(stderr, "Wysypalo sie na odbieraniu\n");
			break;
		}
		else
		{
			printf("Odebrano %d bajtow od %s na porcie %d\n",received, nadawca.toString(), port );
		}
	}


	/*SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;

	std::ofstream plik("wynik.txt");

	slen = sizeof(si_other);

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	//keep listening for data
	while (1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			//exit(EXIT_FAILURE);
		}

		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n", buf);

		plik << buf << std::endl;

		//now reply the client with the same data
		if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			//exit(EXIT_FAILURE);
		}
	}

	closesocket(s);
	WSACleanup();
	
	system("pause");*
	return 0;
}*/