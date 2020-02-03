#ifndef STRUCTS_H
#define STRUCTS_H

#include <Windows.h>
#include <math.h>

struct fVector2
{
	float x = 0;
	float y = 0;

	fVector2() {

	}
	fVector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	fVector2 operator-(fVector2 s)
	{
		return fVector2(this->x - s.x, this->y - s.y);
	}

	fVector2 operator+(fVector2 s)
	{
		return fVector2(this->x + s.x, this->y + s.y);
	}


	fVector2 operator*(int a)
	{
		return fVector2(this->x * a, this->y * a);
	}

	float Magnitude()
	{
		return sqrtf(pow(this->x, 2) + pow(this->y, 2));
	}

	fVector2 Normalize()
	{
		float mag = this->Magnitude();
		return fVector2(this->x / mag, this->y / mag);
	}
};

struct iVector2
{
	int x = 0;
	int y = 0;
	iVector2()
	{

	}
	iVector2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	iVector2 operator+(fVector2 s)
	{
		return iVector2((int)(this->x + s.x), (int)(this->y + s.y));
	}

	iVector2 operator+(iVector2 s)
	{
		return iVector2(this->x + s.x, this->y + s.y);
	}
};


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
	DWORD xPos = 0x1D4;
	DWORD yPos = 0x1D8;
	//DWORD xrot = 0x1DC;
	//DWORD yrot = 0x1E0;
	DWORD team = 0x1C4;  // t=1     ct=2
	DWORD isLive = 0x220;  // live = 0   
	DWORD mouseX = 0x18;
	DWORD mouseY = 0x1C;
	DWORD healthPtr = 0x1F4;
	DWORD ownerBomb = 0x258;
	DWORD weaponPtr = 0x24C;
	DWORD weaponId = 0xC;
	DWORD weaponMode = 0x1C;


	DWORD flash_01 = 0x27493C;
	DWORD flash_02 = 0x2747FA;

	DWORD scope_01 = 0x34C864;
	DWORD scope_02 = 0x34C888;


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


	DWORD mapObj = 0x497E4C;
	DWORD tileCount = 0x14;
	DWORD mapYCount = 0x18;
	DWORD first_TileType = 0x1C;

	DWORD tilePixelSize = 0x4CF274;
} offsets;

struct Functions
{
	DWORD GetHealth = 0x1C1CA0;
	//DWORD ShootEnemy = 0x2C0736;
	DWORD PosToCell = 0x3B5F30;
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
	fVector2 pos = fVector2(0.0f, 0.0f);
	float rotation = 0.0f;
	int team = 0;
	int isLive = 0;
	int hp = 100;
	bool bomb = false;
	int weaponID = 0;
	int weaponMode = 0;

} Player;

struct screen
{
	DWORD mouseX = 0x0;
	DWORD mouseY = 0x0;
	DWORD baseAdress = 0x0;
} Screen;



#endif // !STRUCTS_H