#include "headers.h"

CClient *klientSiec;

CLobby::CLobby()
{
	m_Running = true;
	m_Inited = false;
	
	m_font.loadFromFile("DATA/arial.ttf");

	m_Title.setFont(m_font);
	m_Title.setColor(sf::Color(255, 140, 0, 255));
	m_Title.setPosition(200, 200);
	m_Title.setCharacterSize(80);

	klientSiec = new CClient("gracz1", 53000, "127.0.0.1");
}

int CLobby::Run(sf::RenderWindow & App)
{
	if (klientSiec->enterToServer())
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
					m_Running = false;
					return (-1);
				}

				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::Escape)
				{
					// Wróc do menu g³ównego

					return (0);
				}

			} // end event loop

			App.draw(m_Title);

			App.display();
			//if gracze s¹ to return 1;
		}
		return (-1);
	}
	else
	{
		return (4);
	}
}

void CLobby::m_Init()
{
	m_Title.setString("Waiting for\n 4 players...");
	m_Inited = true;
}

