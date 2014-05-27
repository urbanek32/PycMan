#include "headers.h"

float EndScore;
CGameManager::CGameManager()
{
	

	m_Inited = false;
	
}

void CGameManager::Run(sf::RenderWindow & App)
{
	if(!m_Inited)
		m_Init();

	while( m_Screen >= 0 )
	{
		m_Screen = m_Screens[m_Screen]->Run(App);
	}
}

void CGameManager::m_Init()
{
	gResMng.Load_Image("DATA/pacmanALL.bmp");
	gResMng.Load_Image("DATA/pacmanALL2.bmp");

	gResMng.Load_Image("DATA/BlinkyALL.bmp");
	gResMng.Load_Image("DATA/ClydeALL.bmp");
	gResMng.Load_Image("DATA/InkeyALL.bmp");
	gResMng.Load_Image("DATA/PinkyALL.bmp");
	gResMng.Load_Image("DATA/Fear.bmp");

	gResMng.Load_Image("DATA/cherry.bmp");

	gResMng.Load_Image("DATA/IntroD.png");

	m_Screen = 0;
	m_Game = new CGame();
	m_Menu = new CMainMenu();
	m_ScoresMenu = new CHighScoresMenu();
	m_Lobby = new CLobby();

	m_Screens.push_back(m_Menu); // 0
	m_Screens.push_back(m_Game); // 1
	m_Screens.push_back(m_ScoresMenu); // 2
	m_Screens.push_back(m_Lobby); // 3

}