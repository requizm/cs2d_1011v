#ifndef STRUCTS_H
#define STRUCTS_H


#include <Windows.h>

struct Offsets
{
	DWORD basePlayer = 0x496E5C;
	DWORD screen = 0x4F9F28;

	DWORD entityList = 0x4863AC;
	DWORD nextEntity = 0xC;
	DWORD entityPlayer = 0x8;

	DWORD screenWidth = 0x4972AC;
	DWORD screenHeight = 0x4972B0;

	DWORD id = 0x8; //also check if in match
	DWORD xpos = 0x1D4;
	DWORD ypos = 0x1D8;
	DWORD xrot = 0x1DC;
	DWORD yrot = 0x1E0;
	DWORD team = 0x1C4;  // t=1     ct=2
	DWORD isLive = 0x220;  // live = 0   
	DWORD mouseX = 0x18;
	DWORD mouseY = 0x1C;
	DWORD healthptr = 0x1F4;


	DWORD flash_01 = 0x27493C;
	DWORD flash_02 = 0x2747FA;


	//DWORD in_match = 0x496A94;  // 1 - 0


	DWORD inputLetterPtr = 0x496d98;

	DWORD chatPtr = 0x496FE4;
	DWORD chatStatus = 0x0; //on = 1, off = 0
	DWORD chatSize = 0x8;
	DWORD txtPointer = 0x4;

	DWORD txtSize = 0x8;
	DWORD txtStart = 0xC;


	DWORD readmouseX = 0x496D14;
	DWORD readmouseY = 0x496D18;
	DWORD cameraPosX = 0x497DA4;
	DWORD cameraPosY = 0x497DA8;

} offsets;

struct Functions
{
	DWORD GetHealth = 0x1C1CA0;
	//DWORD ShootEnemy = 0x2C0736;
	//DWORD PosToCell = 0x3B5F30;
	/*DWORD SendMessageToChat = 0x2CA80A;
	DWORD msg_01 = 0x191270;
	DWORD msg_02 = 0x191130;
	DWORD msg_03 = 0x191200;*/
	//DWORD GetEnemyFromID = 0x23C02A; // maybe 0x488028
	//DWORD Move = 0x23FFD2;
	//DWORD ReloadAmmo = 0x2BEABD;
} functions;



struct Entity
{
	DWORD baseAdress = 0x0;
	float xPos = 0.0f;
	float yPos = 0.0f;
	int team = 0;
	int isLive = 0;
	int hp;

} Player;

struct screen
{
	DWORD mouseX = 0x0;
	DWORD mouseY = 0x0;
	DWORD baseAdress = 0x0;
} Screen;

struct sVector2
{
	float x = 0;
	float y = 0;
};

#endif // !STRUCTS_H