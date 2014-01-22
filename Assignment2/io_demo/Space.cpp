/*
Author: Nick Tierney
*/
#include "StdAfx.h"
#include "Space.h"
#include "Maze.h"
#include "Rand.h"
ofstream Space::f;
Space::Space()
{
	xPos = 0;
	zPos = 0;
	parentSpace = 0;
	spaceIsWall = true;
	visited = false;
	value = Rand::getRandomNumber(0, INT_MAX);
	if(!f.is_open())
	{
		f.open("aaaa.txt");
	}
}
Space::Space(int x, int z)
{
	xPos = x;
	zPos = z;
	parentSpace = 0;
	spaceIsWall = true;
	visited = false;
}
Space::~Space(void)
{
	if(f.is_open())
	{
		f.close();
	}

}

bool Space::isWall()
{
	return spaceIsWall;
}
bool Space::isVisited()
{
	return visited;
}
void Space::setWall(bool isAWall)
{
	spaceIsWall = isAWall;
}

void Space::setVisited(bool isVisited)
{
	visited = isVisited;
}

void Space::setParent(Space *parent)
{
	parentSpace = parent;
}
Space* Space::getParent()
{
	return parentSpace;
}

int Space::getX()
{
	return xPos;
}
int Space::getZ()
{
	return zPos;
}

void Space::setPos(int x, int z)
{
	xPos = x;
	zPos = z;
}

string Space::toString()
{
	string s = "";
	s += '('+ getX() +"," + getZ() +')';
	return s;
}

bool Space::operator<( const Space& s2) const 
{ 
	return value < s2.value;
} 
bool Space::operator>( const Space& s2) const 
{ 
	return value > s2.value;
} 
ostream& operator << ( ostream& os, const Space& s )
{
	os <<  "value: " << s.value;

	return os;
}
ifstream& operator >> ( ifstream& fin, Space& s )
{
  fin>>s.value;
  return fin;
}