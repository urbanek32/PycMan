#include "headers.h"

sf::Vector2f PlayerPosition;
sf::Vector2u PlayerSize;
PlayerMode playerMode;

CPlayer::CPlayer()
{
	m_Texture.loadFromImage(gResMng.Get_Image("DATA/pacmanALL.bmp"));
	m_Sprite.setTexture(m_Texture);
	sf::IntRect rect(0,0, 25,25);
	m_Sprite.setTextureRect(rect);
	m_Sprite.setOrigin(25.f/2.f, 25.f/2.f);
	m_Sprite.setPosition(PlayerPosition.x,PlayerPosition.y+1);

	m_StartPosition = sf::Vector2f(PlayerPosition.x,PlayerPosition.y+1);
	PlayerSize = sf::Vector2u(25,25);

	m_Speed = 60.f;
	m_Direction = sf::Vector2f(-1, 0);

	m_kierunek = LEWO;
	m_CanGo = true;

	m_Stamina = 100;
	playerMode = EatFood;

	m_EatenGhosts = 0;
	m_currentframe = 1;
}

void CPlayer::RestartPosition()
{
	m_Sprite.setPosition(m_StartPosition);
}

void CPlayer::Update(sf::RenderWindow & App, sf::Image & ScreenCapture, float deltaTime, bool isWindowActive)
{
	if(gameState == Play)
	{
		if (isWindowActive)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { m_NowyKierunek = LEWO; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { m_NowyKierunek = PRAWO; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { m_NowyKierunek = GORA; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { m_NowyKierunek = DOL; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { if (m_Stamina>0.f) { m_Stamina -= 1.f; m_Speed = 100.f; } else m_Speed = 60.f; }
			else { m_Speed = 60.f; }
		}		

		if(m_regenclock.getElapsedTime().asSeconds() > 0.5f)
		{
			if(m_Stamina < 100.f)
				m_Stamina+=1.f;
			m_regenclock.restart();
		}
		if(m_Stamina > 100.f) m_Stamina = 100.f;
		if(m_Stamina < 0.f) m_Stamina = 0.f;

		Go(ScreenCapture, deltaTime);

		CanTurn(ScreenCapture);
	}
	App.draw(m_Sprite);

}

sf::Sprite & CPlayer::GetSprite()
{
	return m_Sprite;
}

sf::Vector2f CPlayer::GetPlayerPosition()
{
	return m_Sprite.getPosition();
}

void CPlayer::Go(sf::Image& ScreenCapture, float & deltaTime)
{
	sf::Color p1,p2,p3;
	sf::Color sciana = sf::Color::Red;
	bool wynik = false;

	switch(m_kierunek)
	{
	case LEWO:
		{
			p1 = ScreenCapture.getPixel(m_Sprite.getPosition().x-14, m_Sprite.getPosition().y+13);
			p2 = ScreenCapture.getPixel(m_Sprite.getPosition().x-14, m_Sprite.getPosition().y);
			p3 = ScreenCapture.getPixel(m_Sprite.getPosition().x-14, m_Sprite.getPosition().y-13);
			if( p1==sciana || p2==sciana || p3==sciana)
			{
				wynik = false;
			}
			else
			{
				wynik = true;
				//m_Sprite.setRotation(0);
				sf::Vector2f newPosition =  m_Direction * m_Speed * deltaTime;
				if(m_Sprite.getPosition().x > -15.f)
				{
					if(m_frameclock.getElapsedTime().asSeconds() > 0.1f)
					{
						m_currentframe++;
						if(m_currentframe > 3)
							m_currentframe = 0;

						sf::IntRect rect(25*m_currentframe,0, 25,25);

						m_Sprite.setTextureRect(rect);
						m_frameclock.restart();
					}
					m_Sprite.move(newPosition);
					sendPositionChange(m_Sprite.getPosition(), kierunek::LEWO);
				}
				else
					m_Sprite.setPosition(420.f,m_Sprite.getPosition().y);
			}
			break;
		}

	case PRAWO:
		{
			p1 = ScreenCapture.getPixel(m_Sprite.getPosition().x+14, m_Sprite.getPosition().y+13);
			p2 = ScreenCapture.getPixel(m_Sprite.getPosition().x+14, m_Sprite.getPosition().y);
			p3 = ScreenCapture.getPixel(m_Sprite.getPosition().x+14, m_Sprite.getPosition().y-13);
			if( p1==sciana || p2==sciana || p3==sciana)
			{
				wynik = false;
			}
			else
			{
				wynik = true;
				//m_Sprite.setRotation(180);
				sf::Vector2f newPosition =  m_Direction * m_Speed * deltaTime;

				if(m_Sprite.getPosition().x < 420.f)
				{
					if(m_frameclock.getElapsedTime().asSeconds() > 0.1f)
					{
						m_currentframe++;
						if(m_currentframe > 3)
							m_currentframe = 0;

						sf::IntRect rect(200+25*m_currentframe,0, 25,25);

						m_Sprite.setTextureRect(rect);
						m_frameclock.restart();
					}
					m_Sprite.move(newPosition);
					sendPositionChange(m_Sprite.getPosition(), kierunek::PRAWO);
				}
				else
					m_Sprite.setPosition(-15.f,m_Sprite.getPosition().y);
			}
			break;
		}

	case GORA:
		{
			p1 = ScreenCapture.getPixel(m_Sprite.getPosition().x+13, m_Sprite.getPosition().y-14);
			p2 = ScreenCapture.getPixel(m_Sprite.getPosition().x, m_Sprite.getPosition().y-14);
			p3 = ScreenCapture.getPixel(m_Sprite.getPosition().x-13, m_Sprite.getPosition().y-14);
			if( p1==sciana || p2==sciana || p3==sciana)
			{
				wynik = false;
			}
			else
			{
				wynik = true;
				//m_Sprite.setRotation(90);
				sf::Vector2f newPosition =  m_Direction * m_Speed * deltaTime;

				if(m_frameclock.getElapsedTime().asSeconds() > 0.1f)
				{
					m_currentframe++;
					if(m_currentframe > 3)
						m_currentframe = 0;

					sf::IntRect rect(100+25*m_currentframe,0, 25,25);

					m_Sprite.setTextureRect(rect);
					m_frameclock.restart();
				}

				if (m_Sprite.getPosition().x > 15.f || m_Sprite.getPosition().y > 15.f)
				{
					m_Sprite.move(newPosition);
					sendPositionChange(m_Sprite.getPosition(), kierunek::GORA);
				}
			}
			break;
		}

	case DOL:
		{
			p1 = ScreenCapture.getPixel(m_Sprite.getPosition().x+13, m_Sprite.getPosition().y+14);
			p2 = ScreenCapture.getPixel(m_Sprite.getPosition().x, m_Sprite.getPosition().y+14);
			p3 = ScreenCapture.getPixel(m_Sprite.getPosition().x-13, m_Sprite.getPosition().y+14);
			if( p1==sciana || p2==sciana || p3==sciana)
			{
				wynik = false;
			}
			else
			{
				wynik = true;
				//m_Sprite.setRotation(-90);
				sf::Vector2f newPosition =  m_Direction * m_Speed * deltaTime;

				if(m_frameclock.getElapsedTime().asSeconds() > 0.1f)
				{
					m_currentframe++;
					if(m_currentframe > 3)
						m_currentframe = 0;

					sf::IntRect rect(300+25*m_currentframe,0, 25,25);

					m_Sprite.setTextureRect(rect);
					m_frameclock.restart();
				}

				if(m_Sprite.getPosition().x > 15.f || m_Sprite.getPosition().y > 15.f)
				{
					m_Sprite.move(newPosition);
					sendPositionChange(m_Sprite.getPosition(), kierunek::DOL);
				}
			}
			break;
		}
	}
	m_CanGo = wynik;
}

void CPlayer::sendPositionChange(sf::Vector2f newPos, int newDir)
{
	gClient.pakietPos["pos"]["x"] = newPos.x;
	gClient.pakietPos["pos"]["y"] = newPos.y;

	gClient.pakietPos["kierunek"] = newDir;

	gClient.pakietPos["direction"]["x"] = m_Direction.x;
	gClient.pakietPos["direction"]["y"] = m_Direction.y;


}

bool CPlayer::CanTurn(sf::Image& ScreenCapture)
{
	sf::Color p1,p2,p3;
	sf::Color sciana = sf::Color::Red;

	switch(m_NowyKierunek)
	{
	case LEWO:
		{
			p1 = ScreenCapture.getPixel(m_Sprite.getPosition().x-16, m_Sprite.getPosition().y+13);
			p2 = ScreenCapture.getPixel(m_Sprite.getPosition().x-16, m_Sprite.getPosition().y);
			p3 = ScreenCapture.getPixel(m_Sprite.getPosition().x-16, m_Sprite.getPosition().y-13);
			if( p1==sciana || p2==sciana || p3==sciana)
			{
				return false;
			}
			else
			{
				m_kierunek = LEWO;
				m_Direction = sf::Vector2f(-1, 0);
				return true;
			}
			break;
		}

	case PRAWO:
		{
			p1 = ScreenCapture.getPixel(m_Sprite.getPosition().x+15, m_Sprite.getPosition().y+13);
			p2 = ScreenCapture.getPixel(m_Sprite.getPosition().x+15, m_Sprite.getPosition().y);
			p3 = ScreenCapture.getPixel(m_Sprite.getPosition().x+15, m_Sprite.getPosition().y-13);
			if( p1==sciana || p2==sciana || p3==sciana)
			{
				return false;
			}
			else
			{
				m_kierunek = PRAWO;
				m_Direction = sf::Vector2f(1, 0);
				return true;
			}
			break;
		}

	case GORA:
		{
			p1 = ScreenCapture.getPixel(m_Sprite.getPosition().x+13, m_Sprite.getPosition().y-16);
			p2 = ScreenCapture.getPixel(m_Sprite.getPosition().x, m_Sprite.getPosition().y-16);
			p3 = ScreenCapture.getPixel(m_Sprite.getPosition().x-13, m_Sprite.getPosition().y-16);
			if( p1==sciana || p2==sciana || p3==sciana)
			{
				return false;
			}
			else
			{
				m_kierunek = GORA;
				m_Direction = sf::Vector2f(0, -1);
				return true;
			}
			break;
		}

	case DOL:
		{
			p1 = ScreenCapture.getPixel(m_Sprite.getPosition().x+13, m_Sprite.getPosition().y+16);
			p2 = ScreenCapture.getPixel(m_Sprite.getPosition().x, m_Sprite.getPosition().y+16);
			p3 = ScreenCapture.getPixel(m_Sprite.getPosition().x-13, m_Sprite.getPosition().y+16);
			if( p1==sciana || p2==sciana || p3==sciana)
			{
				return false;
			}
			else
			{
				m_kierunek = DOL;
				m_Direction = sf::Vector2f(0, 1);
				return true;
			}
			break;
		}
	}
	return false;
}

float & CPlayer::GetStamina()
{
	return m_Stamina;
}

void CPlayer::IncStamina(float value)
{
	m_Stamina+=value;
}

void CPlayer::ActiveFrenzy()
{
	m_Speed = 100.f;
	playerMode = EatAll;
	m_EatenGhosts = 0;
}

void CPlayer::DeactivateFrenzy()
{
	m_Speed = 60.f;
	playerMode = EatFood;
}

bool CPlayer::IsFrenzy()
{
	if(playerMode == EatAll || playerMode == EatAllEnding)
		return true;
	else
		return false;
}

unsigned int & CPlayer::GetEatenGhosts()
{
	return m_EatenGhosts;
}

void CPlayer::SetStartPosition(sf::Vector2f & Position)
{
	m_StartPosition = sf::Vector2f(Position.x, Position.y+1);
}