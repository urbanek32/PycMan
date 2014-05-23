#include "headers.h"

GameState gameState;

CGame::CGame()
{
	m_Running = true;
	m_ScoreAdded = false;
	m_Inited = false;
	m_done = false;
	m_lastTime = 0;
	m_currentTime = 0;
	m_count = 5;

	gameState = Prepare;

	m_LevelNumer = 1;

	m_font.loadFromFile("DATA/arial.ttf");

	m_Score = 0;
	m_Mana = 0;
	m_Adrealina = 0;
	m_Lives = 3;

	m_FPS.setFont(m_font);
	m_FPS.setColor(sf::Color(0,255,255,255));
	m_FPS.setPosition(500, 10);
	m_FPS.setCharacterSize(20);
	
	m_TScore.setFont(m_font);
	m_TScore.setColor(sf::Color(0,255,120,255));
	m_TScore.setPosition(500, 30);
	m_TScore.setCharacterSize(20);

	m_TStamina.setFont(m_font);
	m_TStamina.setColor(sf::Color(255,0,255,255));
	m_TStamina.setPosition(500, 50);
	m_TStamina.setCharacterSize(20);

	m_TAdrealina.setFont(m_font);
	m_TAdrealina.setColor(sf::Color(255,255,0,255));
	m_TAdrealina.setPosition(500, 70);
	m_TAdrealina.setCharacterSize(20);

	m_TMana.setFont(m_font);
	m_TMana.setColor(sf::Color(0,0,255,255));
	m_TMana.setPosition(500, 90);
	m_TMana.setCharacterSize(20);

	m_TLives.setFont(m_font);
	m_TLives.setColor(sf::Color(255,0,0,255));
	m_TLives.setPosition(500, 110);
	m_TLives.setCharacterSize(20);

	m_TPause.setFont(m_font);
	m_TPause.setColor(sf::Color(255,255,0,255));
	m_TPause.setPosition(170, 250);
	m_TPause.setCharacterSize(20);

	m_TGameOver.setFont(m_font);
	m_TGameOver.setColor(sf::Color(255,140,0,255));
	m_TGameOver.setPosition(150, 250);
	m_TGameOver.setCharacterSize(20);

	m_TQuit.setFont(m_font);
	m_TQuit.setColor(sf::Color(255,140,0,255));
	m_TQuit.setPosition(150, 250);
	m_TQuit.setCharacterSize(20);

	m_TCount.setFont(m_font);
	m_TCount.setColor(sf::Color(255,140,0,255));
	m_TCount.setPosition(190, 230);
	m_TCount.setCharacterSize(100);
}

