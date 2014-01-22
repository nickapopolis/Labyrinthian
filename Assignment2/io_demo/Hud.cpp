/*
Author: Tyler Haskell
*/
#include "StdAfx.h"
#include "Hud.h"
#include "nuss_util.h"


Hud::Hud(IDirect3DDevice9 *d3dDev, int windowWidth, int windowHeight)
{
	bar = NULL;
	font = NULL;
	winWidth = windowWidth;
	winHeight = windowHeight;
	timeSpent = 0.0f;
	levelNum = 1;
	lives = 3;
	best = 0.0f;
	init(d3dDev, "res\\hudBar.png");
}


Hud::~Hud(void)
{
	SAFE_RELEASE(bar);
	SAFE_RELEASE(font);
}

int Hud::init(IDirect3DDevice9 *d3dDev, char * barSprite)
{
	int rc = 0;

	rc = D3DXCreateTextureFromFileEx(d3dDev, 
		barSprite, // file name
		0,			// width
		0,			// height
		D3DX_DEFAULT, // MIPS levels
		0,
		D3DFMT_UNKNOWN,		// format of pixels (take from file)
		//D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT, 
		D3DCOLOR_RGBA(255,255,0, 255),	// transparaent colour
		NULL,
		NULL,
		&bar);

	rc = D3DXCreateFont(d3dDev,
					40, // height of font
					0,	// use the default width
					FW_BOLD,
					//FW_MEDIUM+50,
					//FW_NORMAL,	// normal font weight
					1, // no Mipmap
					FALSE, // italic
					DEFAULT_CHARSET, // default character set
					OUT_DEFAULT_PRECIS, // default precision
					DEFAULT_QUALITY, // default quality
					DEFAULT_PITCH ||FF_DONTCARE, // more defaults...
					"Courier",	// typeface “Courier"
					&font);

	timeBox.top = winHeight - 95;
	timeBox.left = 10;
	timeBox.bottom = winHeight - 47;
	timeBox.right = winWidth/2;

	levelBox.top = winHeight - 95;
	levelBox.left = winWidth - 230;
	levelBox.bottom = winHeight - 47;
	levelBox.right = winWidth;

	bestBox.top = winHeight - 47;
	bestBox.left = 10;
	bestBox.bottom = winHeight;
	bestBox.right = winWidth/2;

	livesBox.top = winHeight - 47;
	livesBox.left = winWidth - 230;
	livesBox.bottom = winHeight;
	livesBox.right = winWidth;

	switch(rc) {
		case D3DERR_NOTAVAILABLE:
			rc = 1;
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			rc = 2;
			break;
		case D3DERR_INVALIDCALL:
			rc = 3;
			break;
		case D3DXERR_INVALIDDATA:
			rc = 4;
			break;
		default:
			break;
	}
	if (rc != D3D_OK) {
		rc = 1;
		bar = NULL;
	} else {
		rc = 0;
	}

	return rc;
}

int Hud::render(ID3DXSprite *d3dSprite, long time)
{
	D3DXVECTOR3 centre;
	D3DXVECTOR3 barPosition;
	D3DCOLOR colour = D3DCOLOR_RGBA(255,255,255,255);
	centre = D3DXVECTOR3(0.0, 0.0, 0.0);
	barPosition = D3DXVECTOR3(0.0, winHeight - 95.0, 0.0);
	float scaleX = (float)winWidth / 510.0f;
	float scaleY = (float)winHeight / 80.0f;

	d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR2 scaling(scaleX,1.0f);
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0.0,NULL);
	d3dSprite->SetTransform(&mat);
	d3dSprite->Draw(bar, NULL, &centre, &barPosition, colour);

	d3dSprite->End();

	char ct[50];
	char bt[50];
	char level[50];
	char live[50];
	timeSpent = timeSpent + 1.0 / 30.0;
	sprintf_s(ct,"Time: %.2f", timeSpent);
	sprintf_s(bt,"Best: %.2f", best);
	sprintf_s(level,"Level: %d", levelNum);
	sprintf_s(live,"Lives: %d", lives);
	font->DrawText(NULL, ct, -1, &timeBox, DT_LEFT, D3DCOLOR_ARGB(255, 255, 0, 0));
	font->DrawText(NULL, level, -1, &levelBox, DT_LEFT, D3DCOLOR_ARGB(255, 255, 0, 0));
	font->DrawText(NULL, bt, -1, &bestBox, DT_LEFT, D3DCOLOR_ARGB(255, 255, 0, 0));
	font->DrawText(NULL, live, -1, &livesBox, DT_LEFT, D3DCOLOR_ARGB(255, 255, 0, 0));

	return 0;
}

void Hud::resetTime(void)
{
	timeSpent = 0.0f;
}

void Hud::setLevelNum(int n)
{
	levelNum = n;
}

void Hud::setBestTime(float f)
{
	best = f;
}

int Hud::getLevelNum(void)
{
	return(levelNum);
}

float Hud::getTimeSpent(void)
{
	return(timeSpent);
}

int Hud::getLives(void)
{
	return(lives);
}

void Hud::setLives(int l)
{
	lives = l;
}


