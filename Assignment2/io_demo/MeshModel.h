#pragma once
#include "stdafx.h"
#include "MeshObject.h"

class MeshModel :public  MeshObject
{
public:
	MeshModel(IDirect3DDevice9 *d3dDev, NxScene *scene, Vector &position, char* dir, char* meshFileName, float width, float height, float depth, bool collision);
	~MeshModel(void);
	
	void drawObject(IDirect3DDevice9 *d3dDev);

	float scaleX;
	float scaleY;
	float scaleZ;

	Vector getPos();
	float rotX;
	float rotY;
	float rotZ;
	
	D3DMATERIAL9 *defaultMaterial;
protected:
	int createMesh(IDirect3DDevice9 *d3dDev, char *dir, char * meshFileName);

	
	Vector pos;
	LPDIRECT3DTEXTURE9 *m_pMeshTexturesBuf;
	D3DMATERIAL9 *m_pMeshMaterials;
	DWORD  mNumMaterials;

};

