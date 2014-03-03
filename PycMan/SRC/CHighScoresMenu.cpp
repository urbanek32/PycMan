#include "headers.h"


CHighScoresMenu::CHighScoresMenu()
{
	m_Running = true;
	m_Inited = false;

	m_font.loadFromFile("DATA/arial.ttf");

	m_Scores.setFont(m_font);
	m_Scores.setColor(sf::Color(220,20,60,255));
	m_Scores.setPosition(100, 110);
	m_Scores.setCharacterSize(40);

	m_Title.setFont(m_font);
	m_Title.setColor(sf::Color(255,140,0,255));
	m_Title.setPosition(80, 10);
	m_Title.setCharacterSize(80);
}

int CHighScoresMenu::Run(sf::RenderWindow & App)
{
	while(m_Running)
	{
		App.clear();

		if(!m_Inited)
			m_Init();

		while(App.pollEvent(m_Event))
		{
			if(m_Event.type == sf::Event::Closed)
			{
				// Zamknij aplikacje
				m_Running = false;
				return (-1);
			}

			if(m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::Escape)
			{
				// Wróc do menu g³ównego
				return (0);
			}

		} // end event loop

		App.draw(m_Title);
		App.draw(m_Scores);


		App.display();
	}
	return (-1);
}

void CHighScoresMenu::m_Init()
{
	ifstream plik("DATA/scores.txt");
	string _tmp;
	
	unsigned long long int _tmp2 = 0;

	if( plik.is_open() )
	{				
		while( !plik.eof() )
		{
			getline(plik, _tmp);
			//cout<<_tmp<<endl;
			std::istringstream iss(_tmp);
			iss>>_tmp2;
			cout<<_tmp2<<endl;
			m_vscores.push_back(_tmp2);
		}
		sort(m_vscores.begin(), m_vscores.end(), std::greater<unsigned long long int>());

		std::ostringstream buf;

		for(unsigned int i=0; i < 10; i++)
		{
			buf<<i+1<<": "<<m_vscores[i]<<"\n";
		}
		m_Scores.setString(buf.str());
	}
	else
	{
		cout<<"Program z plikiem z wynikami\n";
	}

	m_Title.setString("High Scores");
	m_Inited = true;
}

