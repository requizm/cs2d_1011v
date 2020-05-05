#pragma comment(lib,"OpenGL32.lib")
#include "Structs.h"
#include "Functions.h"
#include "glDraw.h"
#include <vector>
#include <Windows.h>
#include <string>


DWORD moduleBase;
DWORD entityList;
std::vector<Entity> enemies;
int screenWidth, screenHeight;
typedef fVector2 Vector2f;
typedef iVector2 Vector2i;
bool aim = false;
bool esp = true;
bool flash = false;
bool scope = false;


//void SendMessageToChat(std::string msg);
void Initiate();
void UpdatePlayer();
void UpdateEnemies();
void UpdateEntity(Entity* entity);
int CalculateClosestEnemy();
int SmartCalculateClosestEnemy();
int ChangeFlash();
int ChangeScope();
void Render();
std::string BoolToString(bool a);
void WriteStatus();
Vector2f ScreenToWorld(Vector2f pos);
Vector2i ScreenToWorld(Vector2i pos);
Vector2f WorldToScreen(Vector2f pos);

twglSwapBuffers owglSwapBuffers;

typedef int(__cdecl* fnGetHealth)(DWORD health_ptr);

//typedef int* (__cdecl* fnReloadAmmo)(int player_id, int b, int c);  // b = 1, c = 1
//typedef int* (__cdecl* fnMove)(DWORD player_ptr, float x, float y, int d, int collision);  // d = 1
//typedef DWORD(__cdecl* fnGetEnemyFromID)(int id); 
typedef int(__cdecl* fnPosToCell)(double a1); //for player: ((double pos - 12.0) / tilePixelSize)
											  //for random pos: ((double pos) / tilePixelSize)


/**
typedef int* (__cdecl* fnSendMessage)(int a1_1, int chat_status_1, DWORD text_pointer, int a4, int a5_0); // a1 = 1, chat_status = 1, a4 = 1, a5 = 0
typedef DWORD(__cdecl* fnMessage_01)(DWORD txtptr, DWORD chatptr_size, DWORD textptr_size); //sub_591270
typedef DWORD(__cdecl* fnMessage_02)(__int16 asci_no); //sub_591130
typedef DWORD(__cdecl* fnMessage_03)(DWORD a1, DWORD a2); //sub_591200
**/

//typedef int (__cdecl* fnEnemyShot)(int target_id, int source_id, int damage, int a4, int a5, int a6, int a7); a4 = 0, a5 = -1, a6 = 0, a7 = 0 




//fnMove Move;
//fnGetEnemyFromID GetEnemyFromID;
fnPosToCell PosToCell;
//fnSendMessage SendMessageChat;
//fnMessage_01 msg_01;
//fnMessage_02 msg_02;
//fnMessage_03 msg_03;
//fnEnemyShot EnemyShot;
fnGetHealth GetHealth;

void UpdatePlayer()
{
	UpdateEntity(&Player);
}


void UpdateEnemies()
{
	enemies.clear();
	DWORD adr = *(DWORD*)(entityList + offsets.entityPlayer);
	DWORD nextentity = *(DWORD*)(entityList + offsets.nextEntity);
	while (true)
	{
		Entity enemy;
		enemy.baseAdress = *(DWORD*)(nextentity + offsets.entityPlayer);

		UpdateEntity(&enemy);
		if (enemy.baseAdress != Player.baseAdress && enemy.team != Player.team)
		{
			enemies.push_back(enemy);
		}

		DWORD nxt = *(DWORD*)(nextentity + offsets.nextEntity);

		if (nxt == adr)
		{
			break;
		}
		nextentity = nxt;
	}
}

void UpdateEntity(Entity* entity)
{
	entity->team = *(int*)(entity->baseAdress + offsets.team);
	entity->isLive = *(int*)(entity->baseAdress + offsets.isLive);
	if (entity->isLive == 0) // 0 = live
	{
		entity->pos.x = *(float*)(entity->baseAdress + offsets.xPos);
		entity->pos.y = *(float*)(entity->baseAdress + offsets.yPos);
		entity->hp = GetHealth(*(DWORD*)(entity->baseAdress + offsets.healthPtr));
		//entity->rotation = *(float*)(entity->baseAdress + offsets.xrot);
		entity->bomb = *(int*)(entity->baseAdress + offsets.ownerBomb);
		if (entity->baseAdress == *(DWORD*)(moduleBase + offsets.basePlayer))
		{
			entity->weaponID = *(int*)(*(DWORD*)(entity->baseAdress + offsets.weaponPtr) + offsets.weaponId);
			entity->weaponMode = *(int*)(*(DWORD*)(entity->baseAdress + offsets.weaponPtr) + offsets.weaponMode);
		}


	}
}

BOOL _stdcall SwapBuffersHook(HDC hDC)
{
	Render();
	return owglSwapBuffers(hDC);
}


