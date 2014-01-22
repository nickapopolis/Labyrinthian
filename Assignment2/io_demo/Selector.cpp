/*
Ziyao Zhou
*/
#include "StdAfx.h"
#include "Selector.h"
#include "nuss_util.h"

/******************************************************/
// STATIC CLASS MEMBERS


/******************************************************/


Selector::Selector(IDirect3DDevice9 *d3dDev)
{
	arrowSprite = NULL;
	arrowSprite2 = NULL;
	previousTime = 10;
	latch = true;
	init(d3dDev, "res\\selector.png", "res\\selector2.png");
}

/******************************************************/


Selector::~Selector(void)
{
	SAFE_RELEASE(arrowSprite);
	SAFE_RELEASE(arrowSprite2);
}


/******************************************************/
//This is a staic function.  
//Purpose: initializes the class by loading the sprite to 
//		 be used by the class

int Selector::init(IDirect3DDevice9 *d3dDev, char * spriteName, char * spriteName2)
{
	int rc = 0;
	rc = D3DXCreateTextureFromFileEx(d3dDev, 
		spriteName2, // file name
		0,			// width
		0,			// height
		D3DX_DEFAULT, // MIPS levels
		0,
		D3DFMT_UNKNOWN,		// format of pixels (take from file)
		//D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT, 
		D3DCOLOR_RGBA(255,255,0, 255),	// transparaent colour
		NULL,
		NULL,
		&arrowSprite);

	rc = D3DXCreateTextureFromFileEx(d3dDev, 
		spriteName, // file name
		0,			// width
		0,			// height
		D3DX_DEFAULT, // MIPS levels
		0,
		D3DFMT_UNKNOWN,		// format of pixels (take from file)
		//D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT, 
		D3DCOLOR_RGBA(255,255,0, 255),	// transparaent colour
		NULL,
		NULL,
		&arrowSprite2);

//	rc = D3DXCreateTextureFromFile(md3dDev, spriteName, &houseSprite);
	switch(rc) {
		case D3DERR_NOTAVAILABLE:
			rc = 1;
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			rc = 2;
			break;
		case D3DERR_INVALIDCALL:
			rc = 3;
			break;
		case D3DXERR_INVALIDDATA:
			rc = 4;
			break;
		default:
			break;
	}
	if (rc != D3D_OK) {
		rc = 1;
		arrowSprite = NULL;
		arrowSprite2 = NULL;
	} else {
		rc = 0;
	}
	return rc;
}

/******************************************************/
// Purpose: renders a house

int Selector::render(ID3DXSprite *d3dSprite, int choose, long time, int windowWidth, int windowHeight)
{
	RECT srcRect;
	D3DXVECTOR3 centre;
	D3DXVECTOR3 position;
	D3DCOLOR colour = D3DCOLOR_RGBA(255,255,255,255);
	centre = D3DXVECTOR3(0.0,0.0,0.0);

	srcRect.top = 0;
	srcRect.left = 0;
	srcRect.bottom = 30;
	srcRect.right = (long)63.5;
	
	float menuItemHeight = 40;
	float drawOffset = choose * menuItemHeight;
	position = D3DXVECTOR3((windowWidth/2 - 190), (windowHeight/4 + 80) + drawOffset, 0.0);

	if (latch && time > previousTime)
	{
		previousTime = previousTime + 10;
		latch = !latch;
	}

	if (!latch && time > previousTime)
	{
		previousTime = previousTime + 10;
		latch = !latch;
	}
	
	d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXVECTOR2 scaling(1.0f,1.0f);
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0.0,NULL);
	d3dSprite->SetTransform(&mat);
	if (latch)
		d3dSprite->Draw(arrowSprite2, &srcRect, &centre, &position, colour);
	else
		d3dSprite->Draw(arrowSprite, &srcRect, &centre, &position, colour);
	d3dSprite->End();
	return 0;
}
