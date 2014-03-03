#ifndef CSCREEN_H
#define CSCREEN_H

#include "headers.h"

class CScreen
{
public :
    virtual int Run (sf::RenderWindow & App) = 0;
};
#endif