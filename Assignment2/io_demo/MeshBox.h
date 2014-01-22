#pragma once
#include "stdafx.h"
#include "PhysicsEngine.h"
#include "MeshObject.h"

class MeshBox : public MeshObject
{
public:
	MeshBox(IDirect3DDevice9 *d3dDev, NxScene *scene,IDirect3DTexture9* tex, Vector &position, Vector &velocity, float width, float height, float depth);
	~MeshBox(void);

	LPD3DXMESH createMappedBox(IDirect3DDevice9 *d3dDev,float width, float height,float depth);
};

