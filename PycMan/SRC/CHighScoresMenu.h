#include "headers.h"

#ifndef CHSMENU_H
#define CHSMENU_H
class CHighScoresMenu : public CScreen
{
public:
	CHighScoresMenu();

	virtual int Run(sf::RenderWindow & App);

private:
	bool m_Running;

	bool m_Inited;
	void m_Init();

	sf::Event m_Event;

	sf::Font m_font;
	
	sf::Text m_Title, m_Scores;

	std::vector<unsigned long long int> m_vscores;

};
#endif