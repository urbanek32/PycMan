#include "headers.h"

GameState gameState;

ConcurrentQueue<Json::Value> packageQueue;


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

	//tutaj wstawiæ nowy w¹tek pingowania serwera
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

		//ile pakietów na klatkê
		for (int i = 0; i < 3; i++)
		{
			switch (updateMultiplayerStuff())
			{
				case 42:
					//nic sie nie stalo
					break;

				case 3:
					return 3;
					break;

				case 0:
					return 0;
					break;
			}
		}			
		
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

			/*if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::F12 && gClient.isMasterClient())
			{
				m_LevelNumer++;

				if (m_LevelNumer > 3)
					m_LevelNumer = 1;

				std::ostringstream style;
				style << "DATA/level" << m_LevelNumer << ".txt";
				m_MapMng->LoadMap(style.str());

				for (unsigned int i = 0; i < m_Enemies.size(); i++)
				{
					cout << i << " ";
					m_Enemies[i].SetStartPosition(enemyStartPositions[i]);
				}
				cout << m_OtherPlayers.size()<<"=="<<otherPlayersStartPositions.size() << endl;
				for (unsigned int i = 0; i < m_OtherPlayers.size()-1; i++)
				{
					cout << i << " ";
					m_OtherPlayers[i].SetStartPosition(otherPlayersStartPositions[i]);
				}
				
				m_Player->SetStartPosition(PlayerPosition);

				m_Captured = false;

				restarted = true;
				packageQueue.clearQueue();

				RestartPositions();
				gameState = Prepare;
			}

			if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::F11 && gClient.isMasterClient())
			{
				restarted = true;
				packageQueue.clearQueue();
				RestartPositions();
			}*/

			if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::Escape)
			{
				if (gameState == Play)
					gameState = Quit;
				else if (gameState == Quit)
					gameState = Play;
			}

			/*if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::P)
			{
				if (gameState == Play)
					gameState = Pauza;
				else if (gameState == Pauza)
					gameState = Play;
			}*/

			if (gameState == GameOver)
			{
				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::T)
				{
					// zakoñcz grê
					m_mutex.lock();
					restarted = true;
					m_mutex.unlock();
					//receiverThread->terminate();
					return (-1);
				}

				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::N)
				{
					// rozpocznij od nowa
					m_mutex.lock();
					restarted = true;
					m_mutex.unlock();
					//receiverThread->terminate();
					packageQueue.clearQueue();
					RestartGame(true);
				}
			}

			if (gameState == Quit)
			{
				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::T)
				{
					// wróæ do menu
					m_mutex.lock();
					restarted = true;
					m_mutex.unlock();
					//receiverThread->terminate();
					gClient.leaveServer();					
					RestartGame(true);
					return (0);

				}

				if (m_Event.type == sf::Event::KeyPressed && m_Event.key.code == sf::Keyboard::N)
				{
					gameState = Play;
				}
			}			
			
			
		} // koniec pêtli eventów

		// Jak mamy 0 ¿yæ to GameOver
		if(m_Lives <= 0)
		{
			m_AddScore();
			EndScore = m_Score;
			gameState = GameOver;
		}
		else
		{ //wyœwietlaj ¿ycia dopóki masz
			std::ostringstream buf;
			buf << "Lives: " << m_Lives;
			m_TLives.setString(buf.str());
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

		//obs³uga graczy
		UpdateOtherPlayers(App, m_ScreenCapture, m_currentTime);

		// Wyœwietl iloœæ FPS
		App.draw(m_FPS);

		// Kolizja gracza z jedzeniem i duszkami
		OtherUpdates(App);		

		CheckCollision(m_MapMng, m_Player);

		//wyœlij nasz pakiet na serwer
		gClient.sendPacketPos();

		// Przygotuj grê
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
int ile = 0;
void CGame::m_Init()
{
	m_MapMng = new CMapManager();
	m_MapMng->LoadMap("DATA/level1.txt");
	
	m_Player = new CPlayer();

	m_Inited = true;
	m_Captured = false;

	restarted = false;

	m_TScore.setString("0");
	m_TStamina.setString("0");
	m_TAdrealina.setString("0");
	m_TMana.setString("0");
	m_TLives.setString("0");
	m_TPause.setString("PAUZA");
	m_TGameOver.setString("Game Over\nZakoñczyæ grê?\nT / N");
	m_TQuit.setString("Quit?\nT / N");

	//tworzê nowych graczy
	COtherPlayer *subPlayer1 = new COtherPlayer("DATA/pacmanALL.bmp", otherPlayersStartPositions[0], sf::Color::Red);
	m_OtherPlayers.push_back(*subPlayer1);
	COtherPlayer *subPlayer2 = new COtherPlayer("DATA/pacmanALL.bmp", otherPlayersStartPositions[0], sf::Color::Green);
	m_OtherPlayers.push_back(*subPlayer2);
	COtherPlayer *subPlayer3 = new COtherPlayer("DATA/pacmanALL.bmp", otherPlayersStartPositions[0], sf::Color(255,150,0), false);
	m_OtherPlayers.push_back(*subPlayer3);

	//duchy
	CEnemyGhost *duch = new CEnemyGhost("DATA/BlinkyALL.bmp", enemyStartPositions[0]);
	m_Enemies.push_back(*duch);
	CEnemyGhost *duch2 = new CEnemyGhost("DATA/ClydeALL.bmp", enemyStartPositions[1]);
	m_Enemies.push_back(*duch2);
	CEnemyGhost *duch3 = new CEnemyGhost("DATA/InkeyALL.bmp", enemyStartPositions[2]);
	m_Enemies.push_back(*duch3);
	CEnemyGhost *duch4 = new CEnemyGhost("DATA/PinkyALL.bmp", enemyStartPositions[3]);
	m_Enemies.push_back(*duch4);
	
	
	//nowe
	CEnemyGhost *duch5 = new CEnemyGhost("DATA/BlinkyALL.bmp", enemyStartPositions[4]);
	m_Enemies.push_back(*duch5);
	CEnemyGhost *duch6 = new CEnemyGhost("DATA/ClydeALL.bmp", enemyStartPositions[5]);
	m_Enemies.push_back(*duch6);
	CEnemyGhost *duch7 = new CEnemyGhost("DATA/InkeyALL.bmp", enemyStartPositions[6]);
	m_Enemies.push_back(*duch7);
	CEnemyGhost *duch8 = new CEnemyGhost("DATA/PinkyALL.bmp", enemyStartPositions[7]);
	m_Enemies.push_back(*duch8);
	CEnemyGhost *duch9 = new CEnemyGhost("DATA/BlinkyALL.bmp", enemyStartPositions[8]);
	m_Enemies.push_back(*duch9);
	CEnemyGhost *duch10 = new CEnemyGhost("DATA/ClydeALL.bmp", enemyStartPositions[9]);
	m_Enemies.push_back(*duch10);
	

	// wywali siê jak jest wiêcej wrogów ni¿ pozycji startowych
	assert(m_Enemies.size() <= enemyStartPositions.size());

	m_overShape.setSize(sf::Vector2f(200,150));
	m_overShape.setPosition(120,220);
	m_overShape.setFillColor(sf::Color(0,0,0,255));
	m_overShape.setOutlineThickness(3.f);
	m_overShape.setOutlineColor(sf::Color(139,134,78,255));

	m_overtp.setSize(sf::Vector2f(150,50));
	m_overtp.setPosition(410,200);
	m_overtp.setFillColor(sf::Color(0,0,0,255));

	receiverThread = new sf::Thread(&CGame::receivePackageInNewThread, this);
	receiverThread->launch();

	m_startclock.restart();
}

void CGame::receivePackageInNewThread()
{
	sf::Mutex mutex;
	gClient.socket.setBlocking(true);
	bool warunek;

	mutex.lock();
	warunek = restarted;
	mutex.unlock();

	while (!warunek)
	{
		gClient.receiveMessageToVariable();
		if (gClient.typeOfReceivedMessage() == Typ::POS)
		{
			packageQueue.push(gClient.m_pakiet);
		}
		mutex.lock();
		warunek = restarted;
		mutex.unlock();
	}
	gClient.socket.setBlocking(false);
}

// domyœlnie zwracamy 42, czyli nic siê nie dzieje
int CGame::updateMultiplayerStuff()
{	
	if (gClient.typeOfReceivedMessage() == Typ::STOP)
	{
		m_mutex.lock();
		restarted = true;
		m_mutex.unlock();
		//receiverThread->terminate();
		gClient.leaveServer();
		RestartGame(true);
		return 0;
	}

	if (!packageQueue.isEmpty())
	{
		Json::Value pakiecik = packageQueue.pop();

		int _id = pakiecik.get("id", -1).asInt();

		//syfny pakiet
		if (_id == -1)
			return 42;		

		//na wypadek gdybyœmy odebrali swój w³asny pakiet
		if (_id != gClient.getClientID())
		{
			_id = calculateID(_id);

			int kier;
			sf::Vector2f p, k;
			p.x = pakiecik["pos"].get("x", m_OtherPlayers[_id].getPosition().x).asFloat();
			p.y = pakiecik["pos"].get("y", m_OtherPlayers[_id].getPosition().y).asFloat();

			kier = pakiecik.get("kierunek", m_OtherPlayers[_id].getKierunek()).asInt();

			k.x = pakiecik["direction"].get("x", m_OtherPlayers[_id].getDirection().x).asFloat();
			k.y = pakiecik["direction"].get("y", m_OtherPlayers[_id].getDirection().y).asFloat();
			//std::cout << p.x << " " << p.y << "\n";
			m_OtherPlayers[_id].setRemotePosition(p, k, static_cast<kierunek>(kier));

			
			std::string kropencja = "0";
			bool dopalony = false; // ¿eby ci¹gle nie dodawa³ dopalacza
			int iloscKropek = pakiecik["kropka"].get("ilosc", 0).asInt();

			//aktualizacja kropek
			for (int i = 0; i < iloscKropek; i++)
			{
				_itoa(i, const_cast<char*>(kropencja.c_str()), 10);

				sf::FloatRect rect;
				rect.height = pakiecik["kropka"][kropencja]["height"].asInt();
				rect.left = pakiecik["kropka"][kropencja]["left"].asInt();
				rect.top = pakiecik["kropka"][kropencja]["top"].asInt();
				rect.width = pakiecik["kropka"][kropencja]["width"].asInt();

				//dopalacz
				sf::FloatRect rect2;
				rect2.height = pakiecik["dopalacz"]["height"].asInt();
				rect2.left = pakiecik["dopalacz"]["left"].asInt();
				rect2.top = pakiecik["dopalacz"]["top"].asInt();
				rect2.width = pakiecik["dopalacz"]["width"].asInt();

				bool act = pakiecik["dopalacz"].get("activate", false).asBool();

				if (act)
				{
					m_Player->ActiveFrenzy();
					m_frenzyclock.restart();
				}

				for (std::deque<Food>::iterator it = m_MapMng->GetFoodShapes().begin(); it != m_MapMng->GetFoodShapes().end();)
				{
					if (it->shape.getGlobalBounds().intersects(rect))
					{
						it = m_MapMng->GetFoodShapes().erase(it);
						m_Score += 10; //aktualizacja wyniku od przychodz¹cego po³¹czenia
					}
					else if (it->shape.getGlobalBounds().intersects(rect2) && !dopalony)
					{
						it = m_MapMng->GetFoodShapes().erase(it);
						m_Score += 50; //aktualizacja wyniku od przychodz¹cego po³¹czenia
						dopalony = true;
					}
					else
					{
						it++;
					}
				}

				
			}

			//jako klient wyci¹gamy jeszcze pozycje duszków
			if (!gClient.isMasterClient())
			{
				std::string e = "0";
				for (unsigned int i = 0; i < m_Enemies.size(); i++)
				{
					p.x = pakiecik["enemies"][e]["pos"].get("x", m_Enemies[i].getPosition().x).asFloat();
					p.y = pakiecik["enemies"][e]["pos"].get("y", m_Enemies[i].getPosition().y).asFloat();
					k.x = pakiecik["enemies"][e]["dir"].get("x", m_Enemies[i].getDirection().x).asFloat();
					k.y = pakiecik["enemies"][e]["dir"].get("y", m_Enemies[i].getDirection().y).asFloat();
					kier = pakiecik["enemies"][e].get("kierunek", m_Enemies[i].getKierunek()).asInt();
					
					m_Enemies[i].setRemotePosition(p, k, static_cast<kierunek>(kier));

					_itoa(i, const_cast<char*>(e.c_str()), 10);
				}

				//aktualizacja wyniku
				m_Score = pakiecik.get("wynik", m_Score).asFloat();
			}
		}
	}
	return 42;
}

void CGame::m_CaptureScreen(sf::RenderWindow & App)
{
	m_ScreenCapture = App.capture();
	m_Captured = true;
}

void CGame::UpdateOtherPlayers(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime)
{
	for (unsigned int i = 0; i < m_OtherPlayers.size(); i++)
	{
		if (!m_OtherPlayers[i].isPlaceHolder())
		{
			m_OtherPlayers[i].Update(App, ScreenCapture, deltaTime);
		}
	}
}

void CGame::UpdateEnemies(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime)
{
	std::string e = "0";

	for (unsigned int i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i].Update(App,ScreenCapture, deltaTime, gClient.isMasterClient());

		if (gClient.isMasterClient())
		{
			gClient.pakietPos["enemies"][e]["pos"]["x"] = m_Enemies[i].getPosition().x;
			gClient.pakietPos["enemies"][e]["pos"]["y"] = m_Enemies[i].getPosition().y;
			gClient.pakietPos["enemies"][e]["dir"]["x"] = m_Enemies[i].getDirection().x;
			gClient.pakietPos["enemies"][e]["dir"]["y"] = m_Enemies[i].getDirection().y;
			gClient.pakietPos["enemies"][e]["kierunek"] = m_Enemies[i].getKierunek();
		}

		_itoa(i, const_cast<char*>(e.c_str()), 10);
	}
}

