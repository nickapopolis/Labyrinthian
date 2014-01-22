/*
Author: Nick Tierney
*/
#include "StdAfx.h"
#include "SpaceNode.h"


SpaceNode::SpaceNode(Space *space)
{
	this->space = space;
	backPaths  = vector<SpaceNode*>();
	children = vector<SpaceNode*>();
	visited = false;
	prev = -1;
	post = -1;
	height = -1;
	value = INT_MAX;
}


SpaceNode::~SpaceNode(void)
{
}

Space* SpaceNode::getSpace()
{
	return space;
}
void SpaceNode::addBackPath(SpaceNode *node)
{
	backPaths.push_back(node);
}