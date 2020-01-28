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
typedef sVector2 Vector2;
bool aim = false;
bool esp = true;
bool flash = false;


//void SendMessageToChat(std::string msg);
void Initiate();
void UpdatePlayer();
void UpdateEnemies();
void UpdateEntity(Entity* entity);
int CalculateClosestEnemy();
int ChangeFlash();
void Render();
std::string BoolToString(bool a);
void WriteStatus();
Vector2 ScreenToWorld(Vector2 pos, Vector2 view);
Vector2 WorldToScreen(Vector2 pos, Vector2 view);
//extern "C" void OnStartMatch();

twglSwapBuffers owglSwapBuffers;

typedef int(__cdecl* fnGetHealth)(DWORD health_ptr);

//typedef int* (__cdecl* fnReloadAmmo)(int player_id, int b, int c);  // b = 1, c = 1
//typedef int* (__cdecl* fnMove)(DWORD player_ptr, float x, float y, int d, int collision);  // d = 1
//typedef DWORD(__cdecl* fnGetEnemyFromID)(int id); 
//typedef int(__cdecl* fnPosToCell)(double a1); //for player: ((double pos - 12.0) / 32.0)


//doesn't work effective
/*typedef int* (__cdecl* fnSendMessage)(int a1_1, int chat_status_1, DWORD text_pointer, int a4, int a5_0); // a1 = 1, chat_status = 1, a4 = 1, a5 = 0
typedef DWORD(__cdecl* fnMessage_01)(DWORD txtptr, DWORD chatptr_size, DWORD textptr_size); //sub_591270
typedef DWORD(__cdecl* fnMessage_02)(__int16 asci_no); //sub_591130
typedef DWORD(__cdecl* fnMessage_03)(DWORD a1, DWORD a2); //sub_591200*/

//typedef int (__cdecl* fnEnemyShot)(int target_id, int source_id, int damage, int a4, int a5, int a6, int a7); a4 = 0, a5 = -1, a6 = 0, a7 = 0 




//fnReloadAmmo ReloadAmmo;
//fnMove Move;
//fnGetEnemyFromID GetEnemyFromID;
//fnPosToCell PosToCell;
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
		entity->xPos = *(float*)(entity->baseAdress + offsets.xpos);
		entity->yPos = *(float*)(entity->baseAdress + offsets.ypos);
		entity->hp = GetHealth(*(DWORD*)(entity->baseAdress + offsets.healthptr));
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
				float xDistance = abs(enemies[i].xPos - Player.xPos);
				float yDistance = abs(enemies[i].yPos - Player.yPos);
				float len = sqrtf(pow(xDistance, 2) + pow(yDistance, 2));
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

int ChangeFlash()
{
	DWORD address = moduleBase + offsets.flash_01;
	DWORD address_2 = moduleBase + offsets.flash_02;
	BYTE value = *(BYTE*)(address);
	BYTE value_2 = *(BYTE*)(address_2);

	if ((int)value == 0x84 && (int)value_2 == 0x84 && !flash) //je - jne
	{
		value = 0x85;
	}
	else if ((int)value == 0x85 && (int)value_2 == 0x85 && flash) //je - jne
	{
		value = 0x84;
	}
	else
	{
		std::cout << "error on flash jump" << std::endl;
		return 0;
	}

	DWORD OldProtection = NULL;
	VirtualProtect(reinterpret_cast<void*>(address), 1, PAGE_EXECUTE_READWRITE, &OldProtection);
	*reinterpret_cast<PBYTE>(address) = value;
	VirtualProtect(reinterpret_cast<void*>(address), 1, OldProtection, &OldProtection);

	OldProtection = NULL;
	VirtualProtect(reinterpret_cast<void*>(address_2), 1, PAGE_EXECUTE_READWRITE, &OldProtection);
	*reinterpret_cast<PBYTE>(address_2) = value;
	VirtualProtect(reinterpret_cast<void*>(address_2), 1, OldProtection, &OldProtection);

	return 1;
}

