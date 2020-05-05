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
	DWORD basePlayer = 0x496E0C;
	DWORD screen = 0x4FA020;

	DWORD entityList = 0x486330;
	DWORD nextEntity = 0xC;
	DWORD entityPlayer = 0x8;

	DWORD screenWidth = 0x49725C;
	DWORD screenHeight = 0x497260;

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


	DWORD flash = 0x2C5404;  // 0 = anti-flash                 1 = flash


	DWORD scope_01 = 0x34C858;  // 1 = enable level 1 scope     
	DWORD scope_02 = 0x34C85D;  // 2 = enable level 2 scope




	DWORD readmouseX = 0x496CD4;
	DWORD readmouseY = 0x496CD8;
	DWORD cameraPosX = 0x497D54;
	DWORD cameraPosY = 0x497D58;


	DWORD mapObj = 0x497DFC;
	DWORD tileCount = 0x14;
	DWORD mapYCount = 0x18;
	DWORD first_TileType = 0x1C;

	DWORD tilePixelSize = 0x4B26C0;
} offsets;

struct Functions
{
	DWORD GetHealth = 0x1C1CB2;
	//DWORD ShootEnemy = 0x2C0731;
	DWORD PosToCell = 0x3B5F2C;
	//DWORD Move = 0x23FFC7;
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