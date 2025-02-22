
//=============================================================================
// gameApp.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates how to use directx input 
// b. demonstrates how to use directx fonts
// c. demonstrates how to initialize direct3D
// d. provide a framework for creating games
// e. demonstrates how to create an egnine frame and provide a skeleton function of one.
//
// Description:
//--------------
// A simple application that demonstrates how to use directx to draw a font on the screen.
// It uses a framework for a general purpose simple game engine
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed with a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================




#pragma once

#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
//#include <dxerr9.h>
#include "gameio.h"
#include "PhysicsEngine.h"
#include "Camera.h"
#include "AudioPlayer.h"
// DEFINES

#define GAME_TITLE_SISE 128



class gameApp
{
//FUNCTIONS
public:
	gameApp(void);
	~gameApp(void);
	gameApp(HINSTANCE hInstance, char* gameName);

	void setViewMatrix(const D3DXVECTOR3& eye, const D3DXVECTOR3& lookAt, const D3DXVECTOR3& up);
	void setProjectionMatrix(float fov, float aspectRatio, float zClose, float zFar);


	virtual int updateGameState(long time);  // update the game state
	virtual int renderFrame(long time);		// redner the frame
	virtual int gameLoop(void);					// start the game loop
	virtual int initGame(void);				// initialize the game


private:
	
	int initWindow(void); // initialize the game window (assuming only one window)
	int initD3D(void);
	int init(void);


//MEMBERS
protected:
	HINSTANCE mhinstance; // application instance
	HWND mhwnd;			 //window handle


	long mFramesPerSecond;	// maximum numbef of frames per second
	int mFullScreen;  // determines whether the game is played in full screen
	
	int mWndWidth;   // window widt and 
	int mWndHeight;  // window height

	char mGameName[GAME_TITLE_SISE+1];  // title of game (to be used in the window)

	// direct3D members
	IDirect3D9 *md3d;		// com object of directx 9;
	IDirect3DDevice9 *md3dDev;		//direct3D device;
	D3DPRESENT_PARAMETERS md3dpp;  // presentation parameters
	ID3DXSprite *md3dSprite;		// sprite object

	Camera *camera;	
	
	gameIO *mInput;
	int initGameInput(void);

	int getWndWidth(void);
	
	AudioPlayer *audioPlayer;
	NxActor* box;
public:
	int getWndHeight(void);
};
