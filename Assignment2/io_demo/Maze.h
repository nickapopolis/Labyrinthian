#pragma once
#include "Space.h"
#include <queue>
#include <fstream>
#include "stdafx.h"
#include "Wall.h"
#include "FloorPanel.h"
#include <list>
#include "SpaceNode.h"
#include "RollingBall.h"
#include "MeshModel.h"
using namespace std;
class Maze
{
public:
	Maze(IDirect3DDevice9 *d3dDev, NxScene *scene, int width, int depth);
	~Maze(void);
	
	void generateMaze(IDirect3DDevice9 *d3dDev, NxScene *scene, Maze *parent);
	void addNeighboursToWallList(std::vector<Space* > &wallList, Space* space);
	void addToWallList(std::vector<Space* > &wallList, Space *parent, Space *space);
	Space** getAdjacent(Space* space);
	Space* getOppositeSpace(Space *space);
	Space* getSpace(int spaceX, int spaceY);
	void draw(IDirect3DDevice9 *d3dDev);
	void drawMaze(LPD3DXFONT font, RECT textBox, char *s);
	int getStartX();
	int getStartZ();
	int getEndX();
	int getEndZ();
	//dfs algorithm
	SpaceNode* exploreMaze();
	void explore(SpaceNode *node, SpaceNode ***nodes);
	SpaceNode* getSpaceNode(int x, int z, SpaceNode ***nodes);
	void visitNode(SpaceNode *parentNode, SpaceNode *node, SpaceNode ***nodes);
	void assignValue(SpaceNode *child, SpaceNode *parent);
	void update(float elapsedTime);
	//dijkstras algorithm
	SpaceNode* calculateDijkstras(Space* space);

	int offsetX;
	int offsetY;
	int offsetZ;
	
	MeshModel *endObject;
	void release();
private:
	
	vector<Wall*> walls;
	vector<FloorPanel*> floorPanels;
	vector<RollingBall*> balls;
	SpaceNode *longestCycleStart;
	SpaceNode *longestCycleEnd;
	SpaceNode *longestPathStart;
	SpaceNode *longestPathEnd;
	SpaceNode *exploreStart;
	Space **spaces;
	int mazeWidth;
	int mazeDepth;
	static float spaceSize;
	Space * startSpace;
	Space * endSpace;

	int pre;
	int post;

};

