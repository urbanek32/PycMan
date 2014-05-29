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
				// Wróc do menu g³ównego
				gClient.leaveServer();
				triedConnect = false; //zmiana w celu ponownego ³¹czenia z serwerem po wejœciu do poczekalni
				m_Inited = false; // po wyjœciu udawaj ¿e obiekt jest jeszcze nie ustawiony aby przywróciæ stan pocz¹tkowy
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

		if (gClient.receiveMessage(Typ::START)) return 1; //jeœli serwer przys³a³ PLAY to gramy, te¿ powoduje œcinanie okna
													//nowy w¹tek?
	}
	return (-1);	
	
}

void CLobby::m_Init()
{
	//po naciœniêciu (P) startujê ³¹cz¹c siê z serwerem
	m_Title.setString("Connecting...");
	triedConnect = false; //zmienna do sprawdzania czy po wejœciu w loopy próbowa³em siê ³¹czyæ 
							//(u¿ywam by nie zawiesiæ ekranu, bo enter ma w sobie pêtlê
	m_Inited = true;

	m_th = new sf::Thread(&CLobby::tryConnect, this);
	m_th->launch();
}

void CLobby::tryConnect()
{
	//podmiana tekstu w zale¿noœci od po³¹czenia siê z serwerem
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

