#include "headers.h"

#ifdef CPP_0x11
CMapManager* ptr=nullptr;
#else
CMapManager* ptr=NULL;
#endif

unsigned int FoodCount;

CMapManager* CMapManager::GetInstance()
{
	if(ptr==NULL) ptr = new CMapManager();
	return ptr;
}

CMapManager::CMapManager()
{
	FoodCount = 0;
}

void CMapManager::DrawMap(sf::RenderWindow & App)
{
	for(unsigned int i=0; i< m_Walls.size(); i++)
	{
		App.draw( m_Walls[i] );
	}
	for(unsigned int i=0; i< m_Foods.size(); i++)
	{
		App.draw( m_Foods[i].shape );
	}
	
}

void CMapManager::LoadMap(const string filepath)
{
	m_vmap.clear();
	m_Foods.clear();
	m_Walls.clear();

	ifstream plik(filepath.c_str());
	string _tmp;

	if( plik.is_open() )
	{				
		while( !plik.eof() )
		{
			getline(plik, _tmp);
			cout<<_tmp<<endl;
			m_vmap.push_back(_tmp);
		}
	}
	else
	{
		cout<<"Program z plikiem z mapa\n";
	}

	for(unsigned int i=0; i< m_vmap.size(); i++)
	{
		for(unsigned int j=0; j< m_vmap[i].length(); j++)
		{
			center.x = 15.f * (j) +7.f;
			center.y = 15.f * (i) +7.f;
			sf::RectangleShape kafel;
			Food food;

			switch( m_vmap[i][j] )
			{
				case '.':
				{
					//kafel = sf::Shape::Line(center.x - 1, center.y, center.x +2, center.y, 3.f, sf::Color(255,255,0,255));
					kafel.setSize(sf::Vector2f(3,3));
					kafel.setPosition(center.x-1, center.y);
					kafel.setFillColor(sf::Color::Cyan);
					food.shape = kafel;
					food.type = 1; // zwyk³e ¿arcie
					m_Foods.push_back(food);
					break;
				}
				case ':':
				{
					//kafel = sf::Shape::Line(center.x - 1, center.y, center.x +2, center.y, 3.f, sf::Color(255,255,0,255));
					kafel.setSize(sf::Vector2f(8,8));
					kafel.setPosition(center.x-1, center.y);
					kafel.setFillColor(sf::Color::Cyan);
					food.shape = kafel;
					food.type = 2; // dopalacz
					m_Foods.push_back(food);
					break;
				}
				case '-':
				{
					//kafel = sf::Shape::Line(center.x - 7, center.y, center.x +8, center.y, 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(15,3));
					kafel.setPosition(center.x-7, center.y);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);

					if( j>0 && m_vmap[i][j-1] == '_')
					{
						//kafel = sf::Shape::Line(center.x -7, center.y, center.x , center.y, 3.f, sf::Color(255,0,0,255));
						kafel.setSize(sf::Vector2f(7,3));
						kafel.setPosition(center.x-7, center.y);
						kafel.setFillColor(sf::Color::Red);
						m_Walls.push_back(kafel);
					
					}
					break;
				}
				case '|':
				{
					//kafel = sf::Shape::Line(center.x, center.y -7, center.x , center.y +8 , 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(3,15));
					kafel.setPosition(center.x, center.y-7);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);
					break;
				}
				case '1':
				{
					//kafel = sf::Shape::Line(center.x, center.y-1, center.x , center.y+8 , 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(3,9));
					kafel.setPosition(center.x, center.y);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);

					//kafel = sf::Shape::Line(center.x-1, center.y, center.x+8 , center.y, 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(9,3));
					kafel.setPosition(center.x, center.y);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);
					break;
				}
				case '2':
				{
					//kafel = sf::Shape::Line(center.x, center.y-8, center.x , center.y+2, 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(3,9));
					kafel.setPosition(center.x, center.y-8);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);

					//kafel = sf::Shape::Line(center.x-1, center.y, center.x+8 , center.y, 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(9,3));
					kafel.setPosition(center.x, center.y);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);
					break;
				}
				case '4':
				{
				
					//kafel = sf::Shape::Line(center.x, center.y-1, center.x, center.y+8, 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(3,9));
					kafel.setPosition(center.x, center.y);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);
				
					//kafel = sf::Shape::Line(center.x -8, center.y, center.x+1, center.y , 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(9,3));
					kafel.setPosition(center.x-7, center.y);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);
					break;
				}
				case '3':
				{
				
					//kafel = sf::Shape::Line(center.x, center.y-8, center.x, center.y+1 , 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(3,9));
					kafel.setPosition(center.x, center.y-7);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);
				
					//kafel = sf::Shape::Line(center.x -8, center.y, center.x+1, center.y , 3.f, sf::Color(255,0,0,255));
					kafel.setSize(sf::Vector2f(9,3));
					kafel.setPosition(center.x-7, center.y);
					kafel.setFillColor(sf::Color::Red);
					m_Walls.push_back(kafel);
					break;
				}
				case '#':
				{
				//DrawLine3(labirynth, center.x - 15, center.y, 30, horizontal, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));
					//kafel = sf::Shape::Line(center.x-15, center.y, center.x +15, center.y , 3.f, sf::Color(255,255,255,255));
					kafel.setSize(sf::Vector2f(30,3));
					kafel.setPosition(center.x-15, center.y);
					kafel.setFillColor(sf::Color::White);
					m_Walls.push_back(kafel);
					break;
				}
				case 'P':
				{
					PlayerPosition.x = center.x;
					PlayerPosition.y = center.y;
					break;
				}
				case 'B':
				{
					BlinkyPosition.x = center.x;
					BlinkyPosition.y = center.y;
					break;
				}
				case 'I':
				{
					InkeyPosition.x = center.x;
					InkeyPosition.y = center.y;
					break;
				}
				case 'N':
				{
					PinkyPosition.x = center.x;
					PinkyPosition.y = center.y;
					break;
				}
				case 'C':
				{
					ClydePosition.x = center.x;
					ClydePosition.y = center.y;
					break;
				}
	
			}// end switch
			
		}// end for
		
	}// end for
	FoodCount = (unsigned int)(m_Foods.size());
}

deque<sf::RectangleShape> & CMapManager::GetWallShapes()
{
	return m_Walls;
}

deque<Food> & CMapManager::GetFoodShapes()
{
	return m_Foods;
}

bool CMapManager::IsCollision(sf::FloatRect & PlayerRect)
{
	for(deque<sf::RectangleShape>::iterator it = m_Walls.begin(); it != m_Walls.end(); it++)
	{
		sf::FloatRect rect = it->getGlobalBounds();
		if( rect.intersects(PlayerRect) )
		{
			return true;
		}
		
	}
	return false;
}