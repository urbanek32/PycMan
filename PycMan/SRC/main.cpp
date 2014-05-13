#include "headers.h"


int main()
{
    srand((unsigned int)time(NULL));

	CGameManager *GameMng = new CGameManager();

	sf::RenderWindow App(sf::VideoMode(800,600,32), "PycMan", sf::Style::Close);
	App.setVerticalSyncEnabled(true);


	GameMng->Run(App);


	App.clear();
	App.close();

	delete GameMng;

	return 0;
}