int CGame::Run(sf::RenderWindow & App)
{
	m_Running = true;

	//tutaj wstawi� nowy w�tek pingowania serwera
	//...........................................
	while( m_Running )
	{
		App.clear();

#pragma region FPS
		m_currentTime = m_clock.restart().asSeconds();
		float Framerate = 1.f / m_currentTime;
		m_lastTime = m_currentTime;
		if(m_fpsclock.getElapsedTime().asSeconds() > 0.5f)
		{
			std::ostringstream buf;
			buf<<"FPS: "<<floor(Framerate);
			m_FPS.setString(buf.str());
			m_fpsclock.restart();
		}
#pragma endregion
		
		if( !m_Inited )
			m_Init();

		//je�li server straci klienta i b�dzie mniej ni� n graczy
		gClient.receiveMessageToVariable();
		//if (gClient.receiveMessage(Typ::STOP))
		if (gClient.typeOfReceivedMessage() == Typ::STOP)
		{
			RestartGame(true);
			return 3; 
		}

		//gdy otrzymano pakiet z pozycj�
		if (gClient.typeOfReceivedMessage() == Typ::POS)
		{
			cout << "POS ";
			if (gClient.m_pakiet.get("id", -1).asInt() != gClient.getClientID())
			{
				sf::Vector2f p;
				p.x = gClient.m_pakiet["pos"].get("x", BlinkyPosition.x).asFloat();
				p.y = gClient.m_pakiet["pos"].get("y", BlinkyPosition.y).asFloat();
				std::cout << p.x << " " << p.y << "\n";
				m_Enemies[0].setRemotePosition(p);

			}
		}
		//std::cout << gClient.typeOfReceivedMessage() << "\n";

		// to tutaj przyjdzie wiadomo�� i trzeba np wr�ci� do menu, nowy w�tek??? nie ogarniam jak go zrobi�
		
		while(App.pollEvent(m_Event))
		{
			
			if (m_Event.type == sf::Event::GainedFocus)
			{
				m_isActive = true;
			}

			if (m_Event.type == sf::Event::LostFocus)
			{
				m_isActive = false;
			}

			if (m_Event.type == sf::Event::Closed)
			{
				// Zamknij aplikacje
				gClient.leaveServer();
				m_Running = false;
				return (-1);
			}

			if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::F12)
			{
				m_LevelNumer++;

				if (m_LevelNumer > 3)
					m_LevelNumer = 1;

				std::ostringstream style;
				style << "DATA/level" << m_LevelNumer << ".txt";
				m_MapMng->LoadMap(style.str());

				m_Enemies[0].SetStartPosition(BlinkyPosition);
				m_Enemies[1].SetStartPosition(ClydePosition);
				m_Enemies[2].SetStartPosition(InkeyPosition);
				m_Enemies[3].SetStartPosition(PinkyPosition);
				m_Player->SetStartPosition(PlayerPosition);

				m_Captured = false;
				RestartPositions();
				gameState = Prepare;
			}

			if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::F11)
			{
				RestartPositions();
			}

			if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::Escape)
			{
				if (gameState == Play)
					gameState = Quit;
				else if (gameState == Quit)
					gameState = Play;
			}

			if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::P)
			{
				if (gameState == Play)
					gameState = Pauza;
				else if (gameState == Pauza)
					gameState = Play;
			}

			if (gameState == GameOver)
			{
				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::T)
				{
					// zako�cz gr�
					return (-1);
				}

				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::N)
				{
					// rozpocznij od nowa
					RestartGame(true);
				}
			}

			if (gameState == Quit)
			{
				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::T)
				{
					// wr�� do menu
					gClient.leaveServer();
					RestartGame(true);
					return (0);

				}

				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::N)
				{
					gameState = Play;
				}
			}			
			
			
		} // koniec p�tli event�w

		// Jak mamy 0 �y� to GameOver
		if(m_Lives <= 0)
		{
			m_AddScore();
			EndScore = m_Score;
			gameState = GameOver;
		}

		// Rysowanie mapy
		m_MapMng->DrawMap(App);

		// Zrzut ekranu dla kolizji
		if(!m_Captured)
			m_CaptureScreen(App);

		// Obsluga gracza
		m_Player->Update(App, m_ScreenCapture, m_currentTime, m_isActive);

		// Obsluga przeciwnika
		UpdateEnemies(App,m_ScreenCapture, m_currentTime);

		// Wy�wietl ilo�� FPS
		App.draw(m_FPS);

		// Kolizja gracza z jedzeniem i duszkami
		//CheckCollision(m_MapMng, m_Player);

		OtherUpdates(App);

		// Przygotuj gr�
		if(gameState == Prepare)
			PrepareGame(App);

		App.draw(m_TScore);
		App.draw(m_TStamina);
		App.draw(m_TAdrealina);
		App.draw(m_TMana);
		App.draw(m_TLives);

		if(gameState == Pauza)
			App.draw(m_TPause);

		if(gameState == GameOver)
			ShowGameOver(App);

		if(gameState == Quit)
			ShowQuit(App);
		
		App.display();
		//sf::sleep(0.01f);
	} // while(running)
	return (-1);
}

