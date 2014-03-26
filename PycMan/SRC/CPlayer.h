#include "headers.h"

#ifndef CPLAYER_H
#define CPLAYER_H
enum kierunek
{
	LEWO, PRAWO, GORA, DOL
};
enum PlayerMode{
	EatFood, EatAll, EatAllEnding
};
extern PlayerMode playerMode;
class CPlayer
{
public:
	CPlayer();

	virtual void Update(sf::RenderWindow & App, sf::Image& ScreenCapture, float deltaTime);

	sf::Sprite & GetSprite();

	sf::Vector2f GetPlayerPosition();

	void RestartPosition();

	float & GetStamina();

	void IncStamina(float value);

	void ActiveFrenzy();

	void DeactivateFrenzy();

	bool IsFrenzy();

	unsigned int & GetEatenGhosts();

	void SetStartPosition(sf::Vector2f & Position);

private:
	sf::Sprite m_Sprite;
	sf::Texture m_Texture;

	float m_Speed;

	sf::Vector2f m_Direction;

	kierunek m_kierunek, m_NowyKierunek;

	bool m_CanGo;

	bool CanTurn(sf::Image& ScreenCapture);

	void Go(sf::Image& ScreenCapture, float & deltaTime);

	sf::Vector2f m_StartPosition;

	float m_Stamina;

	sf::Clock m_regenclock;

	unsigned int m_EatenGhosts;

	sf::Clock m_frameclock;

	int m_currentframe;

	
};
extern sf::Vector2f PlayerPosition;
extern sf::Vector2u PlayerSize;
#endif