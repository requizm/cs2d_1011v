#ifndef GLDRAW_H
#define GLDRAW_H
#include <string>
#include "glText.h"
#include "Functions.h"

namespace rgb
{
	const GLubyte red[3] = { 255, 0, 0 };
	const GLubyte green[3] = { 0, 255, 0 };
	const GLubyte gray[3] = { 55, 55, 55 };
	const GLubyte lightgray[3] = { 192, 192, 192 };
	const GLubyte black[3] = { 0, 0, 0 };
}



GL::Font font;
GLint viewport[4];

typedef BOOL(__stdcall* twglSwapBuffers) (_In_ HDC hDc);
void HookC(const char* function, uintptr_t& oFunction, void* hFunction)
{
	HMODULE hMod = GetModuleHandle("opengl32.dll");

	if (hMod)
	{
		oFunction = (uintptr_t)TrampolineHook((void*)GetProcAddress(hMod, function), hFunction, 5);
	}
}

void SetupOrtho()
{
	viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.000000, viewport[2], viewport[3], 0.000000, -1.000000, 1.000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

}


void DrawLine(float x1, float y1, float x2, float y2, const GLubyte color[3])
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void GL::Font::Print(float x, float y, const unsigned char color[3], const char* format, ...)
{
	glRasterPos2f(x, y);
	
	char text[100];
	va_list	args;

	va_start(args, format);
	vsprintf_s(text, 100, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

void renderText(std::string textString, float xpos, float ypos)
{
	font.Print(xpos, ypos, rgb::red, "%s", textString.c_str());
}

#endif 