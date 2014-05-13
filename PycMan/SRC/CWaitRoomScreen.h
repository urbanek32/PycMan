#include "headers.h"

#ifndef CWAITROOMSCREEN_H
#define CWAITROOMSCREEN_H

class CWaitRoomScreen : public CScreen
{
public:
	CWaitRoomScreen();

	virtual int Run(sf::RenderWindow & App);

private:
	bool m_Running;

	bool m_Inited;
	void m_Init();

	sf::Event m_Event;

	sf::Font m_font;

	sf::Text m_Title;

};
#endif

