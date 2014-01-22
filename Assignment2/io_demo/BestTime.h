#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>

class BestTime
{
public:
	BestTime(void);
	~BestTime(void);
	void saveBestTimes(void);
	void loadBestTimes(void);
	float getLevel1BT(void);
	float getLevel2BT(void);
	float getLevel3BT(void);
	int getnumLevelUnlocked(void);
	void setLevel1BT(float f);
	void setLevel2BT(float f);
	void setLevel3BT(float f);
	void setnumLevelUnlocked(int f);

private:
	float level1BT;
	float level2BT;
	float level3BT;
	int numLevelUnlocked;
};

