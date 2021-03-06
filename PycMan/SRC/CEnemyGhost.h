#include "headers.h"

#ifndef CENEMYGHOST_H
#define CENEMYGHOST_H
class CEnemyGhost
{
public:
	CEnemyGhost();

	CEnemyGhost(const std::string filename, sf::Vector2f StartPosition);

	void Update(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime, bool _randDirection = true);

	sf::Sprite & GetSprite();

	void ResetPosition();

	void SetStartPosition(sf::Vector2f & Position);

	void setRemotePosition(sf::Vector2f pos, sf::Vector2f dir, kierunek kier =  kierunek::LEWO);

	sf::Vector2f getPosition() const;
	sf::Vector2f getDirection() const;
	int getKierunek() const;

private:
	sf::Sprite m_Sprite;
	sf::Texture m_Texture;
	sf::Texture m_Texture2;

	float m_Speed;

	sf::Vector2f m_Direction;

	sf::Vector2f m_StartPosition;

	void Go(sf::Image& ScreenCapture, float & deltaTime);

	bool CanTurn(sf::Image& ScreenCapture);

	kierunek m_kierunek, m_NowyKierunek;
	bool m_CanGo;

	void ChooseDirection();

	sf::Clock m_clock, m_changeclock;

	sf::Clock m_frameclock;

	int m_currentframe, m_changeset;
};
extern sf::Vector2f BlinkyPosition, ClydePosition, InkeyPosition, PinkyPosition, KalibuPosition, YetiPosition;
extern std::vector<sf::Vector2f>enemyStartPositions;
#endif