/**
* Author: Nick Tierney
*
* Maze
* 
* Creates a maze through the use of a modified prim's algorithm, so that the path width is always 1.

*/


#include "StdAfx.h"
#include "Maze.h"
#include "Rand.h"
#include <string>
#include <fstream>
#include "MeshObject.h"

using namespace std;

//function declaration
template <typename T>
T remove_at(std::vector<T>&v, typename std::vector<T>::size_type n);


float Maze::spaceSize = 1.0f;

Maze::Maze(IDirect3DDevice9 *d3dDev, NxScene *scene, int width, int depth)
{
	mazeWidth = width;
	mazeDepth = depth;
	pre = 0;
	post = 0;
	longestCycleStart = NULL;
	longestCycleEnd = NULL;
	longestPathStart = NULL;
	longestPathEnd = NULL;


	walls = vector<Wall*>();
	floorPanels = vector<FloorPanel*>();
	balls = vector<RollingBall*>();
	//allocate memory for spaces array
	spaces =  new Space*[mazeWidth];
	for(int i=0;i<mazeDepth;i++)
	{
		spaces[i] = new Space[depth];
	}

	//myfile.open ("example.txt");
	//create space objects 
	for(int i=0;i<mazeWidth;i++)
	{
		for(int j=0;j<mazeDepth;j++)
		{
			spaces[i][j].setPos(i, j);
		}
	}

}

