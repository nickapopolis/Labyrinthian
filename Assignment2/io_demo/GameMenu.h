#pragma once
#include "stdafx.h"
#include "gameIO.h"
#include "Selector.h"
using namespace std;

class GameMenu
{
public:
	GameMenu(IDirect3DDevice9 *d3dDev, char* name, char* items, int size, LPD3DXFONT &font, int windowWidth, int windowHeight, char* backName);
	~GameMenu(void);

	static int initializeTexture(IDirect3DDevice9 *d3dDev);
	virtual void render(ID3DXSprite *d3dSprite, long gameTime, int windowWidth, int windowHeight);
	static void releaseTexture();
	int initImages(IDirect3DDevice9 *d3dDev, char * backgroundName, char * menuName);
	void handleInput(gameIO *mInput);
	int getSelected();
	void setMenuItems(char* m);
	void setMenuSize(int s);
	int getMenuSize(void);


protected:
	char* menuName;
	char* menuItems;
	char* backgroundName;
	LPD3DXFONT *menuFont;
	int selected;
	static bool wpressed;
	static bool spressed;
	static Selector *selector;
	RECT titleBox;
	RECT bodyBox;
	int menuSize;
	IDirect3DTexture9 *backgroundImage;
	IDirect3DTexture9 *titleImage;
};