DWORD WINAPI OverwriteValues()
{
	HookC("wglSwapBuffers", (uintptr_t&)owglSwapBuffers, &SwapBuffersHook);
	return 0x0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		Initiate();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OverwriteValues, NULL, NULL, NULL);
		break;
	}
	return TRUE;
}

int CalculateClosestEnemy()
{
	if (enemies.size() > 0)
	{
		float maxLen = 340282346638528859811704183484516925440.0000000000000000f; //max distance 
		int targetIndex = 0;
		for (std::vector<int>::size_type i = 0; i != enemies.size(); i++)
		{
			if (Player.isLive == 0 && enemies[i].isLive == 0)
			{
				Vector2f dist = enemies[i].pos - Player.pos;
				if (dist.x < 0) dist.x *= -1; if (dist.y < 0) dist.y *= -1; //abs
				float len = dist.Magnitude();
				if (i == 0)
				{
					maxLen = len;
				}
				else if (len < maxLen)
				{
					maxLen = len;
					targetIndex = i;
				}
			}
		}
		return targetIndex;
	}
	return -1;
}

int SmartCalculateClosestEnemy()
{
	if (enemies.size() > 0 && Player.isLive == 0)
	{
		DWORD mapPtr = *(DWORD*)(moduleBase + offsets.mapObj);
		int tileYCount = *(int*)(mapPtr + offsets.mapYCount);
		DWORD firstTileAddress = mapPtr + offsets.first_TileType;
		float maxLen = 340282346638528859811704183484516925440.0000000000000000f; //max distance 
		int targetIndex = 0;
		int sz = enemies.size();
		for (std::vector<int>::size_type i = 0; i != sz; i++)
		{
			if (enemies[i].isLive == 0)
			{
				Vector2f dist = enemies[i].pos - Player.pos;
				if (dist.x < 0) dist.x *= -1; if (dist.y < 0) dist.y *= -1; //abs
				float len = dist.Magnitude();
				Vector2f dist_n = enemies[i].pos - Player.pos;

				//double d_x = sin((double)Player.rotation * 0.0174532925199433);
				//double d_y = -cos((double)Player.rotation * 0.0174532925199433);

				Vector2f direction = Vector2f(dist_n.x / len, dist_n.y / len);
				direction = direction * 20;
				Vector2i temp_pos = Vector2i((int)Player.pos.x, (int)Player.pos.y);
				bool wallFound = false;
				if (direction.x > 0)
				{
					if (direction.y > 0)
					{
						while (temp_pos.x <= enemies[i].pos.x || temp_pos.y <= enemies[i].pos.y)
						{
							//std::cout << "dusmanin sol ustundeyim" << std::endl;
							temp_pos = temp_pos + direction;
							float div = *(float*)(moduleBase + offsets.tilePixelSize);
							int x = PosToCell((double)((temp_pos.x) / div));
							int y = PosToCell((double)((temp_pos.y) / div));
							int type = *(int*)(firstTileAddress + 4 * ((x * tileYCount) + y));
							if (type == 1 || type == 3)
							{
								wallFound = true;
								break;
							}
						}
					}
					else
					{
						while (temp_pos.x < enemies[i].pos.x || temp_pos.y > enemies[i].pos.y)
						{
							//std::cout << "dusmanin sol altindayim" << std::endl;
							temp_pos = temp_pos + direction;
							float div = *(float*)(moduleBase + offsets.tilePixelSize);
							int x = PosToCell((double)((temp_pos.x) / div));
							int y = PosToCell((double)((temp_pos.y) / div));
							int type = *(int*)(firstTileAddress + 4 * ((x * tileYCount) + y));
							if (type == 1 || type == 3)
							{
								wallFound = true;
								break;
							}
						}

					}
				}
				else
				{
					if (direction.y > 0)
					{
						while (temp_pos.x >= enemies[i].pos.x || temp_pos.y <= enemies[i].pos.y)
						{
							//std::cout << "dusmanin sag ustundeyim" << std::endl;
							temp_pos = temp_pos + direction;
							float div = *(float*)(moduleBase + offsets.tilePixelSize);
							int x = PosToCell((double)((temp_pos.x) / div));
							int y = PosToCell((double)((temp_pos.y) / div));
							int type = *(int*)(firstTileAddress + 4 * ((x * tileYCount) + y));
							if (type == 1 || type == 3)
							{
								wallFound = true;
								break;
							}
						}
					}
					else
					{
						while (temp_pos.x > enemies[i].pos.x || temp_pos.y > enemies[i].pos.y)
						{
							//std::cout << "dusmanin sag altindayim" << std::endl;
							temp_pos = temp_pos + direction;
							float div = *(float*)(moduleBase + offsets.tilePixelSize);
							int x = PosToCell((double)((temp_pos.x) / div));
							int y = PosToCell((double)((temp_pos.y) / div));
							int type = *(int*)(firstTileAddress + 4 * ((x * tileYCount) + y));
							if (type == 1 || type == 3)
							{
								wallFound = true;
								break;
							}
						}
					}
				}

				if (wallFound && i == 0)
				{
					targetIndex = -1;
				}
				else if (len < maxLen && !wallFound)
				{
					maxLen = len;
					targetIndex = i;
				}
			}
		}
		return targetIndex;
	}
	return -1;
}