Maze::~Maze(void)
{
	release();
}
void Maze::generateMaze(IDirect3DDevice9 *d3dDev, NxScene *scene, Maze *parent)
{
	
	if(parent == NULL)
	{
		//bottom level so just get a random starting place
		startSpace = getSpace( Rand::getRandomNumber(0, mazeWidth),  Rand::getRandomNumber(0, mazeDepth));
		offsetX = 0;
		offsetY = 0;
		offsetZ = 0;
	}
	else
	{
		//maze will be smaller and rest on top of parents starting place. How much the maze is offset is random 
		//but it still must rest on the start of the parent maze
		int leftBound = max(0, parent->getEndX()-mazeWidth +1);
		int rightBound = min(parent->getEndX(), parent->mazeWidth+parent->offsetX-mazeWidth);
		int topBound = max(0, parent->getEndZ()-mazeDepth +1);
		int bottomBound = min(parent->getEndZ(), parent->mazeDepth+parent->offsetZ-mazeDepth);
		
		offsetX = Rand::getRandomNumber(leftBound, rightBound);
		offsetZ = Rand::getRandomNumber(topBound, bottomBound);
		//up one level
		offsetY = parent->offsetY+1;

		startSpace = getSpace(parent->getEndX() -offsetX, parent->getEndZ() - offsetZ);
	}

	//run prims algorithm on spaces to create the maze
	vector<Space*> wallList;
	startSpace->setWall(false);
	startSpace->setVisited(true);
	addNeighboursToWallList(wallList, startSpace);
	
	while(wallList.size() >0)
	{
		//get the next space from list of spaces remaining
		Space* randomSpace = remove_at(wallList, Rand::getRandomNumber(0, wallList.size()-1));
		Space* oppositeSpace = getOppositeSpace(randomSpace);
		Space** adjacentSpaces = getAdjacent(randomSpace);

		//make sure that we can make a path of length 2
		//make sure that we can make a path of width 1
	
		
		if((adjacentSpaces[0] == NULL || adjacentSpaces[0]->isWall())
			&&(adjacentSpaces[1] == NULL || adjacentSpaces[1]->isWall()))
		{
			if(oppositeSpace != NULL && oppositeSpace->isWall())  
			{
				randomSpace->setWall(false);
				oppositeSpace->setWall(false);
				addNeighboursToWallList(wallList, randomSpace);
			}
			else if(oppositeSpace == NULL)
			{
				randomSpace->setWall(false);
				addNeighboursToWallList(wallList, randomSpace);
			}
		}
		randomSpace->setVisited(true);


	
	}
	
	SpaceNode *dijkstras = calculateDijkstras(startSpace);
	
	SpaceNode *lpe = longestPathEnd;
	//create the actual game objects that we will be drawing
	for(int i=0;i<mazeWidth;i++)
	{
		for(int j=0;j<mazeDepth;j++)
		{
			Space *space = getSpace(i,j);
			float x, y, z;
			x = offsetX * spaceSize + space->getX()* spaceSize;
			z = offsetZ * spaceSize + space->getZ()* spaceSize;

			//remove the space at the ending and add the hints for going down a level + ending
			if(space == startSpace)
			{
				if(parent == NULL)
				{
					endObject = new MeshModel(d3dDev, scene, Vector(x, offsetY*spaceSize, z), "res\\Arrow", "arrow.x", spaceSize, spaceSize, spaceSize, true);
					//endObject = new MeshModel(d3dDev, scene, Vector(x-0.5f, offsetY*spaceSize, z), "res\\Treasure", "treasurestacked.x", spaceSize, spaceSize, spaceSize, true);
					endObject->actor->setGroup(END_OBJECT);
					endObject->actor->raiseBodyFlag(NX_BF_FROZEN);
					endObject->scaleX = 0.5f;
					endObject->scaleY = 0.5f;
					endObject->scaleZ = 0.5f;
					endObject->rotX = 3.14f;
				}
				else
				{
					endObject = new MeshModel(d3dDev, scene, Vector(x, offsetY*spaceSize, z), "res\\Arrow", "arrow.x", spaceSize, spaceSize, spaceSize, false);
					endObject->scaleX = 0.5f;
					endObject->scaleY = 0.5f;
					endObject->scaleZ = 0.5f;
					endObject->rotX = 3.14f;
					continue;
				}
				
			}

			if(space->isWall())
			{
				y = offsetY * spaceSize;
				walls.push_back(new Wall(d3dDev, scene, Vector(x, y, z), spaceSize));
			}
			else
			{
				//edge cases
				y = offsetY * spaceSize - spaceSize * 0.5f + spaceSize * 0.01f;
				floorPanels.push_back(new FloorPanel(d3dDev, scene, Vector(x, y, z), spaceSize));

				if(i == 0) {
					x = offsetX * spaceSize + space->getX() * spaceSize - spaceSize;
					z = offsetZ * spaceSize + space->getZ()* spaceSize;
					y = offsetY * spaceSize;
					walls.push_back(new Wall(d3dDev, scene, Vector(x, y, z), spaceSize));
				} 

				if(i == (mazeWidth - 1))
				{
					x = offsetX * spaceSize + space->getX() * spaceSize + spaceSize;
					z = offsetZ * spaceSize + space->getZ()* spaceSize;
					y = offsetY * spaceSize;
					walls.push_back(new Wall(d3dDev, scene, Vector(x, y, z), spaceSize));
				}

				if(j == 0)
				{
					x = offsetX * spaceSize + space->getX() * spaceSize;
					z = offsetZ * spaceSize + space->getZ()* spaceSize - spaceSize;
					y = offsetY * spaceSize;
					walls.push_back(new Wall(d3dDev, scene, Vector(x, y, z), spaceSize));
				}

				if(j == (mazeDepth - 1))
				{
					x = offsetX * spaceSize + space->getX() * spaceSize;
					z = offsetZ * spaceSize + space->getZ()* spaceSize + spaceSize;
					y = offsetY * spaceSize;
					walls.push_back(new Wall(d3dDev, scene, Vector(x, y, z), spaceSize));
				}
			}
		}
	}

	//traverse the maze to create a tree
	SpaceNode *exploreStart = exploreMaze();
	SpaceNode *start = exploreStart;
	if(longestCycleStart != NULL &&(longestCycleStart->height - longestCycleEnd->height ) >4)
	{
		RollingBall *ball = new RollingBall(d3dDev, scene, Vector(longestCycleStart->getSpace()->getX()*spaceSize + this->offsetX* spaceSize, offsetY*spaceSize, longestCycleStart->getSpace()->getZ()*spaceSize + this->offsetZ* spaceSize), 1.0f, longestCycleStart, longestCycleEnd);
		balls.push_back(ball);
	}


}

void Maze::addNeighboursToWallList(std::vector<Space*> &wallList, Space* space)
{
	int spaceX = space->getX();
	int spaceY = space->getZ();


	addToWallList(wallList, space, getSpace(spaceX-1, spaceY)); //left
	addToWallList(wallList, space, getSpace(spaceX+1, spaceY)); //right
	addToWallList(wallList, space, getSpace(spaceX, spaceY-1)); //bottom
	addToWallList(wallList, space, getSpace(spaceX, spaceY+1)); //top

}
Space* Maze::getOppositeSpace(Space *space)
{
	Space* oppositeSpace = NULL;
	Space* parent = space->getParent();;

	if(parent != NULL)
	{
		int x = space->getX() + (1*(space->getX() - space->getParent()->getX()));
		int y = space->getZ() + (1*(space->getZ() - space->getParent()->getZ()));

		oppositeSpace = getSpace(x, y);
	}
	return oppositeSpace;
}

