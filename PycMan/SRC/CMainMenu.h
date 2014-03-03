#include "headers.h"

#ifndef CMAINMENU_H
#define CMAINMENU_H
class CMainMenu : public CScreen
{
public:
	CMainMenu();

	virtual int Run(sf::RenderWindow & App);


private:
	bool m_Running;

	bool m_Inited;
	void Init();

	sf::Event m_Event;

	sf::Sprite m_SBackground;
	sf::Texture m_TBackground;

	sf::Font m_font;

	sf::Text m_Exit, m_HighScores, m_Play;
};
#endif