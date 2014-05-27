#include "headers.h"

#ifndef COPLAYER_H
#define COPLAYER_H

class COtherPlayer
{
public:
	COtherPlayer();

	COtherPlayer(const std::string filename, sf::Vector2f StartPosition, sf::Color color);

	sf::Sprite & GetSprite();

	void Update(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime);

	void ResetPosition();

	void SetStartPosition(sf::Vector2f & Position);

	void setRemotePosition(sf::Vector2f pos, sf::Vector2f dir, kierunek kier = kierunek::LEWO);

	sf::Vector2f getPosition() const;
	sf::Vector2f getDirection() const;
	int getKierunek() const;

private:
	sf::Sprite m_Sprite;
	sf::Texture m_Texture;

	sf::Clock m_frameclock;
	int m_currentframe;

	sf::Vector2f m_Direction;

	sf::Vector2f m_StartPosition;

	kierunek m_kierunek, m_NowyKierunek;
	
};
extern std::vector<sf::Vector2f>otherPlayersStartPositions;
#endif