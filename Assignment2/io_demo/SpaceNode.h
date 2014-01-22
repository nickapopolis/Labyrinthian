#pragma once
#include "Space.h"
#include "stdafx.h"
class SpaceNode
{
public:
	SpaceNode(Space *space);
	~SpaceNode(void);

	Space* getSpace();
	void addBackPath(SpaceNode *node);

public:
	SpaceNode *parent;
	vector<SpaceNode*> backPaths;
	vector<SpaceNode*> children;
	bool visited;
	int prev;
	int post;
	int height;
	int value;

protected:
	
	Space *space;

};

