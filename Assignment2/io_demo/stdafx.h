// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <math.h>
#include <ctime>
#include<iostream>

//directx
#include <d3d9.h>
#include <d3dx9.h>


#ifndef PLAYER
#define  PLAYER  0
#endif PLAYER

#ifndef TERRAIN
#define  TERRAIN  1
#endif TERRAIN

#ifndef TRAP
#define  TRAP 2
#endif TRAP

#ifndef FLOOR
#define  FLOOR 3
#endif FLOOR

#ifndef END_OBJECT
#define  END_OBJECT 4
#endif END_OBJECT