int ChangeFlash()
{
	DWORD address = moduleBase + offsets.flash;
	BYTE value = *(BYTE*)(address);

	if ((int)value == 0x01 && !flash)
	{
		value = 0x00;
	}
	else if ((int)value == 0x00 && flash)
	{
		value = 0x01;
	}
	else
	{
		std::cout << "error on flash func" << std::endl;
		return 0;
	}

	DWORD OldProtection = NULL;
	VirtualProtect(reinterpret_cast<void*>(address), 1, PAGE_EXECUTE_READWRITE, &OldProtection);
	*reinterpret_cast<PBYTE>(address) = value;
	VirtualProtect(reinterpret_cast<void*>(address), 1, OldProtection, &OldProtection);


	return 1;
}

int ChangeScope()
{
	DWORD address = moduleBase + offsets.scope_01;
	DWORD address_2 = moduleBase + offsets.scope_02;
	BYTE value = *(BYTE*)(address);
	BYTE value_2 = *(BYTE*)(address_2);

	if ((int)value == 0x01 && (int)value_2 == 0x02 && !scope)
	{
		value = 0x05;
		value_2 = 0x05;
	}
	else if ((int)value == 0x05 && (int)value_2 == 0x05 && scope)
	{
		value = 0x01;
		value_2 = 0x02;
	}
	else
	{
		std::cout << "error on scope func" << std::endl;
		return 0;
	}

	DWORD OldProtection = NULL;
	VirtualProtect(reinterpret_cast<void*>(address), 1, PAGE_EXECUTE_READWRITE, &OldProtection);
	*reinterpret_cast<PBYTE>(address) = value;
	VirtualProtect(reinterpret_cast<void*>(address), 1, OldProtection, &OldProtection);

	OldProtection = NULL;
	VirtualProtect(reinterpret_cast<void*>(address_2), 1, PAGE_EXECUTE_READWRITE, &OldProtection);
	*reinterpret_cast<PBYTE>(address_2) = value_2;
	VirtualProtect(reinterpret_cast<void*>(address_2), 1, OldProtection, &OldProtection);

	return 1;
}