void Maze::addToWallList(std::vector<Space*> &wallList, Space *parent, Space *space)
{

	//only add the space to the list if it hasnt already been visited
	if(space != NULL && !space->isVisited())
	{
		//the adjacent spaces must also not be in the list so we do not create parallel walls
		space->setParent(parent);
		wallList.push_back(space);
	}
}

Space* Maze::getSpace(int spaceX, int spaceY)
{
	Space *space = NULL;
	
	if(spaceX >= 0 && spaceX < mazeWidth && spaceY>=0 && spaceY < mazeDepth)
		space = &spaces[spaceX][spaceY];
	
	//myfile << "getspace: " << spaceX + ',' + spaceY << (space==NULL?"null":"not") << '\n';
	return space;
}


Space** Maze::getAdjacent(Space* space)
{
	Space* parent = space->getParent();
	Space** adjacent = new Space*[2];
	adjacent[0] = NULL;
	adjacent[1] = NULL;

	adjacent[0] = getSpace(space->getX() + (-1*(space->getZ() - parent->getZ())), space->getZ() + (-1*(space->getX() - parent->getX())));
	adjacent[1] = getSpace(space->getX() + ((space->getZ() - parent->getZ())), space->getZ() + ((space->getX() - parent->getX())));

	return adjacent;
}

void Maze::drawMaze(LPD3DXFONT font, RECT textBox, char *s)
{
	string mazeString = "";
	for(int i=0;i<mazeWidth;i++)
	{
		for(int j=0;j<mazeDepth;j++)
		{
			mazeString += getSpace(i,j)->isWall()?'X':' ';
		}
		mazeString += '\n';
	}
//	sprintf(s, mazeString.c_str());
//	font->DrawText(NULL, s, -1, &textBox, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 0)); 
}
int Maze::getStartX()
{
	return startSpace->getX() + offsetX;
}
int Maze::getStartZ()
{
	return startSpace->getX() + offsetZ;
}
int Maze::getEndX()
{
	return longestPathEnd->getSpace()->getX() + offsetX;
}
int Maze::getEndZ()
{
	return longestPathEnd->getSpace()->getZ() + offsetZ;
}
void Maze::update(float elapsedTime)
{
	for(int i=0;i<(int)balls.size();i++)
		balls[i]->updatePosition(1/30, offsetX, offsetY, offsetZ, spaceSize);
}
void Maze::draw(IDirect3DDevice9 *d3dDev)
{
	//draw walls
	for(int i=0;i<(int)walls.size();i++)
		walls[i]->drawObject(d3dDev);
	//draw floor panels
	for(int i=0;i<(int)floorPanels.size();i++)
		floorPanels[i]->drawObject(d3dDev);
	//draw balls
	for(int i=0;i<(int)balls.size();i++)
		balls[i]->drawObject(d3dDev);

	endObject->drawObject(d3dDev);


}

/**
* Removes an item at location n in a vector and returns it
***sourcehttp://stackoverflow.com/questions/9218724/get-random-element-and-remove-it
*/

template <typename T>
T remove_at(std::vector<T>&v, typename std::vector<T>::size_type n)
{
	std::swap(v[n], v.back());
	T ans = v.back();
    v.pop_back();
    return ans;
}

SpaceNode* Maze::exploreMaze()
{
	SpaceNode ***nodes;
	//allocate memory for nodes
	nodes = (SpaceNode***)malloc(mazeWidth*sizeof(SpaceNode*));
	for(int i=0;i<mazeWidth;i++)
		nodes[i] = (SpaceNode**)malloc(mazeDepth*sizeof(SpaceNode*));

	//initialize nodes
	for(int i=0;i<mazeWidth;i++)
		for(int j=0;j<mazeDepth;j++)
			nodes[i][j] = new SpaceNode(&spaces[i][j]);

	//begin the traverse from where we created the maze from.
	SpaceNode *start = getSpaceNode(startSpace->getX(), startSpace->getZ(),nodes);
	start->height = 0;
	explore(start , nodes);
	
	return start;
}

void Maze::explore(SpaceNode *node, SpaceNode ***nodes)
{
	node->prev = pre++;
	node->visited = true;
	int x = node->getSpace()->getX();
	int z = node->getSpace()->getZ();
	
	//explore adjacent spaces
	SpaceNode *leftSpaceNode = getSpaceNode(x-1, z, nodes);
	SpaceNode *rightSpaceNode = getSpaceNode(x+1, z, nodes);
	SpaceNode *topSpaceNode = getSpaceNode(x, z+1, nodes);
	SpaceNode *bottomSpaceNode = getSpaceNode(x, z-1, nodes);

	visitNode(node, leftSpaceNode, nodes);
	visitNode(node, rightSpaceNode, nodes);
	visitNode(node, bottomSpaceNode, nodes);
	visitNode(node, topSpaceNode, nodes);

	node->post = post++;
}

