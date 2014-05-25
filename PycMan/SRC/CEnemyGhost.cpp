#include "headers.h"

sf::Vector2f BlinkyPosition, ClydePosition, InkeyPosition, PinkyPosition;

CEnemyGhost::CEnemyGhost()
{

}

CEnemyGhost::CEnemyGhost(const std::string filename, sf::Vector2f StartPosition)
{
	m_Texture.loadFromImage(gResMng.Get_Image(filename));
	m_Texture2.loadFromImage(gResMng.Get_Image("DATA/Fear.bmp"));
	m_Sprite.setTexture(m_Texture);
	sf::IntRect rect(0,0, 25,25);
	m_Sprite.setTextureRect(rect);
	m_Sprite.setOrigin(25.f/2.f, 25.f/2.f);
	m_Sprite.setPosition(StartPosition.x+1,StartPosition.y+1);

	m_Speed = 60.f;
	m_Direction = sf::Vector2f(-1, 0);
	m_kierunek = LEWO;

	m_StartPosition = sf::Vector2f(StartPosition.x+1,StartPosition.y+1);

	m_currentframe = 1;
	m_changeset = 0;
}

void CEnemyGhost::Update(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime, bool _randDirection)
{
	if(gameState == Play)
	{
		if( (m_clock.getElapsedTime().asSeconds() > 2.f || !m_CanGo) && _randDirection)
		{
			ChooseDirection();
			m_clock.restart();
		}

		if(playerMode == EatAll || playerMode == EatAllEnding)
		{
			m_Speed = 30.f;
			m_Sprite.setTexture(m_Texture2);

			if(playerMode == EatAllEnding)
			{
				if(m_changeclock.getElapsedTime().asSeconds() > 0.1f)
				{
					if(m_changeset == 100)
						m_changeset = 0;
					else
						m_changeset = 100;
					m_changeclock.restart();
				}
				
			}
			else
			{
				m_changeset = 0;
			}

			if(m_frameclock.getElapsedTime().asSeconds() > 0.1f && playerMode == EatAll || playerMode == EatAllEnding)
			{
				m_currentframe++;
				if(m_currentframe > 3)
					m_currentframe = 0;

				sf::IntRect rect(25*m_currentframe+m_changeset,0, 25,25);

				m_Sprite.setTextureRect(rect);
				m_frameclock.restart();
			}
			
		}
		else
		{
			m_Speed = 60.f;
			m_Sprite.setTexture(m_Texture);
		}

		Go(ScreenCapture, deltaTime);	

		CanTurn(ScreenCapture);
	}

	App.draw(m_Sprite);
}

void CEnemyGhost::Go(sf::Image& ScreenCapture, float & deltaTime)
{
	sf::Color p1,p2,p3;
	sf::Color sciana = sf::Color::Red;
	bool wynik = false;

	if(m_Sprite.getPosition().y <= 5.f)
		ResetPosition();

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
				if(m_Sprite.getPosition().x > -10.f && m_Sprite.getPosition().y > 5.f)
				{
					if(m_frameclock.getElapsedTime().asSeconds() > 0.1f && playerMode != EatAll && playerMode != EatAllEnding )
					{
						m_currentframe++;
						if(m_currentframe > 3)
							m_currentframe = 0;

						sf::IntRect rect(25*m_currentframe,0, 25,25);

						m_Sprite.setTextureRect(rect);
						m_frameclock.restart();
					}

					m_Sprite.move(newPosition);
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
				if(m_Sprite.getPosition().x < 420.f && m_Sprite.getPosition().y > 5.f)
				{
					if(m_frameclock.getElapsedTime().asSeconds() > 0.1f && playerMode != EatAll && playerMode != EatAllEnding )
					{
						m_currentframe++;
						if(m_currentframe > 3)
							m_currentframe = 0;

						sf::IntRect rect(200+25*m_currentframe,0, 25,25);

						m_Sprite.setTextureRect(rect);
						m_frameclock.restart();
					}

					m_Sprite.move(newPosition);
				}
				else
					m_Sprite.setPosition(-10.f,m_Sprite.getPosition().y);
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

				if(m_frameclock.getElapsedTime().asSeconds() > 0.1f && playerMode != EatAll && playerMode != EatAllEnding )
					{
						m_currentframe++;
						if(m_currentframe > 3)
							m_currentframe = 0;

						sf::IntRect rect(100+25*m_currentframe,0, 25,25);

						m_Sprite.setTextureRect(rect);
						m_frameclock.restart();
					}

				m_Sprite.move(newPosition);
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

				if(m_frameclock.getElapsedTime().asSeconds() > 0.1f && playerMode != EatAll && playerMode != EatAllEnding )
				{
					m_currentframe++;
					if(m_currentframe > 3)
						m_currentframe = 0;

					sf::IntRect rect(300+25*m_currentframe,0, 25,25);

					m_Sprite.setTextureRect(rect);
					m_frameclock.restart();
				}

				m_Sprite.move(newPosition);
			}
			break;
		}
	}
	m_CanGo = wynik;
}

bool CEnemyGhost::CanTurn(sf::Image& ScreenCapture)
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

void CEnemyGhost::ChooseDirection()
{
	kierunek nowy;
	unsigned int gdzie = 0;

	
	nowy = static_cast<kierunek>(rand()%4);
	

	
	m_NowyKierunek = nowy;

}

sf::Vector2f CEnemyGhost::getPosition() const
{
	return m_Sprite.getPosition();
}

sf::Vector2f CEnemyGhost::getDirection() const
{
	return m_Direction;
}

int CEnemyGhost::getKierunek() const
{
	return m_kierunek;
}

void CEnemyGhost::setRemotePosition(sf::Vector2f pos, sf::Vector2f dir, kierunek kier)
{
	m_Sprite.setPosition(pos);
	m_Direction = dir;
	m_kierunek = kier;
}

sf::Sprite & CEnemyGhost::GetSprite()
{
	return m_Sprite;
}

void CEnemyGhost::ResetPosition()
{
	m_Sprite.setPosition(m_StartPosition);
}

void CEnemyGhost::SetStartPosition(sf::Vector2f & Position)
{
	m_StartPosition = sf::Vector2f(Position.x+1,Position.y+1);
}