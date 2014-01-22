/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "Level.h"
#include "MeshObject.h"
using namespace std;



Level::Level(IDirect3DDevice9 *d3dDev, NxUserContactReport* contactCallback, int height, int difficulty)
{
	scene = PhysicsEngine::createScene(contactCallback);
	//mazes = new Maze[height];
	mazes =  vector<Maze*>();
	levelHeight = height;
	levelDifficulty = difficulty;
	spaceSize = 1.0;
	maxMazeSize = 32;


	int mazeSize = maxMazeSize;
	for(int i=0; i< levelHeight; i++)
	{
		mazeSize /= 2;
		mazes.push_back(new Maze(d3dDev, scene, mazeSize,mazeSize));
	}

	Maze *parentMaze = NULL;
	//all other levels, lower level exit is the entrance of the next 
	for(int i=0; i< levelHeight; i++)
	{
		mazes[i]->generateMaze(d3dDev, scene, parentMaze);
		parentMaze = mazes[i];
	}

}
Level::~Level(void)
{
	release();
}
void Level::updateState(float elapsedTime)
{
	for(int i=0; i<levelHeight; i++)
	{
		mazes[i]->update(elapsedTime);
	}
}
void Level::draw(IDirect3DDevice9 *d3dDev)
{
	for(int i=0; i<levelHeight; i++)
	{
		mazes[i]->draw(d3dDev);
	}
}

NxScene* Level::getScene()
{
	return scene;
}

/*
Returns the starting position at the top level of the maze
*/

Vector Level::getStartPos()
{
	Maze *topMaze = mazes[levelHeight-1];
	float startX = topMaze->getEndX()* spaceSize ;
	float startY = levelHeight * spaceSize - 0.25f;
	float startZ = topMaze->getEndZ()* spaceSize ;

	return Vector(startX, startY, startZ);
}
void Level::release()
{
	PhysicsEngine::release(scene);

	for(int i=0; i<(int)mazes.size();i++)
		delete mazes[i];
}

MeshObject* Level::getEndObject()
{
	return mazes[0]->endObject;
}

