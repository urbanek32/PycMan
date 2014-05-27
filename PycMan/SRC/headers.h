#pragma warning(disable: 4244)
#ifndef HEADERS_H
#define HEADERS_H
// Biblioteki
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <json/json.h>

// Standartowe
using namespace std;
#include <iostream>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>
#include <cmath>
#include <functional>
#include <Windows.h>
#include <assert.h>

// Sieciowe
#include <UDP_Socket.h>
#include <IPAddress.h>

// Moje w³asne
#include "TSingleton.h"
#include "CScreen.h"
#include "CGameManager.h"
#include "CResourceManager.h"
#include "CGame.h"
#include "CMapManager.h"
#include "CPlayer.h"
#include "CEnemyGhost.h"
#include "CMainMenu.h"
#include "CHighScoresMenu.h"
#include "CLobby.h"
#include "CClient.h"
#include "COtherPlayer.h"
#endif