void Render()
{
	HDC currentHDC = wglGetCurrentDC();

	SetupOrtho();

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
	/*Vector2 mouse, view;
		mouse.x = *(float*)(moduleBase + offsets.readmouseX);
		mouse.y = *(float*)(moduleBase + offsets.readmouseY);
		view.x = (float)(*(int*)(moduleBase + offsets.cameraPosX));
		view.y = (float)(*(int*)(moduleBase + offsets.cameraPosY));
		float div = *(float*)(moduleBase + 0x4CF274);
		Vector2 pos = ScreenToWorld(mouse, view);
		int x = PosToCell((double)((pos.x) / div));
		int y = PosToCell((double)((pos.y) / div));*/
	

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

		int index = CalculateClosestEnemy();
		if (index == -1)
		{
			goto ata;
		}
		Entity target = enemies.at(index);
		if (Player.isLive == 0 && aim)
		{

			Vector2 view, scr;
			view.x = (float)(*(int*)(moduleBase + offsets.cameraPosX));
			view.y = (float)(*(int*)(moduleBase + offsets.cameraPosY));
			scr.x = screenWidth;
			scr.y = screenHeight;

			Vector2 res = ScreenToWorld(scr, view);

			float xDistance = target.xPos - Player.xPos;
			float yDistance = target.yPos - Player.yPos;

			float dist = sqrtf(pow(xDistance, 2) + pow(yDistance, 2)); //magnitude
			float maxDist = sqrtf(pow(res.x / 6, 2) + pow(res.y / 6, 2));

			float d_x = xDistance / dist; //normalized
			float d_y = yDistance / dist; //normalized

			Vector2 direction;
			direction.x = d_x;
			direction.y = d_y;

			Vector2 enemy, player;
			enemy.x = target.xPos;
			enemy.y = target.yPos;

			Vector2 result;
			if (dist > maxDist)
			{
				Vector2 ps;
				ps.x = Player.xPos + direction.x * 150;
				ps.y = Player.yPos + direction.y * 150;
				Vector2 t = WorldToScreen(enemy, view);
				Vector2 e = WorldToScreen(ps, view);
				{
					float k = 480 / e.y;
					float k_1 = screenHeight / k;
					e.y = k_1;

					k = (744.0f - 105.0f) / (e.x - 105.0f);
					k_1 = screenWidth / k;
					e.x = k_1;

					k = 480 / t.y;
					k_1 = screenHeight / k;
					t.y = k_1;

					k = (744.0f - 105.0f) / (t.x - 105.0f);
					k_1 = screenWidth / k;
					t.x = k_1;
				}
				result.x = e.x;
				result.y = e.y;
			}
			else
			{
				player.x = Player.xPos;
				player.y = Player.yPos;
				Vector2 t = WorldToScreen(enemy, view);
				Vector2 e = WorldToScreen(player, view);

				{
					float k = 480 / e.y;
					float k_1 = screenHeight / k;
					e.y = k_1;

					k = (744.0f - 105.0f) / (e.x - 105.0f);
					k_1 = screenWidth / k;
					e.x = k_1;

					k = 480 / t.y;
					k_1 = screenHeight / k;
					t.y = k_1;

					k = (744.0f - 105.0f) / (t.x - 105.0f);
					k_1 = screenWidth / k;
					t.x = k_1;
				}
				result.x = t.x;
				result.y = t.y;
			}

			*(int*)Screen.mouseX = result.x;
			*(int*)Screen.mouseY = result.y;
		}
	}

