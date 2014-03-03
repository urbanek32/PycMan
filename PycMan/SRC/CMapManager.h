#ifndef CMAPMNG_H
#define CMAPMNG_H

#include "headers.h"

extern unsigned int FoodCount;

struct Food
{
	sf::RectangleShape shape;
	unsigned int type;
};
class CMapManager
{
public: 
	CMapManager();

	void LoadMap(const string filepath);

	void DrawMap(sf::RenderWindow & App);

	deque<sf::RectangleShape> & GetWallShapes();

	deque<Food> & GetFoodShapes();

	bool IsCollision(sf::FloatRect & PlayerRect);

	static CMapManager* GetInstance();

private:
	deque<sf::RectangleShape> m_Walls;

	deque<Food> m_Foods;

	deque<std::string> m_vmap;

	sf::Vector2f center;

};
#endif