#include "headers.h"

CClient *klientSiec;

CLobby::CLobby()
{
	m_Running = true;
	m_Inited = false;
	
	m_font.loadFromFile("DATA/arial.ttf");

	m_Title.setFont(m_font);
	m_Title.setColor(sf::Color(255, 140, 0, 255));
	m_Title.setPosition(140, 70);
	m_Title.setCharacterSize(80);

	klientSiec = new CClient("gracz1", 53000, "127.0.0.1");
}

int CLobby::Run(sf::RenderWindow & App)
{
	m_Init();
	triedConnect = false; //zmienna do sprawdzania czy po wejœciu w loopy próbowa³em siê ³¹czyæ 
						  //(u¿ywam by nie zawiesiæ ekranu, bo enter ma w sobie pêtlê
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
				klientSiec->exitFromServer();
				m_Running = false;
				return (-1);			
			}

			if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::Escape)
			{
				// Wróc do menu g³ównego
				klientSiec->exitFromServer();				
				triedConnect = false; //zmiana w celu ponownego ³¹czenia z serwerem po wejœciu do poczekalni
				return 0;
			}

		} // end event loop

		App.draw(m_Title);

		App.display();

		//podmiana tekstu w zale¿noœci od po³¹czenia siê z serwerem
		if (triedConnect == false)
		{
			if (klientSiec->enterToServer())
			{
				m_Title.setString("Connected\n to server.\n\nWaiting for\n 4 players...");
				triedConnect = true;
			}
			else
			{
				m_Title.setString("Sorry, but\nYou can not \njoin to server...");
				triedConnect = true;
			}
		}
		
		
		//if gracze s¹ to return 1;
	}
	return (-1);	
	
}

void CLobby::m_Init()
{
	//po naciœniêciu (P) startujê ³¹cz¹c siê z serwerem
	m_Title.setString("Connecting...");
	m_Inited = true;
}

