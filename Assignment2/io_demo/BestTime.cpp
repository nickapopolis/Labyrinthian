/*
Author: Tyler Haskell
*/
#include "StdAfx.h"
#include "nuss_util.h"
#include "BestTime.h"
#include <iostream>
#include <fstream>

using namespace std;


BestTime::BestTime(void)
{
	level1BT = 100.0f;
	level2BT = 200.0f;
	level3BT = 300.0f;
	numLevelUnlocked = 1;
}


BestTime::~BestTime(void)
{
}

void BestTime::saveBestTimes(void)
{
  ofstream out("scores.bin", ios_base::binary);
  if(out.good())
  {
    out.write((char *)&level1BT,sizeof(float));
	out.write((char *)&level2BT,sizeof(float));
	out.write((char *)&level3BT,sizeof(float));
	out.write((char *)&numLevelUnlocked,sizeof(int));
    out.close();
  }
}

void BestTime::loadBestTimes(void)
{
  ifstream in("scores.bin", ios_base::binary);
  if(in.good())
  {
    in.read((char *)&level1BT,sizeof(float));
	in.read((char *)&level2BT,sizeof(float));
	in.read((char *)&level3BT,sizeof(float));
	in.read((char *)&numLevelUnlocked,sizeof(int));
	in.close();
  }
}

float BestTime::getLevel1BT(void)
{
	return(level1BT);
}

float BestTime::getLevel2BT(void)
{
	return(level2BT);
}

float BestTime::getLevel3BT(void)
{
	return(level3BT);
}

int BestTime::getnumLevelUnlocked(void)
{
	return(numLevelUnlocked);
}

void BestTime::setLevel1BT(float f)
{
	if(f <= level1BT)
		level1BT = f;
}

void BestTime::setLevel2BT(float f)
{
	if(f <= level2BT)
		level2BT = f;
}

void BestTime::setLevel3BT(float f)
{
	if(f <= level3BT)
		level3BT = f;
}

void BestTime::setnumLevelUnlocked(int f)
{
		numLevelUnlocked = f;
}