void CGame::CheckCollision(CMapManager *maper, CPlayer *player)
{
	int iloscKolizji = 0;
	std::string _e = "0";
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
					if (gClient.isMasterClient())
					{
						m_Score += 10;
					}

					//wysy³am wsp do usuniêcia kropki
					gClient.pakietPos["kropka"]["ilosc"] = ++iloscKolizji;
					_itoa(iloscKolizji - 1, const_cast<char*>(_e.c_str()), 10);
					gClient.pakietPos["kropka"][_e]["height"] = it->shape.getGlobalBounds().height;
					gClient.pakietPos["kropka"][_e]["left"] = it->shape.getGlobalBounds().left;
					gClient.pakietPos["kropka"][_e]["top"] = it->shape.getGlobalBounds().top;
					gClient.pakietPos["kropka"][_e]["width"] = it->shape.getGlobalBounds().width;	
					//std::cout << iloscKolizji << "\t" << _e << "\n";

				}
				else if(it->type == 2) // Dopalacz
				{
					if (gClient.isMasterClient())
					{
						m_Score += 50;
					}

					player->IncStamina(50);
					player->ActiveFrenzy();

					//wysy³am wsp do usuniêcia kropki
					gClient.pakietPos["dopalacz"]["height"] = it->shape.getGlobalBounds().height;
					gClient.pakietPos["dopalacz"]["left"] = it->shape.getGlobalBounds().left;
					gClient.pakietPos["dopalacz"]["top"] = it->shape.getGlobalBounds().top;
					gClient.pakietPos["dopalacz"]["width"] = it->shape.getGlobalBounds().width;
					gClient.pakietPos["dopalacz"]["activate"] = true;
					gClient.pakietPos["kropka"]["ilosc"] = ++iloscKolizji;

					m_frenzyclock.restart();
				}


				it = maper->GetFoodShapes().erase(it);
			}
			else
			{
				it++;
			}

			
		}

		//wysy³am pakiet wyniku do innych graczy
		if (gClient.isMasterClient()) gClient.pakietPos["wynik"] = m_Score;
	}
	else // Je¿eli zjdeliœmy wszystko za³aduj now¹ mape
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
	/*for(vector<class CEnemyGhost>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
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
	m_TLives.setString(buf2.str());*/
}