void Maze::visitNode(SpaceNode *parentNode, SpaceNode *node, SpaceNode ***nodes)
{
	if( node != NULL && !node->getSpace()->isWall())
	{
		if(!node->visited)
		{
			//didnt find it, explore this to traverse
			node->parent = parentNode;
			node->height = parentNode->height +1;
			explore(node, nodes);
		}
		else
		{
			//cycle found, add backpath
			if(node ->parent != parentNode && node->prev > parentNode->prev && parentNode->post == -1)
			{
				node->backPaths.push_back(parentNode);

				//Keep track of longest cycle. Can get this by just calculating the difference of height between the nodes in the tree
				if(longestCycleStart == NULL || ((node->height - parentNode->height) > (longestCycleStart->height - longestCycleEnd->height)))
				{
					longestCycleStart = node;
					longestCycleEnd = parentNode;
				}
			}
		}
	}
}

SpaceNode* Maze::getSpaceNode(int x, int z, SpaceNode ***nodes)
{
	SpaceNode *node = NULL;
	if (x>=0 && x<mazeWidth && z>=0 && z<mazeDepth)
		node = nodes[x][z];

	return node;
}
SpaceNode* Maze::calculateDijkstras(Space* space)
{
	SpaceNode ***nodes;
	//allocate memory for nodes
	nodes = (SpaceNode***)malloc(mazeWidth*sizeof(SpaceNode*));
	for(int i=0;i<mazeWidth;i++)
		nodes[i] = (SpaceNode**)malloc(mazeDepth*sizeof(SpaceNode*));

	for(int i=0;i<mazeWidth;i++)
		for(int j=0;j<mazeDepth;j++)
			nodes[i][j] = new SpaceNode(&spaces[i][j]);

	//create queue q
	vector<SpaceNode*> q = vector<SpaceNode*>();

	//initialize nodes
	for(int i=0;i<mazeWidth;i++)
	{
		for(int j=0;j<mazeDepth;j++)
		{
			q.push_back(nodes[i][j]);
		}
	}
	SpaceNode *startSpaceNode = getSpaceNode(space->getX(), space->getZ(), nodes);
	startSpaceNode->value = 0;

         
	  while(q.size()>0)
	  {
			  //get smallest element, not enough time to fix std heap (decrease_key takes O(n) time with it) so just using array
			  int smallest = 0;
			  for(int i=0;i<q.size();i++)
			  {
				  if(q[i]->value <  q[smallest]->value)
				  {
					  smallest = i;
				  }
			  }

			  SpaceNode *u = q[smallest];
			  remove_at(q, smallest);

			  cout<<"bl";
			
			 if(u->value == INT_MAX)
			 {
				 cout<<"blah"<<endl;
				 return startSpaceNode;
			 }
			  //keep track of the longest path
			 if(longestPathEnd == NULL || u->value >= longestPathEnd->value)
			 {
				 longestPathEnd = u;
			 }


			 int x = u->getSpace()->getX();
			 int z = u->getSpace()->getZ();

				//explore adjacent spaces
			SpaceNode *leftSpaceNode = getSpaceNode(x-1, z, nodes);
			SpaceNode *rightSpaceNode = getSpaceNode(x+1, z, nodes);
			SpaceNode *topSpaceNode = getSpaceNode(x, z+1, nodes);
			SpaceNode *bottomSpaceNode = getSpaceNode(x, z-1, nodes);

			assignValue(leftSpaceNode, u);
			assignValue(rightSpaceNode, u);
			assignValue(topSpaceNode, u);
			assignValue(bottomSpaceNode, u);

	  }
	  return startSpaceNode;
}
void Maze::release()
{
	delete[] spaces;

	delete endObject;
	
	for(int i=0;i<(int)walls.size();i++)
		delete walls[i];

	for(int i=0;i<(int)floorPanels.size();i++)
		delete floorPanels[i];

	for(int i=0;i<(int)balls.size();i++)
		delete balls[i];

}

void Maze::assignValue(SpaceNode *child, SpaceNode *parent)
{ 
	if(child != NULL && !child->getSpace()->isWall())
	{
		int newDist = parent->value + 1;
		if(newDist < child->value)
		{
			child->value = newDist;
			child->parent = parent;
		}
	}

}
