#include "StdAfx.h"
#include "myGame.h"
#include <stdio.h>
#include "nuss_util.h"
#include "BestTime.h"
#include "LevelCompleteScreen.h"
#include <math.h>

using namespace std;

/*************************************************************************/
//callback function for any collision where a report is generated
class ContactReport : public NxUserContactReport
	{
		void onContactNotify(NxContactPair& pair, NxU32 events)
		{

			NxActor *actor1 = pair.actors[0];
			NxActor *actor2 = pair.actors[1];
			Player* player;

			if(actor1->getGroup() == PLAYER)
			{

				player = static_cast<Player*>(actor1->userData);
				if(actor2->getGroup() == TRAP)
				{
					player->adjustHealth(-1);
				}
				else if (actor2->getGroup() == FLOOR)
				{
					FloorPanel *floor = static_cast<FloorPanel*>(actor2->userData);
					floor->playerSteppedOnFloor();
				}
				else if(actor2->getGroup() == END_OBJECT)
				{
				//	player->hasBeatLevel = true;
				}
			}
			else if(actor2->getGroup() == PLAYER)
			{

				player = static_cast<Player*>(actor2->userData);
				if(actor1->getGroup() == TRAP)
				{

					player->adjustHealth(-1);
				}
				else if (actor1->getGroup() == FLOOR)
				{

					FloorPanel *floor = static_cast<FloorPanel*>(actor1->userData);
					floor->playerSteppedOnFloor();
				}
				else if(actor1->getGroup() == END_OBJECT)
				{
				//	player->hasBeatLevel = true;
				}
			}
		}
	} myContactReport;

myGame::myGame(HINSTANCE hInstance, char* gameName):gameApp(hInstance, gameName)
{
	currentLevel = NULL;
	fontCourier = NULL;
	preambleFont = NULL;
	returnPressed = false;
	light = NULL;
}

myGame::~myGame(void)
{
	SAFE_RELEASE(fontCourier);
	SAFE_RELEASE(preambleFont);
	
	delete levels[0];
	delete levels[1];
	delete levels[2];

	if(mainMenu != NULL)
		delete mainMenu;

	if( pauseMenu!= NULL)
		delete pauseMenu;
	if( creditMenu!= NULL)
		delete creditMenu;
	if( helpMenu!= NULL)
		delete helpMenu;
	if( levelSelectionMenu!= NULL)
		delete levelSelectionMenu;

	GameMenu::releaseTexture();
}


/*************************************************************************/

int myGame::initGame(void)
{

	int rc = 0;

	hud = new Hud(md3dDev, getWndWidth(), getWndHeight());

	bestTimes = new BestTime();
	bestTimes->loadBestTimes();

	rc = initTextures();
	rc = initFonts();
	rc = initMenus();
	rc = initLight();

	currentMenu = mainMenu;

	levels = (Level**)malloc(3*sizeof(Level*));
	levels[0] = new Level(md3dDev, &myContactReport, 1, 32);
	levels[1] = new Level(md3dDev, &myContactReport, 2, 32);
	levels[2] = new Level(md3dDev, &myContactReport, 3, 32);

	player = new Player(md3dDev, Vector(0.0f, 0.0f, 0.0f), 0.5f);

	return 0;
}

