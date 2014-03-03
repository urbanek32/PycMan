#include "headers.h"

CMainMenu::CMainMenu()
{
	m_Running = true;
	m_Inited = false;

	m_font.loadFromFile("DATA/arial.ttf");

	m_Play.setFont(m_font);
	m_Play.setColor(sf::Color(255,255,255,255));
	m_Play.setPosition(100, 120);
	m_Play.setCharacterSize(80);
	m_Play.setStyle(sf::Text::Bold);

	m_HighScores.setFont(m_font);
	m_HighScores.setColor(sf::Color(255,255,255,255));
	m_HighScores.setPosition(100, 220);
	m_HighScores.setCharacterSize(80);
	m_HighScores.setStyle(sf::Text::Bold);

	m_Exit.setFont(m_font);
	m_Exit.setColor(sf::Color(255,255,255,255));
	m_Exit.setPosition(100, 320);
	m_Exit.setCharacterSize(80);
	m_Exit.setStyle(sf::Text::Bold);
}

int CMainMenu::Run(sf::RenderWindow & App)
{
	while(m_Running)
	{
		App.clear();

		if(!m_Inited)
			Init();

		while(App.pollEvent(m_Event))
		{
			if(m_Event.type == sf::Event::Closed)
			{
				// Zamknij aplikacje
				m_Running = false;
				return (-1);
			}

			if(m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::E)
			{
				// Zamknij aplikacje
				m_Running = false;
				return (-1);
			}

			if(m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::P)
			{
				// Rozpocznij grê
				return (1);
			}

			if(m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::H)
			{
				// Ekran wyników
				return (2);
			}
		} // end event loop

		App.draw(m_SBackground);


		App.draw(m_Play);
		App.draw(m_HighScores);
		App.draw(m_Exit);
		
		
		App.display();
	}
	return (-1);
}

void CMainMenu::Init()
{
	m_TBackground.loadFromImage(gResMng.Get_Image("DATA/IntroD.png"));
	m_SBackground.setTexture(m_TBackground);

	m_Exit.setString("(E)xit");
	m_Play.setString("(P)lay");
	m_HighScores.setString("(H)igh Scores");

	m_Inited = true;
}