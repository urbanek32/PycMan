#include "headers.h"

// global var for access to ResourceManager
CResourceManager gResMng;

CResourceManager::CResourceManager() 
{
	
}

const sf::Image & CResourceManager::Get_Image(const std::string & filename)
{ 
	//	//	checking is image exist in memory
	for(std::map<std::string, sf::Image>::iterator it = images_.begin(); it != images_.end(); it++)
	{
		if(filename == it->first)
			return it->second;
	}

	//	//	adding image to memory if doesnt exist
	sf::Image _image;
/*	if( _image.LoadFromFile(filename) )
	{
		images_[filename] = _image;
		return images_[filename];
	}
*/
	//	//	if not found nothing, use default blank
	images_[filename] = _image;
	return images_[filename];
}

bool CResourceManager::Load_Image(const std::string & filename)
{
	sf::Image _image;
	if( _image.loadFromFile(filename) )
	{
		images_[filename] = _image;
		return true;
	}
	return false;
}

int CResourceManager::Get_MapSize()
{
	return (int)images_.size();
}

void CResourceManager::Delete_Image(const std::string & filename)
{
	std::map<std::string, sf::Image>::iterator it = images_.find(filename);
	if( it != images_.end() )
		images_.erase( it );
}