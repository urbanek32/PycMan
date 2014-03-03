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

	class CGame *m_Game;

	class CMainMenu *m_Menu;

	class CHighScoresMenu *m_ScoresMenu;

	vector<CScreen*> m_Screens;
};
#endif