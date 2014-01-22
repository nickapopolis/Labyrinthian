#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>

class Hud
{
public:
	Hud(IDirect3DDevice9 *d3dDev, int windowWidth, int windowHeight);
	~Hud(void);
	int init(IDirect3DDevice9 *d3dDev, char * barSprite);
	int render(ID3DXSprite *d3dSprite, long time);
	void resetTime(void);
	void setLevelNum(int n);
	void setBestTime(float f);
	int getLevelNum(void);
	float getTimeSpent(void);
	int getLives(void);
	void setLives(int l);

private:
	IDirect3DTexture9 *bar;
	float timeSpent;
	int winWidth;
	int winHeight;
	int levelNum;
	int lives;
	float best;
	LPD3DXFONT font;
	RECT timeBox;
	RECT levelBox;
	RECT bestBox;
	RECT livesBox;
};

