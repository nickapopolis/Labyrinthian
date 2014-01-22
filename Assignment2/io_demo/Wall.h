#pragma once
#include "stdafx.h"
#include "MeshBox.h"
class Wall :
	public MeshBox
{
public:
	Wall(IDirect3DDevice9 *d3dDev, NxScene *scene,  Vector &position, float size);
	~Wall(void);
	static int initializeTexture(IDirect3DDevice9 *d3dDev);

protected:
	static IDirect3DTexture9* wallTex;
};

