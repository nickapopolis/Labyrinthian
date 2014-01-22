
#pragma once
#include "stdafx.h"
#include <string>
using namespace std;
class Space
{
public:
	Space();
	Space(int x, int z);
	~Space(void);

	void setWall(bool isAWall);
	void setVisited(bool isVisited);
	void setParent(Space *parent);
	bool isWall();
	bool isVisited();
	Space* getParent();
	int getX();
	int getZ();
	void setPos(int x, int z);
	string toString();
	bool operator>(Space& rhs);
	static ofstream f;
	bool operator<( const Space& s2) const ;
	bool operator>( const Space& s2) const ;
	int value;
private:
	bool spaceIsWall;
	bool visited;
	Space *parentSpace;
	int xPos, zPos;
};

