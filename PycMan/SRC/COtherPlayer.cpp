#include "COtherPlayer.h"

std::vector<sf::Vector2f>otherPlayersStartPositions;

COtherPlayer::COtherPlayer()
{
}

COtherPlayer::COtherPlayer(const std::string filename, sf::Vector2f StartPosition)
{
	m_Texture.loadFromImage(gResMng.Get_Image(filename));
	m_Sprite.setTexture(m_Texture);
	sf::IntRect rect(0, 0, 25, 25);
	m_Sprite.setTextureRect(rect);
	m_Sprite.setOrigin(25.f / 2.f, 25.f / 2.f);
	m_Sprite.setPosition(StartPosition.x + 1, StartPosition.y + 1);
	
	m_Direction = sf::Vector2f(-1, 0);
	m_kierunek = LEWO;

	m_StartPosition = sf::Vector2f(StartPosition.x + 1, StartPosition.y + 1);

}

void COtherPlayer::Update(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime)
{
	if (gameState == Play)
	{

	}
	App.draw(m_Sprite);
}

sf::Vector2f COtherPlayer::getPosition() const
{
	return m_Sprite.getPosition();
}

sf::Vector2f COtherPlayer::getDirection() const
{
	return m_Direction;
}

int COtherPlayer::getKierunek() const
{
	return m_kierunek;
}

void COtherPlayer::setRemotePosition(sf::Vector2f pos, sf::Vector2f dir, kierunek kier)
{
	m_Sprite.setPosition(pos);
	m_Direction = dir;
	m_kierunek = kier;
}

sf::Sprite & COtherPlayer::GetSprite()
{
	return m_Sprite;
}

void COtherPlayer::ResetPosition()
{
	m_Sprite.setPosition(m_StartPosition);
}

void COtherPlayer::SetStartPosition(sf::Vector2f & Position)
{
	m_StartPosition = sf::Vector2f(Position.x + 1, Position.y + 1);
}

