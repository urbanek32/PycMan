extern float EndScore;

#ifndef CGAMEMANAGER_H
#define CGAMEMANAGER_H

#include "headers.h"

class CGameManager
{
public:
	CGameManager();

	void Run(sf::RenderWindow & App);


private:
	bool m_Inited;
	void m_Init();

	int m_Screen;

public: 
	class CGame *m_Game;

	class CMainMenu *m_Menu;

	class CHighScoresMenu *m_ScoresMenu;

	class CLobby *m_Lobby;

	class CError *m_Error;

	vector<CScreen*> m_Screens;
};
#endif