#ifndef CGAME_H
#define CGAME_H

#include "headers.h"


enum GameState{
	Pauza, Quit, Play, GameOver, Prepare
};

extern GameState gameState;


class CGame : public CScreen
{
public: 
	CGame();

	virtual int Run(sf::RenderWindow & App);

private:
	bool m_Running;

	bool m_Inited;
	void m_Init();

	bool m_Captured;
	void m_CaptureScreen(sf::RenderWindow & App);
	
	

	//uaktualnia pozycje wzglêdem otrzymanego pakietu
	void changePositions();

	sf::Event m_Event;

	class CMapManager *m_MapMng;
	
	class CPlayer *m_Player;


	sf::Text m_FPS, m_TScore, m_TStamina, m_TAdrealina, m_TMana, m_TLives, m_TPause, m_TGameOver, m_TQuit, m_TCount;
	sf::Font m_font;

	float m_Score, m_Stamina, m_Adrealina, m_Mana, m_Lives;

	sf::Image m_ScreenCapture;

	bool m_done;
	int m_count;

	sf::Clock m_clock, m_fpsclock, m_frenzyclock, m_startclock;
	float m_lastTime, m_currentTime;

	void UpdateEnemies(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime);

	void UpdateOtherPlayers(sf::RenderWindow & App, sf::Image& ScreenCapture, float & deltaTime);

	vector<class CEnemyGhost> m_Enemies;
	vector<class COtherPlayer> m_OtherPlayers;

	void CheckCollision(CMapManager *maper, CPlayer *player);

	void RestartPositions();

	void OtherUpdates(sf::RenderWindow & App);

	void RestartGame(bool ResetScore = false);

	void ShowGameOver(sf::RenderWindow & App);
	void ShowQuit(sf::RenderWindow & App);
	sf::RectangleShape m_overShape, m_overtp;

	unsigned int m_LevelNumer;

	void m_AddScore();
	bool m_ScoreAdded;

	void LoadNextMap();

	void PrepareGame(sf::RenderWindow & App);

	int updateMultiplayerStuff();

	// czy okno z gra jest aktywne
	bool m_isActive;
};
#endif