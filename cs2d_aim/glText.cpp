#include "glText.h"

void GL::Font::Build(int height)
{
	hdc = wglGetCurrentDC();
	base = glGenLists(96);
	HFONT hFont = CreateFontA(-(height), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
	wglUseFontBitmaps(hdc, 32, 96, base);
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

	bBuilt = true;
}

vec GL::Font::centerText(float x, float y, float width, float height, float textWidth, float textHeight)
{
	vec text;
	text.x = x + (width - textWidth) / 2;
	text.y = y + textHeight;
	return text;
}

float GL::Font::centerText(float x, float width, float textWidth)
{
	if (width > textWidth)
	{
		float difference = width - textWidth;
		return (x + (difference / 2));
	}

	else
	{
		float difference = textWidth - width;
		return (x - (difference / 2));
	}
}