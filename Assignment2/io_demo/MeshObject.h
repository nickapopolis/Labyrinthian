#pragma once
#include "stdafx.h"
#include "PhysicsEngine.h"
#include <sstream>

#define FVF_VERTEX  (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

using namespace std;
class MeshObject
{
public:
		
	struct MESH_VERTEX
	{
		D3DXVECTOR3 pos;     // vertex position
		D3DXVECTOR3 norm;    // vertex normal
		float tu;            // texture coordinates
		float tv;
	};

	MeshObject(IDirect3DDevice9 *d3dDev, NxScene *scene,IDirect3DTexture9* tex, Vector &position, Vector &velocity, float width, float height, float depth);
	~MeshObject(void);

public:
	virtual void drawObject(IDirect3DDevice9 *d3dDev);
	void drawObjectShader(IDirect3DDevice9 *d3dDev);
	float getX();
	float getY();
	float getZ();
	virtual float getRotX();
	virtual float getRotY();
	virtual float getRotZ();

	virtual Vector getPos();
	string toString();

	void release();
	
	NxActor *actor;
protected:
	IDirect3DTexture9* texture;
	LPD3DXMESH mesh;


	//shader effects
	ID3DXEffect *shaderEffect;
	D3DXHANDLE hTime;
	D3DXHANDLE hHighWaves;
	D3DXHANDLE hTechnique;
	D3DXHANDLE hWorldMat;
	D3DXHANDLE hWorldInverseTransposeMat;
	D3DXHANDLE hViewMat;
	D3DXHANDLE hProjMat;
	D3DXHANDLE hTexture;

};