ata:
	if (enemies.size() > 0 && Player.isLive == 0 && match != 0 && esp)
	{
		for (std::vector<int>::size_type i = 0; i != enemies.size(); i++)
		{
			if (enemies[i].isLive != 0 || Player.isLive != 0 || enemies[i].team == Player.team)
				continue;

			Vector2 enemy, view, player;
			enemy.x = enemies[i].xPos;
			enemy.y = enemies[i].yPos;

			player.x = Player.xPos;
			player.y = Player.yPos;

			view.x = (float)(*(int*)(moduleBase + offsets.cameraPosX));
			view.y = (float)(*(int*)(moduleBase + offsets.cameraPosY));

			Vector2 t = WorldToScreen(enemy, view);
			Vector2 e = WorldToScreen(player, view);

			{
				float k = 480 / e.y;
				float k_1 = screenHeight / k;
				e.y = k_1;

				k = (744.0f - 105.0f) / (e.x - 105.0f);
				k_1 = screenWidth / k;
				e.x = k_1;

				k = 480 / t.y;
				k_1 = screenHeight / k;
				t.y = k_1;

				k = (744.0f - 105.0f) / (t.x - 105.0f);
				k_1 = screenWidth / k;
				t.x = k_1;
			}


			if (enemies[i].hp > 70)
				DrawLine(e.x, e.y, t.x, t.y, rgb::green);
			else if (enemies[i].hp > 40)
				DrawLine(e.x, e.y, t.x, t.y, rgb::gray);
			else if (enemies[i].hp > 0)
				DrawLine(e.x, e.y, t.x, t.y, rgb::red);



		}
	}

}

/*void SendMessageToChat(std::string msg)
{
	DWORD chatBase = moduleBase + offsets.chatPtr;
	*(int*)(chatBase + offsets.chatStatus) = 1;

	DWORD sendTo = moduleBase + offsets.inputLetterPtr;
	DWORD chatpointer, txtpointer;


	int sz = msg.size();
	std::string s("ab");
	for (char& c : s)
	{
		*(int*)sendTo = c;
		txtpointer = *(DWORD*)(chatBase + offsets.txtPointer);
		msg_01(txtpointer, *(DWORD*)(chatBase + offsets.chatSize), *(DWORD*)(txtpointer + offsets.txtSize));
		DWORD a = msg_02(c);
		DWORD b = msg_01(txtpointer, 0x0, *(DWORD*)(chatBase + offsets.chatSize));
		DWORD c = msg_03(b, a);
		txtpointer = msg_03(c, moduleBase + 0x3C4C80);
	}

	*(int*)(txtpointer + offsets.txtSize) = sz;

	int i = 0;
	int d = offsets.txtStart;
	for (char& c : msg)
	{
		*(BYTE*)(txtpointer + d++) = c;
		if (i + 1 != sz)
			*(BYTE*)(txtpointer + d++) = 0x00;
		i++;
	}

	SendMessageChat(1, 1, txtpointer, 1, 0);
	*(int*)chatSt = 0;
}*/

void Initiate()
{
	moduleBase = getModuleBase("cs2d.exe");

	//ReloadAmmo = (fnReloadAmmo)(moduleBase + functions.ReloadAmmo);
	//Move = (fnMove)(moduleBase + functions.Move);
	//GetEnemyFromID = (fnGetEnemyFromID)(moduleBase + functions.GetEnemyFromID); //
	//PosToCell = (fnPosToCell)(moduleBase + functions.PosToCell);
	/*SendMessageChat = (fnSendMessage)(moduleBase + functions.SendMessageToChat);
	msg_01 = (fnMessage_01)(moduleBase + functions.msg_01);
	msg_02 = (fnMessage_02)(moduleBase + functions.msg_02);
	msg_03 = (fnMessage_03)(moduleBase + functions.msg_03);*/
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

	std::cout << "[F5] aim: " << a << std::endl;
	std::cout << "[F6] esp: " << e << std::endl;
	std::cout << "[F7] noflash: " << n << std::endl;
}

Vector2 ScreenToWorld(Vector2 pos, Vector2 view)
{
	Vector2 result;
	result.x = pos.x + view.x;
	result.y = pos.y + view.y;
	return result;
}

Vector2 WorldToScreen(Vector2 pos, Vector2 view)
{
	Vector2 result;
	result.x = pos.x - view.x;
	result.y = pos.y - view.y;
	return result;
}
