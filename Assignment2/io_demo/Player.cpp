/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "Player.h"
#include <fstream>

IDirect3DTexture9* Player::playerTex = NULL;

Player::Player(IDirect3DDevice9 *d3dDev, Vector &position, float size )
	:MeshSphere(d3dDev, NULL, playerTex, position, Vector(0.0f, 0.0f, 0.0f), size)
{
	movementSpeed = 0.5f;
	turnSpeed = 0.07f;
	orient = 0;
	velocity = Vector(0.0f, 0.0f, 0.0f);
	hasBeatLevel = false;
	health = 3;
}


Player::~Player(void)
{
	
}

//static texture initialization
int  Player::initializeTexture(IDirect3DDevice9 *d3dDev)
{
	return D3DXCreateTextureFromFile(d3dDev,TEXT("res\\thwomp.png"), &playerTex);
}
void Player::updateState(long elapsedTime, gameIO *mInput)
{
	velocity.x = 0;
	velocity.z = 0;

	//turn left
	if(mInput->keyboardPressed(DIK_A))
		orient += turnSpeed;
	//turn right
	if(mInput->keyboardPressed(DIK_D))
		orient -= turnSpeed;
	//move forward
	if(mInput->keyboardPressed(DIK_W) && !mInput->keyboardPressed(DIK_S))
	{
		velocity.x = (float)cos(orient) * movementSpeed;
		velocity.z = (float)sin(orient) * movementSpeed;
	}
	//move backward
	if(!mInput->keyboardPressed(DIK_W) && mInput->keyboardPressed(DIK_S))
	{
		velocity.x = -(float)cos(orient) * movementSpeed;
		velocity.z = -(float)sin(orient) * movementSpeed;
	}

	

	move(elapsedTime);
}
void Player::move(long elapsedTime)
{
	Vector displacement(velocity.x*100 ,0,velocity.z*100 );
	Vector gravity(0 ,-80.81f,0);
	actor->addForce(gravity.asNxVec3());
	actor->addForceAtPos(displacement.asNxVec3(), getPos().asNxVec3());
}

NxActor* Player::getActor()
{
	return actor;
}

void Player::acquireActor(NxScene *scene, Vector position, float size)
{
	actor = PhysicsEngine::physicsSphere(scene, position, Vector(0.0f, 0.0f, 0.0f), size, 10.0f);
	//actor->raiseBodyFlag(NX_BF_KINEMATIC);
	actor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
	actor->setContactReportFlags(NX_NOTIFY_ON_TOUCH);
	actor->setGroup(PLAYER);
	actor->userData = this;
}

float Player::getOrientation()
{
	return orient;
}

void Player::adjustHealth(int diff)
{

	if(diff>0)
		health = min(maxHealth, health+diff);
	else if(diff<0)
		health = min(0, health +diff);
}
int Player::getHealth()
{
	return health;
}
