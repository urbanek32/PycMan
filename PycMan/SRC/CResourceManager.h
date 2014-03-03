#include "headers.h"

#ifndef CRESNAGER_H
#define CRESNAGER_H
class CResourceManager
{
public:
	CResourceManager();

	// zwraca sf::Image o podanej nazwie jezeli istnieje, jak nie to zwraca pusty obraz
	const sf::Image & Get_Image(const std::string & filename);

	//	load images to memory via filename
	bool Load_Image(const std::string & filename);


	// remove image from memory via filename
	void Delete_Image(const std::string & filename);

	// return size of resource container
	int Get_MapSize();

	
private:
	std::map<std::string , sf::Image > images_;

};
// global var for access to ResourceManager
extern CResourceManager gResMng;
#endif