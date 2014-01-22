/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "RollingBall.h"

 IDirect3DTexture9* RollingBall::rollingBallTex = NULL;

RollingBall::RollingBall(IDirect3DDevice9 *d3dDev, NxScene *scene,  Vector &position, float size, SpaceNode *pathStart, SpaceNode *pathEnd)
	: MeshSphere(d3dDev, scene, rollingBallTex, position, Vector(0.0f,0.0f,0.0f), size)
{
	startSpace = pathStart;
	endSpace = pathEnd;
	currentSpace = startSpace;
	movementSpeed = 0.05f;
	rotX = 0.0f;
	rotZ = 0.0f;
	xRot = false;
	actor->raiseBodyFlag(NX_BF_KINEMATIC);
	actor->setGroup(TRAP);
}


RollingBall::~RollingBall(void)
{
}

int RollingBall::initializeTexture(IDirect3DDevice9 *d3dDev)
{
	return D3DXCreateTextureFromFile(d3dDev,TEXT("res\\concretewall.jpg"), &rollingBallTex);
}
void RollingBall::updatePosition(long elapsedTime, int offsetX, int offsetY, int offsetZ, float spaceSize)
{
	Vector currentPos = getPos();

	SpaceNode *nextSpace; 

	//cycle through start to end
	if(currentSpace == endSpace)
		nextSpace = startSpace;
	else
		nextSpace = currentSpace->parent;

	Vector goalPos = Vector(nextSpace->getSpace()->getX()* spaceSize+ offsetX* spaceSize, offsetY* spaceSize, nextSpace->getSpace()->getZ()* spaceSize+ offsetZ* spaceSize);

	Vector displacement = goalPos - currentPos;

	Vector nextPos = currentPos;
	if(abs(displacement.x)> abs(displacement.z))
	{
		//reached target
		if(abs(displacement.x) <= abs(movementSpeed))
		{
			currentSpace = nextSpace;
		}
		// distance to move is sign * minimum between the distance to get there and movement speed
		float distanceToMove = displacement.x / abs(displacement.x) * min(abs(displacement.x), abs(movementSpeed));
		currentPos.x += distanceToMove;
		rotZ -= distanceToMove/ 0.5f; //   distance/radius = radians
		xRot = false;
	}
	else
	{
		//reached target
		if(abs(displacement.z) <= abs(movementSpeed))
		{
			currentSpace = nextSpace;
		}
		// distance to move is sign * minimum between the distance to get there and movement speed
		float distanceToMove = displacement.z / abs(displacement.z) * min(abs(displacement.z), abs(movementSpeed));
		currentPos.z += distanceToMove;
		rotX += distanceToMove/ 0.5f; //   distance/radius = radians
		xRot = true;
	}

	actor->moveGlobalPosition(currentPos.asNxVec3());
}


float RollingBall::getRotX()
{
	return rotX;
}
float RollingBall::getRotZ()
{
	return rotZ;
}

void RollingBall::drawObject(IDirect3DDevice9 *d3dDev)
{
	D3DXMATRIX worldMat, viewMat, matTransform, matProjection, matScale, matTranslate,  matRotation;

	if(actor == NULL)
	{
		return;
	}
	
	//scaling
	D3DXMatrixScaling(&matScale,1.0f, 1.0f, 1.0f);
	worldMat = matScale;

	//rotation
	if(xRot)
		D3DXMatrixRotationX(&matRotation, getRotX());
	else
		D3DXMatrixRotationZ(&matRotation, getRotZ());
	worldMat *= matRotation;

	//translation
	D3DXMatrixTranslation(&matTranslate, getX(), getY(), getZ());
	worldMat *= matTranslate;

	//final matrix = ISROT, identity * scale * rotation * orbit * translation
	d3dDev->SetTransform(D3DTS_WORLD, &worldMat);
	//set texture
	d3dDev->SetTexture(0, texture);
	
	//draw object
	mesh->DrawSubset(0);
}