/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "FloorPanel.h"

//init static floor texture
IDirect3DTexture9* FloorPanel::floorTex = 0;

//main constructor
FloorPanel::FloorPanel(IDirect3DDevice9 *d3dDev, NxScene *scene,  Vector &position, float size): 
MeshBox(d3dDev, scene, floorTex, position, Vector(0.0f, 0.0f, 0.0f), size, size * 0.001f ,size)
{
	actor->raiseBodyFlag(NX_BF_KINEMATIC);
	actor->setGroup(FLOOR);
}


FloorPanel::~FloorPanel(void)
{
}

int FloorPanel::initializeTexture(IDirect3DDevice9 *d3dDev)
{
	return D3DXCreateTextureFromFile(d3dDev,TEXT("res\\concrete.jpg"), &floorTex);
}
void FloorPanel::playerSteppedOnFloor()
{
	//pass in value to shader in order to highlight the floor
}
