#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>

class Selector
{
public:
	Selector(IDirect3DDevice9 *d3dDev);
	~Selector(void);
	int init(IDirect3DDevice9 *d3dDev, char * spriteName, char *spriteName2);


private:
	IDirect3DTexture9 *arrowSprite, *arrowSprite2;
	long previousTime;
	bool latch;
public:
	static int setd3dDev(IDirect3DDevice9 * d3dDev, ID3DXSprite *d3dSprite);
						 
private:
	
public:
	int render(ID3DXSprite *d3dSprite, int choose, long time, int windowWidth, int windowHeight);
};
