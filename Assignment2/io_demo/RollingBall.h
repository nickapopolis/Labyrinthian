#pragma once
#include  "MeshSphere.h"
#include "stdafx.h"
#include "SpaceNode.h"

class RollingBall: public MeshSphere
{
public:
	RollingBall(IDirect3DDevice9 *d3dDev, NxScene *scene,  Vector &position, float size, SpaceNode *pathStart, SpaceNode *pathEnd);
	~RollingBall(void);

	static int initializeTexture(IDirect3DDevice9 *d3dDev);
	void updatePosition(long elapsedTime, int offsetX, int offsetY, int offsetZ, float spaceSize);

	float getRotX();
	float getRotZ();
	void drawObject(IDirect3DDevice9 *d3dDev);

protected:
	SpaceNode *startSpace;
	SpaceNode *endSpace;
	SpaceNode *currentSpace;
	static IDirect3DTexture9* rollingBallTex;
	float movementSpeed;
	float rotX;
	float rotZ;
	bool xRot;

};

