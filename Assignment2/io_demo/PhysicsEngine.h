#pragma once
#undef min
#undef max
#include<NxPhysics.h>
#include <typeinfo>
#include "Vector.h"
#include "stdafx.h"

class PhysicsEngine
{
public:
	PhysicsEngine(void);
	~PhysicsEngine(void);

	static int initialize();
	static void simulate(NxScene *scene, float elapsedTime);
	static void release(NxScene *scene);
	static void release();

	static NxScene* createScene(NxUserContactReport* contactCallback);

	static NxActor* physicsSphere (NxScene *scene, Vector &position, Vector &velocity, float diameter, float mass);
	static NxActor* physicsBox (NxScene *scene, Vector &position, Vector &velocity, float width, float height, float depth, float mass);

	static bool sweptSphereHits (NxActor *physicsSphere, Vector &displacement, double &intersectionT, Vector &intersectionPoint);

	static NxPhysicsSDK *physicsSDK;

};

