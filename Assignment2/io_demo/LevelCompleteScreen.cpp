/*
Author: Tyler Haskell
*/
#include "StdAfx.h"
#include "LevelCompleteScreen.h"
#include "nuss_util.h"


LevelCompleteScreen::LevelCompleteScreen(IDirect3DDevice9 *d3dDev, char* name, char* items, int size, LPD3DXFONT &font, int windowWidth, int windowHeight, char* backName) : GameMenu(d3dDev, name, items, size, font, windowWidth, windowHeight, backName)
{
	playTime = 0.0f;
	deaths = 0;
}


LevelCompleteScreen::~LevelCompleteScreen(void)
{
}

void LevelCompleteScreen::render(ID3DXSprite *d3dSprite, long gameTime, int windowWidth, int windowHeight)
{
	D3DXVECTOR3 centre;
	D3DXVECTOR3 position, titlePos;
	centre = D3DXVECTOR3(0.0,0.0,0.0);
	position = D3DXVECTOR3(0.0, 0.0, 0.0);
	titlePos = D3DXVECTOR3((float)windowWidth/2 - 200, 50.0, 0.0);
	RECT resultsBox;
	LONG x, y;
	x = windowWidth / 2;
	y = windowHeight / 2;
	resultsBox.top = y + 100;
	resultsBox.left = x - 130;
	resultsBox.bottom = y+200;
	resultsBox.right = x+300;	
	D3DCOLOR colour = D3DCOLOR_RGBA(255,255,255,255);
	float scaleX = (float)windowWidth / 512.0f;
	float scaleY = (float)windowHeight / 512.0f;

	char t[50];
	sprintf_s(t,"Time: %.2f\nDeaths: %d", playTime, deaths);

	d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR2 scaling(scaleX, scaleY);
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0.0,NULL);
	d3dSprite->SetTransform(&mat);
	d3dSprite->Draw(backgroundImage, NULL, &centre, &position, colour);

	D3DXVECTOR2 scaling2(1.0f, 1.0f);
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling2,NULL,0.0,NULL);
	d3dSprite->SetTransform(&mat);
	d3dSprite->Draw(titleImage, NULL, &centre, &titlePos, colour);

	d3dSprite->End();
	(*menuFont)->DrawText(NULL, menuItems, -1, &bodyBox, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 0));
	(*menuFont)->DrawText(NULL, t, -1, &resultsBox, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 0));
	selector->render(d3dSprite, selected, gameTime, windowWidth, windowHeight); 
}

void LevelCompleteScreen::setPlayTime(float t)
{
	playTime = t;
}

void LevelCompleteScreen::setDeaths(int d)
{
	deaths = d;
}

