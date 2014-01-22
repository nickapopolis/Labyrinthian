//=============================================================================
// myGame.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates the usage of the gameIO and gameApp framework 
//
//
//
// Description:
//--------------
// A simple application that draws text on the screen.  
// The application draws a text on the screen.  The text shows the text location 
// and the current frame number.  The text scrolls on the screen.
// The text scrolling changes direction when:
// a. the text reaches a boundary of the window
//
// b. the user presses the left mouse button - the left mouse button changes x-direction
// and the right mouse button changes the y-direction
//
// c. The escape key leaves the application
//
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed with a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================

#pragma once
#include "gameapp.h"
#include "Maze.h"
#include "Wall.h"
#include "FloorPanel.h"
#include "Level.h"
#include "Selector.h"
#include "GameMenu.h"
#include "Player.h"
#include "stdafx.h"
#include "MeshSphere.h"
#include "Hud.h"
#include "BestTime.h"
#include "LevelCompleteScreen.h"
#include "irrKlang.h"
class myGame : public gameApp
{
	/***********************************************/
	/*		           Functions		           */
	/***********************************************/
public:
	myGame(HINSTANCE hInstance, char* gameName);
	~myGame(void);
	int updateGameState(long time);
	int renderFrame(long time);
	int initGame(void);
	int initTextures(void);
	int initFonts(void);
	int initMenus(void);
	int initLight(void);
	
	int initAudioBackground(void);
	int initAudioEnter(void);
	int initAudioSelect(void);
	int initAudioWalk(void);

protected:
	void mainMenuChooseOption(int count);
	void levelMenuChooseOption(int count);
	void winMenuChooseOption(int count);
	void loseMenuChooseOption(int count);
	void pauseMenuChooseOption(int count);
	void creditMenuChooseOption(int count);
	void helpMenuChooseOption(int count);
	void switchToLevel(Level *level);
	void updateCameraPosition();
	void endGame();
	/***********************************************/
	/*		           Variables		           */
	/***********************************************/
protected:
	//game objects
	ISound *currentSound;
	ISoundEngine *soundEng;
	Level **levels;
	Level *currentLevel;
	bool paused;
	Player *player;
	D3DLIGHT9 *light;

	GameMenu *mainMenu;
	LevelCompleteScreen *winMenu;
	GameMenu *loseMenu;
	GameMenu *pauseMenu;
	GameMenu *creditMenu;
	GameMenu *helpMenu;
	GameMenu *levelSelectionMenu;
	GameMenu *currentMenu;

	BestTime *bestTimes;
	//Game Hud
	Hud		 *hud;
	
	//fonts
	LPD3DXFONT fontCourier; // font to be used 
	LPD3DXFONT preambleFont;
	char s[1024]; // string to be drawn

	//input
	bool returnPressed;

};
