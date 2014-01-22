
#ifndef RAND_H
#define RAND_H

#include "stdafx.h"
class Rand
{
public:
	Rand(){};
	~Rand(void){};
	static int getRandomNumber(int min, int max)
	{
	// x is in [0,1[
	  double randomNum = rand()/static_cast<double>(RAND_MAX); 

	  // [0,1[ * (max - min) + min is in [min,max[
	  int that = min + static_cast<int>( randomNum * (max - min) );

	  return that;
	}
};

#endif