int myGame::initTextures(void)
{
	int rc = 0;
	//initialize textures
	rc = Wall::initializeTexture(md3dDev);
	rc = FloorPanel::initializeTexture(md3dDev);
	rc = GameMenu::initializeTexture(md3dDev);
	rc = Player::initializeTexture(md3dDev);
	rc = RollingBall::initializeTexture(md3dDev);
	return rc;
}
int myGame::initFonts(void)
{
	int rc = D3DXCreateFont(md3dDev,
						10, // height of font
						0,	// use the default width
						FW_MEDIUM+50,
						//FW_NORMAL,	// normal font weight
						1, // no Mipmap
						FALSE, // italic
						DEFAULT_CHARSET, // default character set
						OUT_DEFAULT_PRECIS, // default precision
						DEFAULT_QUALITY, // default quality
						DEFAULT_PITCH ||FF_DONTCARE, // more defaults...
						"Courier",	// typeface “Courier"
						&fontCourier); 

	rc = D3DXCreateFont(md3dDev,
					40, // height of font
					0,	// use the default width
					FW_BOLD,
					//FW_MEDIUM+50,
					//FW_NORMAL,	// normal font weight
					1, // no Mipmap
					FALSE, // italic
					DEFAULT_CHARSET, // default character set
					OUT_DEFAULT_PRECIS, // default precision
					DEFAULT_QUALITY, // default quality
					DEFAULT_PITCH ||FF_DONTCARE, // more defaults...
					"Aharoni",	// typeface courier"
					&preambleFont); 
	return rc;
}
int myGame::initMenus(void)
{
	int rc = 0;
	mainMenu = new GameMenu(md3dDev, "res\\title1.png", "Level Selection\nHelp\nCredits\nQuit", 4, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");
	if(bestTimes->getnumLevelUnlocked() == 1)
		levelSelectionMenu = new GameMenu(md3dDev, "res\\title2.png", "Level 1\nBack", 2, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");
	else if(bestTimes->getnumLevelUnlocked() == 2)
		levelSelectionMenu = new GameMenu(md3dDev, "res\\title2.png", "Level 1\nLevel 2\nBack", 3, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");
	else if(bestTimes->getnumLevelUnlocked() == 3)
		levelSelectionMenu = new GameMenu(md3dDev, "res\\title2.png", "Level 1\nLevel 2\nLevel 3\nBack", 4, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");
	winMenu = new LevelCompleteScreen(md3dDev, "res\\title3.png", "Main Menu\nQuit", 2, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");
	loseMenu = new GameMenu(md3dDev, "res\\title7.png", "Main Menu\nQuit", 2, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");
	pauseMenu = new GameMenu(md3dDev, "res\\title4.png", "Continue\nMain Menu", 2, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");
	creditMenu = new GameMenu(md3dDev, "res\\title5.png", "Main Menu\n\nAaron Crozman\nNicholas Tierney\nTyler Haskell\nZiyao Zhou", 1, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");
	helpMenu = new GameMenu(md3dDev, "res\\title6.png", "Main Menu\n\nMovement: W-A-S-D\nPause: P", 1, preambleFont, getWndWidth(), getWndHeight(), "res\\stonewall.png");

	if(mainMenu == NULL || levelSelectionMenu == NULL || winMenu == NULL || pauseMenu == NULL || creditMenu == NULL || helpMenu == NULL)
		rc = 1;

	return rc;
}

int myGame::initLight()
{
	light = &D3DLIGHT9();
    ZeroMemory(light, sizeof(light));
	light->Phi = D3DXToRadian(90.0f);    // set the outer cone to 30 degrees
    light->Theta = D3DXToRadian(50.0f);    // set the inner cone to 10 degrees
    light->Falloff = 1.0f;    // use the typical falloff
    light->Type = D3DLIGHT_SPOT;    // make the light type 'point light'
    light->Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
    light->Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
    light->Range = 100.0f;    // a range of 100
    light->Attenuation0 = 0.0f;    // no constant inverse attenuation
    light->Attenuation1 = 0.125f;    // only .125 inverse attenuation
    light->Attenuation2 = 0.0f;    // no square inverse attenuation
    md3dDev->SetLight(0, light);
    md3dDev->LightEnable(0, TRUE);

	return light == NULL;
}
//initial background music
int myGame::initAudioBackground(void)
{
	audioPlayer->setFileName("start.wav");
	audioPlayer->playSound();
	return 0;
}
int myGame::initAudioEnter(void)
{
	audioPlayer->setFileName("enter.wav");
	audioPlayer->playSound(1000);
	return 0;
}
int myGame::initAudioSelect(void)
{
	audioPlayer->setFileName("selector.mav");
	audioPlayer->playSound();
	return 0;
}
int myGame::initAudioWalk(void)
{
	/*audioPlayer->setFileName("step.mav");
	audioPlayer->playSound(4000);*/
	return 0;
}
int myGame::updateGameState(long time)
{
	int rc = 0;
	// poll the input
	mInput->poll();


	if(currentLevel == NULL || paused)
	{
		currentMenu->handleInput(mInput);
		int selection = currentMenu->getSelected();
		
		if(mInput->keyboardPressed(DIK_RETURN)){
			if(returnPressed==false){
				returnPressed = true;
				initAudioEnter();
			}
		}
		else if(returnPressed==true){
			returnPressed = false;

			//get menu selection
			if(currentMenu == mainMenu)
				mainMenuChooseOption(selection);
			else if(currentMenu == winMenu)
				winMenuChooseOption(selection);
			else if(currentMenu == pauseMenu)
				pauseMenuChooseOption(selection);
			else if(currentMenu == creditMenu)
				creditMenuChooseOption(selection);
			else if(currentMenu == helpMenu)
				helpMenuChooseOption(selection);
			else if(currentMenu == levelSelectionMenu)
				levelMenuChooseOption(selection);
			else if(currentMenu == loseMenu)
				winMenuChooseOption(selection);
		}
	}
	else
	{
		//update the physics engine
		PhysicsEngine::simulate(currentLevel->getScene(), (float)time);
		player->updateState(time, mInput);
		currentLevel->updateState(time);
		hud->setLives(player->getHealth());

		if(player->hasBeatLevel == true || player->getHealth() == 0)
			endGame();

		if (mInput->keyboardPressed(DIK_E)) {
			endGame();
		}

		if (mInput->keyboardPressed(DIK_P)) {
			paused = true;
			currentMenu = pauseMenu;
			audioPlayer->pause();
		}

		updateCameraPosition();
	}


	// check if escape key was pressed
	if (mInput->keyboardPressed(DIK_ESCAPE)) {
		rc = 1;
	}
	
	return(rc);
}

/*************************************************************************/



int myGame::renderFrame(long time)
{

	md3dDev->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0,0, 0), 1.0f, 0);
	md3dDev->BeginScene();

	if(currentLevel == NULL || paused )
	{
		//draw menu
		currentMenu->render(md3dSprite, time, getWndWidth(), getWndHeight());
	}
	else
	{
		hud->render(md3dSprite, time);
		//draw game
		
		currentLevel->draw(md3dDev);
		//player->drawObject(md3dDev);
	}

	md3dDev->EndScene();
	md3dDev->Present(0, 0, 0, 0);

	return 0;
}


void myGame::mainMenuChooseOption(int choose){
	if(choose==0){
		currentMenu = levelSelectionMenu;
	}
	else if(choose==1){
		currentMenu = helpMenu;
	}
	else if(choose==2){
		currentMenu = creditMenu;
	}
	else if(choose==3){
		exit(0);
	}
}

void myGame::levelMenuChooseOption(int choose)
{
	if(levelSelectionMenu->getMenuSize() == 4)
	{
		if(choose==0){
			switchToLevel(levels[0]);
			bestTimes->loadBestTimes();
			hud->setBestTime(bestTimes->getLevel1BT());
			hud->setLevelNum(1);
			hud->resetTime();
			paused = false;
		}
		else if(choose==1){
			switchToLevel(levels[1]);
			bestTimes->loadBestTimes();
			hud->setBestTime(bestTimes->getLevel2BT());
			hud->resetTime();
			hud->setLevelNum(2);
			paused = false;
		}
		else if(choose==2){
			switchToLevel(levels[2]);
			bestTimes->loadBestTimes();
			hud->setBestTime(bestTimes->getLevel3BT());
			hud->resetTime();
			hud->setLevelNum(3);
			paused = false;
		}
		else if(choose==3){
			currentMenu = mainMenu;
		}
	}
	else if(levelSelectionMenu->getMenuSize() == 3)
	{
		if(choose==0){
			switchToLevel(levels[0]);
			bestTimes->loadBestTimes();
			hud->setBestTime(bestTimes->getLevel1BT());
			hud->setLevelNum(1);
			hud->resetTime();
			paused = false;
		}
		else if(choose==1){
			switchToLevel(levels[1]);
			bestTimes->loadBestTimes();
			hud->setBestTime(bestTimes->getLevel2BT());
			hud->resetTime();
			hud->setLevelNum(2);
			paused = false;
		}
		else if(choose==2){
			currentMenu = mainMenu;
		}
	}
	else if(levelSelectionMenu->getMenuSize() == 2)
	{
		if(choose==0){
			switchToLevel(levels[0]);
			bestTimes->loadBestTimes();
			hud->setBestTime(bestTimes->getLevel1BT());
			hud->setLevelNum(1);
			hud->resetTime();
			paused = false;
		}
		else if(choose==1){
			currentMenu = mainMenu;
		}
	}
}

void myGame::winMenuChooseOption(int choose)
{
	if(choose==0){
		currentMenu = mainMenu;
	}
	else if(choose==1){
		exit(0);
	}
}

void myGame::loseMenuChooseOption(int choose)
{
	if(choose==0){
		currentMenu = mainMenu;
	}
	else if(choose==1){
		exit(0);
	}
}

void myGame::pauseMenuChooseOption(int choose)
{
	if(choose==0){
		paused = false;
		initAudioBackground();
	}
	else if(choose==1){
		currentLevel = NULL;
		currentMenu = mainMenu;
	}
}

void myGame::creditMenuChooseOption(int choose)
{
	if(choose==0)
		currentMenu = mainMenu;
}

void myGame::helpMenuChooseOption(int choose)
{
	if(choose==0)
		currentMenu = mainMenu;
}

void myGame::switchToLevel(Level *level)
{
	if(currentLevel != NULL)
	{
		//must remove the player actor from the scene 
		currentLevel->getScene()->releaseActor(*player->getActor());
	}
	currentLevel = level;
	//Vector sp = Vector( 8, 1.0f , 0.0f);
	Vector sp = currentLevel->getStartPos();
	player->acquireActor(currentLevel->getScene(), sp, 0.5f);
	currentLevel->getScene()->setActorPairFlags(*player->getActor(), *currentLevel->getEndObject()->actor, NX_NOTIFY_ON_TOUCH);
	initAudioBackground();
	//initAudioSelect();
	//initAudioEnter();
	initAudioWalk();
}

void myGame::updateCameraPosition()
{
	//set camera position to the players position
	Vector camPos = player->getPos();

	//shift camera up
	camPos.y+= 0.25;

	//move the camera behind the players head
	camPos = camPos - (Vector(cos(player->getOrientation()), 0 , sin(player->getOrientation()))* 0.5f);

	//set the camera to look in the direction they are facing
	Vector lookAt = camPos + Vector(cos(player->getOrientation()), 0 , sin(player->getOrientation()));
	
	//updates directx camera
	camera->setViewMatrix(camPos.asD3DXVECTOR3(), lookAt.asD3DXVECTOR3(), Vector(0,1,0).asD3DXVECTOR3());
	
	
	md3dDev->GetLight(0, light);
	light->Position = camPos.asD3DXVECTOR3();
	Vector dir = (lookAt-camPos);
	dir.normalize();
	light->Direction = dir.asD3DXVECTOR3();
    md3dDev->SetLight(0, light);
    //md3dDev->LightEnable(0, TRUE);

}

void myGame::endGame()
{
	if(hud->getLives() > 0)
	{
		if(hud->getLevelNum() == 1)
		{
			bestTimes->setLevel1BT(hud->getTimeSpent());
			if(bestTimes->getnumLevelUnlocked() == 1)
			{
				levelSelectionMenu->setMenuItems("Level 1\nLevel 2\nBack");
				levelSelectionMenu->setMenuSize(3);
				bestTimes->setnumLevelUnlocked(2);
			}
			bestTimes->saveBestTimes();
		}
		else if(hud->getLevelNum() == 2)
		{
			bestTimes->setLevel2BT(hud->getTimeSpent());
			if(bestTimes->getnumLevelUnlocked() == 2)
			{
				levelSelectionMenu->setMenuItems("Level 1\nLevel 2\nLevel 3\nBack");
				levelSelectionMenu->setMenuSize(4);
				bestTimes->setnumLevelUnlocked(3);
			}
			bestTimes->saveBestTimes();
		}
		else if(hud->getLevelNum() == 3)
		{
			bestTimes->setLevel3BT(hud->getTimeSpent());
			bestTimes->saveBestTimes();
		}
		winMenu->setPlayTime(hud->getTimeSpent());
		winMenu->setDeaths(3 - hud->getLives());
		currentLevel = NULL;
		paused = true;
		hud->setLives(3);
		currentMenu = winMenu;
	}
	else
	{
		hud->setLives(3);
		currentLevel = NULL;
		paused = true;
		currentMenu = loseMenu;
	}
	player->adjustHealth(3);
}