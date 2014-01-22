#pragma once
#include "GameMenu.h"
#include "stdafx.h"
#include "gameIO.h"
#include "Selector.h"

class LevelCompleteScreen : public GameMenu
{
public:
	LevelCompleteScreen(IDirect3DDevice9 *d3dDev, char* name, char* items, int size, LPD3DXFONT &font, int windowWidth, int windowHeight, char* backName);
	~LevelCompleteScreen(void);
	void LevelCompleteScreen::render(ID3DXSprite *d3dSprite, long gameTime, int windowWidth, int windowHeight);
	void LevelCompleteScreen::setPlayTime(float t);
	void LevelCompleteScreen::setDeaths(int d);

private:
	float playTime;
	int deaths;
};

