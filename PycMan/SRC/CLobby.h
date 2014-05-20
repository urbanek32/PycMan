#include "headers.h"

#ifndef CLOBBY_H
#define CLOBBY_H

class CLobby : public CScreen
{
public:
	CLobby();

	virtual int Run(sf::RenderWindow & App);

private:
	bool m_Running;
	bool triedConnect;

	bool m_Inited;
	void m_Init();

	sf::Event m_Event;

	sf::Font m_font;

	sf::Text m_Title;

	sf::Thread *m_th;
	sf::Mutex m_mutex;

	void tryConnect();

};
#endif

