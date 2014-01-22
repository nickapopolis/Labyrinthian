/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "PhysicsEngine.h"
#include <iostream>
#include <fstream>
using namespace std;

NxPhysicsSDK* PhysicsEngine::physicsSDK = NULL;

PhysicsEngine::PhysicsEngine(void)
{
}


PhysicsEngine::~PhysicsEngine(void)
{
	
}

/**
initializes the physics engine with scene and gravity
**/
int PhysicsEngine::initialize()
{
	cout<<"physics engine initialization";
	physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL);
	
	return physicsSDK == NULL;
}

NxScene* PhysicsEngine::createScene(NxUserContactReport* contactCallback)
{
	NxScene* scene = NULL;
	NxSceneDesc sceneDesc;
	sceneDesc.gravity.set(0,-9.81f, 0);
	scene = physicsSDK->createScene(sceneDesc);

	

	if(scene != NULL)
	{
		NxMaterial* defaultMaterial = scene->getMaterialFromIndex(0); 
		defaultMaterial->setRestitution(0.5f);
		defaultMaterial->setStaticFriction(0.9f);
		defaultMaterial->setDynamicFriction(0.9f);
		
		//ignore collisions between self and traps/terrain
		scene->setGroupCollisionFlag(FLOOR, FLOOR, FALSE);
		scene->setGroupCollisionFlag(TERRAIN, FLOOR, FALSE);
		scene->setGroupCollisionFlag(FLOOR, TRAP, FALSE);
		scene->setGroupCollisionFlag(TERRAIN, TRAP, FALSE);
		scene->setGroupCollisionFlag(TRAP, TRAP, FALSE);
		scene->setGroupCollisionFlag(TERRAIN, TERRAIN, FALSE);
		scene->setGroupCollisionFlag(PLAYER, TRAP, TRUE);
		scene->setGroupCollisionFlag(PLAYER, TERRAIN, TRUE);
		scene->setGroupCollisionFlag(TRAP, END_OBJECT, FALSE);

		scene->setGroupCollisionFlag(PLAYER, END_OBJECT, TRUE);
		scene->setUserContactReport(contactCallback);
	}
	return scene;
}
/**
Advances the physics engine by a specified time amount
**/
void PhysicsEngine::simulate(NxScene *scene, float elapsedTime)
{
	scene->simulate(elapsedTime/1000);
	scene->flushStream();
	scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
}

/**
Stops the physics engine and releases objects involved with it
**/
void PhysicsEngine::release(NxScene *scene)
{
	if(physicsSDK != NULL)
	{
		if(scene != NULL)
			physicsSDK->releaseScene(*scene);
		scene = NULL;
	}
}


NxActor* PhysicsEngine::physicsSphere (NxScene *scene, Vector &position, Vector &velocity, float diameter, float mass)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	NxActor *actor;

	// The actor has one shape, a sphere, 1m on radius
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius		= diameter* 0.5f;
	sphereDesc.localPose.t	= NxVec3(0, 0, 0);
	sphereDesc.mass = mass;
	

	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(position.x, position.y, position.z);	

	actor = scene->createActor(actorDesc);
	actor->setLinearVelocity(NxVec3(velocity.x, velocity.y, velocity.z));
	actor->setLinearDamping(0.98f);
	return actor;
}
NxActor* PhysicsEngine::physicsBox (NxScene *scene,Vector &position, Vector &velocity, float width, float height, float depth, float mass)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	NxActor *actor;

	// The actor has one shape, a sphere, 1m on radius
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions = NxVec3(width*0.5f, height*0.5f, depth*0.5f);
	boxDesc.localPose.t	= NxVec3(0, 0, 0);
	boxDesc.mass = mass;

	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.body			= &bodyDesc;
	actorDesc.density		= 10.0f;
	actorDesc.globalPose.t	= NxVec3(position.x, position.y, position.z);

	actor = scene->createActor(actorDesc);
	actor->setLinearVelocity(NxVec3(velocity.x, velocity.y, velocity.z));

	
	return actor;
}

bool PhysicsEngine::sweptSphereHits (NxActor *physicsSphere, Vector &displacement, double &intersectionT, Vector &intersectionPoint) {
	//Returns true if there is an intersection along with intersectionT and the intersection point; otherwise, false with intersectionT = 1.0
	//and the intersection point set to "to". Hence the intersection point can be interpreted as where you can get to if you try to go 
	//from "from" to "to"...

	//We need the sphere geometry of the shape of the physics sphere (an actor)..
	if (physicsSphere->getNbShapes () != 1) 
	{
		printf ("\nSphere sweeping with a sphere with %d shapes instead of 1...", physicsSphere->getNbShapes ());
		exit(0);
	}

    NxU32 flags = NX_SF_ALL_HITS;
    NxShape *sweepShape;// Shape in actor
	NxShape *hitShape; // Shape in scene
    NxSweepQueryHit result;
	result.point.x = 0;
	result.point.y = 0;
	result.point.z = 0;
	//query the physics scene for a result
	physicsSphere->linearSweep(displacement.asNxVec3(), flags, NULL, 1, &result, NULL);
    	
	intersectionPoint.x = result.point.x;
	intersectionPoint.y = result.point.y;
	intersectionPoint.z = result.point.z;

	intersectionT = result.t;

	sweepShape = result.sweepShape;
	hitShape = result.hitShape;

	if(result.point.x ==0 && result.point.y == 0 && result.point.z == 0 )
		return false;
	else
		return true;
} 

void PhysicsEngine::release()
{
	NxReleasePhysicsSDK(physicsSDK);
	physicsSDK = NULL;
}