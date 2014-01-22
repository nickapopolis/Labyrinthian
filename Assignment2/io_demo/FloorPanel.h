#pragma once
#include "stdafx.h"
#include "MeshBox.h"

class FloorPanel : 
	public MeshBox
{
public:
	FloorPanel(IDirect3DDevice9 *d3dDev, NxScene *scene,  Vector &position, float size);
	~FloorPanel(void);

	static int initializeTexture(IDirect3DDevice9 *d3dDev);
	void playerSteppedOnFloor();
protected:
	static IDirect3DTexture9* floorTex;

};

