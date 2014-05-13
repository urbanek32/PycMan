#include "headers.h"

#ifndef CERROR_H
#define CERROR_H

class CError : public CScreen
{
public:
	CError();

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

