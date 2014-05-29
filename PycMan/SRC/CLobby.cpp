#include "headers.h"

CLobby::CLobby()
{
	m_Running = true;
	m_Inited = false;
	
	m_font.loadFromFile("DATA/arial.ttf");

	m_Title.setFont(m_font);
	m_Title.setColor(sf::Color(255, 140, 0, 255));
	m_Title.setPosition(140, 70);
	m_Title.setCharacterSize(80);


	ifstream plik("DATA/server_ip.txt");
	std::string ip = "127.0.0.1";
	if (plik.is_open())
	{
		plik >> ip;
		plik.close();
	}
	gClient.initClient("gracz1", 53000, ip);
}

int CLobby::Run(sf::RenderWindow & App)
{
	while (m_Running)
	{
		App.clear();

		if (!m_Inited)
			m_Init();

		while (App.pollEvent(m_Event))
		{
			if (m_Event.type == sf::Event::Closed)
			{
				// Zamknij aplikacje
				gClient.leaveServer();
				m_Running = false;
				return (-1);			
			}

			if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::Escape)
			{
				// Wr�c do menu g��wnego
				gClient.leaveServer();
				triedConnect = false; //zmiana w celu ponownego ��czenia z serwerem po wej�ciu do poczekalni
				m_Inited = false; // po wyj�ciu udawaj �e obiekt jest jeszcze nie ustawiony aby przywr�ci� stan pocz�tkowy
				return 0;
			}

		} // end event loop

		App.draw(m_Title);

		App.display();



		m_mutex.lock();
		if (!triedConnect)
		{	
			m_Title.setString(m_Title.getString()+".");
			if (m_Title.getString().getSize() > 13)
				m_Title.setString("Connecting");
		}
		m_mutex.unlock();

		if (gClient.receiveMessage(Typ::START)) return 1; //je�li serwer przys�a� PLAY to gramy, te� powoduje �cinanie okna
													//nowy w�tek?
	}
	return (-1);	
	
}

void CLobby::m_Init()
{
	//po naci�ni�ciu (P) startuj� ��cz�c si� z serwerem
	m_Title.setString("Connecting...");
	triedConnect = false; //zmienna do sprawdzania czy po wej�ciu w loopy pr�bowa�em si� ��czy� 
							//(u�ywam by nie zawiesi� ekranu, bo enter ma w sobie p�tl�
	m_Inited = true;

	m_th = new sf::Thread(&CLobby::tryConnect, this);
	m_th->launch();
}

void CLobby::tryConnect()
{
	//podmiana tekstu w zale�no�ci od po��czenia si� z serwerem
	if (triedConnect == false)
	{
		if (gClient.enterToServer())
		{
			m_mutex.lock();
			m_Title.setString("Connected\n to server.\n\nWaiting for\n  players...");
			m_mutex.unlock();
			triedConnect = true;
			std::cout << gClient.socket.getLocalPort() << "\n";
		}
		else
		{
			m_mutex.lock();
			m_Title.setString("Sorry, but\nYou can not \njoin to server...");
			m_mutex.unlock();
			triedConnect = true;
			
		}
	}
}