void Render()
{
	HDC currentHDC = wglGetCurrentDC();

	SetupOrtho();

	if (!font.bBuilt || currentHDC != font.hdc)
	{
		font.Build(16);
	}

	if (GetAsyncKeyState(VK_F5))
	{
		aim = !aim;
		WriteStatus();
		Sleep(200);
	}

	if (GetAsyncKeyState(VK_F6))
	{
		esp = !esp;
		WriteStatus();
		Sleep(200);
	}
	if (GetAsyncKeyState(VK_F7))
	{
		if (ChangeFlash() == 1) {
			flash = !flash;
			WriteStatus();
		}
		Sleep(200);
	}
	if (GetAsyncKeyState(VK_F8))
	{
		if (ChangeScope() == 1) {
			scope = !scope;
			WriteStatus();
		}
		Sleep(200);
	}

	Player.baseAdress = *(DWORD*)(moduleBase + offsets.basePlayer);
	int match = *(int*)(Player.baseAdress + offsets.id);
	if (match != 0)
	{

		screenWidth = *(int*)(moduleBase + offsets.screenWidth);
		screenHeight = *(int*)(moduleBase + offsets.screenHeight);

		Screen.baseAdress = *(DWORD*)(moduleBase + offsets.screen);
		Screen.mouseX = Screen.baseAdress + offsets.mouseX;
		Screen.mouseY = Screen.baseAdress + offsets.mouseY;


		entityList = *(DWORD*)((*(DWORD*)(moduleBase + offsets.entityList)) + 0x8);
		UpdatePlayer();
		UpdateEnemies();

		int index = SmartCalculateClosestEnemy();
		if (index != -1)
		{
			Entity target = enemies.at(index);
			if (Player.isLive == 0 && aim && target.isLive == 0)
			{
				Vector2f scr = Vector2f(screenWidth, screenHeight);
				Vector2f res = ScreenToWorld(scr);
				res.x /= 6; res.y /= 6;

				Vector2f dist_v = target.pos - Player.pos;

				float dist = dist_v.Magnitude();
				float maxDist = res.Magnitude();

				Vector2f direction = dist_v.Normalize();

				Vector2f result;
				if (dist > maxDist)
				{
					Vector2f ps = Vector2f(Player.pos.x + direction.x * 150, Player.pos.y + direction.y * 150);
					Vector2f e = WorldToScreen(ps);
					result = e;
				}
				else
				{
					Vector2f t = WorldToScreen(target.pos);
					result = t;
				}

				*(int*)Screen.mouseX = result.x;
				*(int*)Screen.mouseY = result.y;
			}
		}
	}

	if (Player.isLive == 0 && match != 0)
	{
		if (esp && enemies.size() > 0)
		{
			for (std::vector<int>::size_type i = 0; i != enemies.size(); i++)
			{
				if (enemies[i].isLive != 0 || Player.isLive != 0 || enemies[i].team == Player.team)
					continue;

				Vector2f t = WorldToScreen(enemies[i].pos);
				Vector2f e = WorldToScreen(Player.pos);

				if (enemies[i].hp > 70)
					DrawLine(e.x, e.y, t.x, t.y, rgb::green);
				else if (enemies[i].hp > 40)
					DrawLine(e.x, e.y, t.x, t.y, rgb::gray);
				else if (enemies[i].hp > 0)
					DrawLine(e.x, e.y, t.x, t.y, rgb::red);

				Vector2f dist_v = enemies[i].pos - Player.pos;
				Vector2f direction = dist_v.Normalize();
				Vector2f ps = Vector2f(Player.pos.x + direction.x * 100, Player.pos.y + direction.y * 100);
				Vector2f k = WorldToScreen(ps);
				renderText(std::to_string(enemies[i].hp), k.x, k.y);
				if (Player.team == 2 && enemies[i].bomb == 1)
				{
					ps = Vector2f(Player.pos.x + direction.x * 150, Player.pos.y + direction.y * 150);
					k = WorldToScreen(ps);
					renderText("BOMB!", k.x, k.y);
				}

			}
		}
		if (scope)
		{
			if (Player.weaponID == 34 || Player.weaponID == 35 || Player.weaponID == 36 || Player.weaponID == 37 || Player.weaponID == 31 || Player.weaponID == 33)
			{
				Vector2f clonePlayer = Player.pos;
				clonePlayer.y += 100;
				Vector2f e = WorldToScreen(clonePlayer);
				switch (Player.weaponMode)
				{
				case 0:
					renderText("Scope Vision: OFF", e.x, e.y);
					break;
				case 1:
					renderText("Scope Vision: ZOOM-1", e.x, e.y);
					break;
				case 2:
					renderText("Scope Vision: ZOOM-2", e.x, e.y);
					break;
				default: break;
				}
			}
		}
	}
}


void Initiate()
{
	moduleBase = getModuleBase("cs2d.exe");

	//Move = (fnMove)(moduleBase + functions.Move);
	PosToCell = (fnPosToCell)(moduleBase + functions.PosToCell);
	//EnemyShot = (fnEnemyShot)(moduleBase + functions.ShootEnemy);
	GetHealth = (fnGetHealth)(moduleBase + functions.GetHealth);


	WriteStatus();
}

std::string BoolToString(bool a)
{
	return a ? "TRUE" : "FALSE";
}

void WriteStatus()
{
	std::cout << std::string(100, '\n');

	std::string a = BoolToString(aim);
	std::string e = BoolToString(esp);
	std::string n = BoolToString(flash);
	std::string s = BoolToString(scope);

	std::cout << "[F5] Smart Aim : " << a << std::endl;
	std::cout << "[F6] Esp       : " << e << std::endl;
	std::cout << "[F7] Anti Flash: " << n << std::endl;
	std::cout << "[F8] No Scope  : " << s << std::endl;
}

Vector2f ScreenToWorld(Vector2f pos)
{
	Vector2f v, result;
	v.x = (float)(*(int*)(moduleBase + offsets.cameraPosX));
	v.y = (float)(*(int*)(moduleBase + offsets.cameraPosY));
	return (pos + v);
}

Vector2i ScreenToWorld(Vector2i pos)
{
	Vector2f v; Vector2i result;
	v.x = (float)(*(int*)(moduleBase + offsets.cameraPosX));
	v.y = (float)(*(int*)(moduleBase + offsets.cameraPosY));
	return (pos + v);
}

Vector2f WorldToScreen(Vector2f pos)
{
	Vector2f v;
	v.x = (float)(*(int*)(moduleBase + offsets.cameraPosX));
	v.y = (float)(*(int*)(moduleBase + offsets.cameraPosY));
	Vector2f result = pos - v;

	{
		float k = 480 / result.y;
		float k_1 = screenHeight / k;
		result.y = k_1;

		k = (744.0f - 105.0f) / (result.x - 105.0f);
		k_1 = screenWidth / k;
		result.x = k_1;
	}
	return result;
}