void CGame::RestartPositions()
{
	m_Player->RestartPosition();
	
	for (unsigned int i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i].ResetPosition();
	}

	for (unsigned int i = 0; i < m_OtherPlayers.size(); i++)
	{
		m_OtherPlayers[i].ResetPosition();
	}
	
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

	for (unsigned int i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i].SetStartPosition(enemyStartPositions[i]);
	}

	for (unsigned int i = 0; i < m_OtherPlayers.size(); i++)
	{
		m_OtherPlayers[i].SetStartPosition(otherPlayersStartPositions[0]);
	}

	m_Player->SetStartPosition(PlayerPosition);

	packageQueue.clearQueue();

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

int CGame::calculateID(int _ID)
{
	/*int newID = -1;
	switch (gClient.getClientID())
	{
		case 1:
		{
			if (_ID == 2)
				newID = 0;
			else if (_ID == 3)
				newID = 1;
			else if (_ID == 4)
				newID = 2;
			//newID = (gClient.getClientID() + _ID) % 3;
			break;
		}

		case 2:
		{
			if (_ID == 1)
				newID = 0;
			else if (_ID == 3)
				newID = 1;
			else if (_ID == 4)
				newID = 2;
			break;		  
		}

		case 3:
		{
			if (_ID == 1)
				newID = 0;
			else if (_ID == 2)
				newID = 1;
			else if (_ID == 4)
				newID = 2;
			break;
		}

		case 4:
		{
			if (_ID == 1)
				newID = 0;
			else if (_ID == 2)
				newID = 1;
			else if (_ID == 3)
				newID = 2;
			break;
		}
	}

	return newID;*/

	//Czary mary: Reducto! Standardowa ksiêga zaklêæ optymalizacyjnych
	int newID = -1;
	int gcID = gClient.getClientID();
	if (gcID >= 1 && gcID <= 4)     //warunek spe³nialnoœci switcha
	{
		newID = (gcID < _ID) ? _ID - 2 : _ID - 1;       //zredukowane dzia³anie switcha
	}

	return newID;

}