void CGame::m_Init()
{
	m_MapMng = new CMapManager();
	m_MapMng->LoadMap("DATA/level1.txt");
	
	m_Player = new CPlayer();

	m_Inited = true;
	m_Captured = false;

	m_TScore.setString("0");
	m_TStamina.setString("0");
	m_TAdrealina.setString("0");
	m_TMana.setString("0");
	m_TLives.setString("0");
	m_TPause.setString("PAUZA");
	m_TGameOver.setString("Game Over\nZako�czy� gr�?\nT / N");
	m_TQuit.setString("Quit?\nT / N");

	CEnemyGhost *duch = new CEnemyGhost("DATA/BlinkyALL.bmp", BlinkyPosition);
	m_Enemies.push_back(*duch);
	CEnemyGhost *duch2 = new CEnemyGhost("DATA/ClydeALL.bmp", ClydePosition);
	m_Enemies.push_back(*duch2);
	CEnemyGhost *duch3 = new CEnemyGhost("DATA/InkeyALL.bmp", InkeyPosition);
	m_Enemies.push_back(*duch3);
	CEnemyGhost *duch4 = new CEnemyGhost("DATA/PinkyALL.bmp", PinkyPosition);
	m_Enemies.push_back(*duch4);


	m_overShape.setSize(sf::Vector2f(200,150));
	m_overShape.setPosition(120,220);
	m_overShape.setFillColor(sf::Color(0,0,0,255));
	m_overShape.setOutlineThickness(3.f);
	m_overShape.setOutlineColor(sf::Color(139,134,78,255));

	m_overtp.setSize(sf::Vector2f(150,50));
	m_overtp.setPosition(410,200);
	m_overtp.setFillColor(sf::Color(0,0,0,255));

	m_startclock.restart();
}

void CGame::m_CaptureScreen(sf::RenderWindow & App)
{
	m_ScreenCapture = App.capture();
	m_Captured = true;
}

