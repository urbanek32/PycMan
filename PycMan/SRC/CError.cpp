#include "headers.h"


CError::CError()
{
	m_Running = true;
	m_Inited = false;
	
	m_font.loadFromFile("DATA/arial.ttf");

	m_Title.setFont(m_font);
	m_Title.setColor(sf::Color(255, 140, 0, 255));
	m_Title.setPosition(200, 200);
	m_Title.setCharacterSize(80);	
}

int CError::Run(sf::RenderWindow & App)
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

			//if dostanê ¿e gramy od servera zwróc 1 - do dopisania
		}
		return (-1);
}

void CError::m_Init()
{
	m_Title.setString("Error!\n You can not \njoin to server");
	m_Inited = true;
}

