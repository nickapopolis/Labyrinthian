#pragma once
#include "stdafx.h"
#include "PhysicsEngine.h"
#include "Vector.h"
#include  "Wall.h"
#include "gameIO.h"
#include "MeshSphere.h"

class Player : public MeshSphere
{
public: 
	Player(IDirect3DDevice9 *d3dDev, Vector &position, float size );
	~Player(void);

	static int  initializeTexture(IDirect3DDevice9 *d3dDev);
	void move(long elapsedTime);
	void updateState(long elapsedTime, gameIO *mInput);
	NxActor *getActor();
	void acquireActor(NxScene *scene, Vector position, float size);

	float getOrientation();
	
	bool hasBeatLevel;
	void adjustHealth(int diff);
	int getHealth();
protected:
	static IDirect3DTexture9* playerTex;
	Vector velocity;
	float orient;
	float movementSpeed; //spaces per second
	float turnSpeed; //cycles per second

	int health;
	int maxHealth;
};

