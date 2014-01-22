/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "Wall.h"

//initialize static wall texture
IDirect3DTexture9* Wall::wallTex = 0;

//Main constructor for wall
Wall::Wall(IDirect3DDevice9 *d3dDev, NxScene *scene,  Vector &position, float size) :
MeshBox(d3dDev, scene, wallTex, position, Vector(0.0f, 0.0f, 0.0f), size, size ,size)
{
	actor->raiseBodyFlag(NX_BF_FROZEN);
	actor->setGroup(TERRAIN);
}

Wall::~Wall(void)
{

}

//static texture initialization
int  Wall::initializeTexture(IDirect3DDevice9 *d3dDev)
{
	return D3DXCreateTextureFromFile(d3dDev,TEXT("res\\stonewall.png"), &wallTex);
}