void CGame::UpdateEnemies(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime)
{
	for(vector<class CEnemyGhost>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	{
		it->Update(App,ScreenCapture, deltaTime);
	}
}

void CGame::CheckCollision(CMapManager *maper, CPlayer *player)
{
	
	if(maper->GetFoodShapes().size() > 0)
	{
		// Kolizja pacmana z jedzeniem
		for(std::deque<Food>::iterator it = maper->GetFoodShapes().begin(); it != maper->GetFoodShapes().end(); )
		{
			sf::FloatRect rect = it->shape.getGlobalBounds();

			if(player->GetSprite().getGlobalBounds().intersects(rect))
			{
				if(it->type == 1) // Normalne jedzenie
				{
					m_Score+=10;
				}
				else if(it->type == 2) // Dopalacz
				{
					m_Score+=50;
					player->IncStamina(50);
					player->ActiveFrenzy();
					m_frenzyclock.restart();
				}
				it = maper->GetFoodShapes().erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	else // Je�eli zjdeli�my wszystko za�aduj now� mape
	{
		m_LevelNumer++;
		if(m_LevelNumer > 3)
			m_LevelNumer = 1;

		std::ostringstream style;
		style<<"DATA/level"<<m_LevelNumer<<".txt";
		m_MapMng->LoadMap(style.str());

		RestartGame();
	}
	std::ostringstream buf;
	buf<<"Wynik: "<<m_Score;
	m_TScore.setString(buf.str());

	// Kolizja pacmana z duszkami
	for(vector<class CEnemyGhost>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	{
		sf::FloatRect rect = it->GetSprite().getGlobalBounds();

		if(player->GetSprite().getGlobalBounds().intersects(rect))
		{
			if(player->IsFrenzy() )
			{
				it->ResetPosition();
				m_Score+= 200 * pow(2, float(player->GetEatenGhosts()));
				player->GetEatenGhosts()++;
			}
			else
			{
				m_Lives--;
				RestartPositions();
			}
			
		}
	}
	std::ostringstream buf2;
	buf2<<"Zycia: "<<m_Lives;
	m_TLives.setString(buf2.str());
}

void CGame::RestartPositions()
{
	m_Player->RestartPosition();
	
	m_Enemies[0].ResetPosition();
	m_Enemies[1].ResetPosition();
	m_Enemies[2].ResetPosition();
	m_Enemies[3].ResetPosition();
	
}

void CGame::OtherUpdates(sf::RenderWindow & App)
{
	std::ostringstream buf;
	buf<<"Stamina: "<<m_Player->GetStamina();
	m_TStamina.setString(buf.str());

	if(m_Player->IsFrenzy() )
	{
		if(m_frenzyclock.getElapsedTime().asSeconds() > 5.f )
		{
			playerMode = EatAllEnding;
		}
		if(m_frenzyclock.getElapsedTime().asSeconds() > 7.f )
		{
			m_Player->DeactivateFrenzy();
		}
	}

	if(m_startclock.getElapsedTime().asSeconds() > 0.5f && !m_done)
	{
		RestartPositions();
		m_done = true;
	}

	App.draw(m_overtp);
}

void CGame::RestartGame(bool ResetScore)
{
	RestartPositions();
	m_Captured = false;
	m_ScoreAdded = false;
	m_LevelNumer = 1;
	m_done = false;
	m_count = 5;

	if(ResetScore)
		m_Score = 0;

	m_Mana = 0;
	m_Adrealina = 0;
	m_Lives = 3;

	m_Player->DeactivateFrenzy();

	m_MapMng->LoadMap("DATA/level1.txt");

	m_Enemies[0].SetStartPosition(BlinkyPosition);
	m_Enemies[1].SetStartPosition(ClydePosition);
	m_Enemies[2].SetStartPosition(InkeyPosition);
	m_Enemies[3].SetStartPosition(PinkyPosition);

	m_Player->SetStartPosition(PlayerPosition);

	gameState = Prepare;
}

void CGame::ShowGameOver(sf::RenderWindow & App)
{
	App.draw(m_overShape);
	App.draw(m_TGameOver);

}

void CGame::m_AddScore()
{
	if(!m_ScoreAdded)
	{
		ifstream plik("DATA/scores.txt");
		string _tmp;
		std::vector<unsigned long long int> m_vscores;
		unsigned long long int _tmp2 = 0;

		m_vscores.push_back( (unsigned long long int)(m_Score) );

		if( plik.is_open() )
		{				
			while( !plik.eof() )
			{
				getline(plik, _tmp);
				std::istringstream iss(_tmp);
				iss>>_tmp2;
				//cout<<_tmp2<<endl;
				m_vscores.push_back(_tmp2);
			}

			sort(m_vscores.begin(), m_vscores.end(), std::greater<unsigned long long int>());
			
			plik.close();

			ofstream plik2("DATA/scores.txt");

			if(plik2.is_open())
			{
				unsigned int ile = 0;
				for(std::vector<unsigned long long int>::iterator it = m_vscores.begin(); it != m_vscores.end(); it++)
				{
					if(ile < 10)
					{
						plik2<<*it<<endl;
						ile++;
					}
					else
					{
						break;
					}
				}
				plik2.close();
			}
			else
			{
				cout<<"Problem z plikiem z wynikami\n";
			}
		}
		else
		{
			cout<<"Problem z plikiem z wynikami\n";
		}
		m_ScoreAdded = true;
	}
}

void CGame::ShowQuit(sf::RenderWindow & App)
{
	App.draw(m_overShape);
	App.draw(m_TQuit);
}

void CGame::LoadNextMap()
{
	m_LevelNumer++;

	if(m_LevelNumer > 3)
	{
		m_LevelNumer = 1;
	}

	std::ostringstream style;
	style<<"DATA/level"<<m_LevelNumer<<".txt";
	m_MapMng->LoadMap(style.str());

	RestartGame();
}

void CGame::PrepareGame(sf::RenderWindow & App)
{
	if(m_startclock.getElapsedTime().asSeconds() > 1.f)
	{
		if(m_count > 0)
		{
			m_count--;

			std::ostringstream buf;
			buf<<m_count;
			m_TCount.setString(buf.str());		
		}
		else
		{
			gameState = Play;
			m_count = 5;
		}

		m_startclock.restart();
	}

	if(gameState == Prepare)
	{
		App.draw(m_overShape);
		App.draw(m_TCount);
	}
}