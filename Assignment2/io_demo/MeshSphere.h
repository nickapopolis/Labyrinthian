#pragma once
#include "stdafx.h"
#include "PhysicsEngine.h"
#include "MeshObject.h"

class MeshSphere : public MeshObject
{
public:
	MeshSphere(IDirect3DDevice9 *d3dDev, NxScene *scene,IDirect3DTexture9* tex, Vector &position, Vector &velocity, float radius);
	~MeshSphere(void);
	LPD3DXMESH createMappedSphere(IDirect3DDevice9 *d3dDev,float radius,UINT slices,UINT stacks);

	
};

