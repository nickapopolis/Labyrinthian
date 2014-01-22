#pragma once
#include "Maze.h"
#include<vector>
#include "stdafx.h"
#include "MeshObject.h"
using namespace std;

class Level
{
public:
	Level(IDirect3DDevice9 *d3dDev,NxUserContactReport* contactCallback, int height, int difficulty);
	~Level(void);
	
	void updateState(float elapsedTime);
	void draw(IDirect3DDevice9 *d3dDev);
	void release();
	NxScene* getScene();
	Vector getStartPos();

	MeshObject* getEndObject();
protected:
	vector<Maze*> mazes;
	//Maze *mazes;
	int levelHeight;
	int levelDifficulty;
	int maxMazeSize;
	float spaceSize;
	NxScene *scene;
};

