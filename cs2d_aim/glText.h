#ifndef GLTEXT_H
#define GLTEXT_H


#include <windows.h>
#include <stdio.h>
#include <gl\GL.h>

namespace GL
{
	class Font
	{
	public:
		bool bBuilt = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void Build(int height);
		void Print(float x, float y, const unsigned char color[3], const char* format, ...);
	};
};

#endif // GLTEXT_H