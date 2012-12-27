#pragma once

#include "texture.h"
#include <math.h>

class CFont
{
private:
	CTexture tex;
	float fontSize;
	float fontColor[4];

public:
	CFont();
	~CFont();
	
	void Print(int x, int y, char *str,...);
	void SetSize(float size);
	void SetColor(float r, float g, float b, float a);
};