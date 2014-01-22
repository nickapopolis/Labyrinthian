#include "StdAfx.h"
#include "GameMenu.h"
#include "nuss_util.h"

Selector* GameMenu::selector = NULL;
bool GameMenu::wpressed = false;
bool GameMenu::spressed = false;

GameMenu::GameMenu(IDirect3DDevice9 *d3dDev, char* name, char* items, int size, LPD3DXFONT &font, int windowWidth, int windowHeight, char* backName)
{
	this->menuName = name;
	this->menuFont = &font;
	this->menuItems = items;
	this->menuSize = size;
	this->backgroundName = backName;
	selected = 0;
	backgroundImage = NULL;
	titleImage = NULL;
	LONG x, y;

	x = windowWidth / 2;
	y = 0;
	
	//title
	titleBox.top = y+80;
	titleBox.left = x-300;
	titleBox.bottom = y+180;
	titleBox.right = x+300;

	y = windowHeight /4;
	//contents
	bodyBox.top = y+80;
	bodyBox.left = x-120;
	bodyBox.bottom = y+600;
	bodyBox.right = x+300;

	initImages(d3dDev, backgroundName, menuName);
}

int GameMenu::initImages(IDirect3DDevice9 *d3dDev, char * backgroundName, char * menuName)
{
	int rc = 0;
	rc = D3DXCreateTextureFromFileEx(d3dDev, 
		backgroundName, // file name
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
		&backgroundImage);

	rc = D3DXCreateTextureFromFileEx(d3dDev, 
		menuName, // file name
		400,			// width
		100,			// height
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
		&titleImage);

	if (rc != D3D_OK) {
		rc = 1;
		backgroundImage = NULL;
	} else {
		rc = 0;
	}

	return rc;
}

GameMenu::~GameMenu(void)
{
	SAFE_RELEASE(backgroundImage);
	SAFE_RELEASE(titleImage);
}
int  GameMenu::initializeTexture(IDirect3DDevice9 *d3dDev)
{
	int rc = 0;
	selector =  new Selector(d3dDev);
	if (selector == NULL)
		rc = 1;

	return rc;
}
int GameMenu::getSelected()
{
	return selected;
}
void GameMenu::render(ID3DXSprite *d3dSprite, long gameTime, int windowWidth, int windowHeight)
{
	D3DXVECTOR3 centre;
	D3DXVECTOR3 position, titlePos;
	centre = D3DXVECTOR3(0.0,0.0,0.0);
	position = D3DXVECTOR3(0.0, 0.0, 0.0);
	titlePos = D3DXVECTOR3((float)windowWidth/2 - 200, 50.0, 0.0);
	D3DCOLOR colour = D3DCOLOR_RGBA(255,255,255,255);
	float scaleX = (float)windowWidth / 512.0f;
	float scaleY = (float)windowHeight / 512.0f;

	d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR2 scaling(scaleX, scaleY);
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0.0,NULL);
	d3dSprite->SetTransform(&mat);
	d3dSprite->Draw(backgroundImage, NULL, &centre, &position, colour);

	D3DXVECTOR2 scaling2(1.0f, 1.0f);
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling2,NULL,0.0,NULL);
	d3dSprite->SetTransform(&mat);
	d3dSprite->Draw(titleImage, NULL, &centre, &titlePos, colour);

	d3dSprite->End();
	(*menuFont)->DrawText(NULL, menuItems, -1, &bodyBox, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 0));
	selector->render(d3dSprite, selected, gameTime, windowWidth, windowHeight); 
}
void GameMenu::handleInput(gameIO *mInput)
{
	if(mInput->keyboardPressed(DIK_W)){
		if(wpressed==false){
			wpressed = true;
		}
	}
	else if(wpressed==true){
		selected--;
		if(selected<0){
			selected =menuSize -1;
		}
		else if(selected>= menuSize){
			selected=0;
		}
		wpressed = false;
	}
	if(mInput->keyboardPressed(DIK_S)){
		if(spressed==false){
			spressed = true;
		}
	}
	else if(spressed==true){
		selected++;
		if(selected<0){
			selected = menuSize-1;
		}
		else if(selected>= menuSize){
			selected=0;
		}
		spressed = false;
	}
}

void GameMenu::setMenuItems(char* m)
{
	this->menuItems = m;
}

void GameMenu::setMenuSize(int s)
{
	this->menuSize = s;
}

int GameMenu::getMenuSize(void)
{
	return(this->menuSize);
}

void GameMenu::releaseTexture()
{
	delete selector;
}