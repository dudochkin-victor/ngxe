#include "font.h"

CFont::CFont()
{
	fontSize = 1;
	fontColor[0] = 1;
	fontColor[1] = 1;
	fontColor[2] = 1;
	fontColor[3] = 1;
	tex.LoadTexture("font.bmp");
}

CFont::~CFont()
{
}

void CFont::Print(int x, int y, char *string,...)
{
	char text[4096];
	va_list	ap;
	int x1=x, y1=y;
	unsigned int i;
	float cx, cy;
	int offset = 96; // first font : -32

	if(string==NULL)
		return;

	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	tex.BindTexture();

	glColor4fv(fontColor);

	for(i=0;i<strlen(text);i++)
	{
		while(text[i]=='\n'||text[i]=='\r')
		{
			x1=x;
			y1+=16+16*(int)(fontSize-1.0f);
			i++;
		}

		cx=(float)fmod((text[i]+offset)/16.0f, 1);
		cy=((text[i]+offset)/16)/16.0f;

		glBegin(GL_QUADS);
			glTexCoord2f(cx, 1-cy);
			glVertex2i(x1, y1);
			glTexCoord2f(cx, 1-cy-0.0625f);
			glVertex2i(x1, (int)(16*fontSize)+y1);
			glTexCoord2f(cx+0.0625f, 1-cy-0.0625f);
			glVertex2i((int)(16*fontSize)+x1, (int)(16*fontSize)+y1);
			glTexCoord2f(cx+0.0625f, 1-cy);
			glVertex2i((int)(16*fontSize)+x1, y1);
		glEnd();

		x1+=10+(int)(10*(fontSize-1.0f));
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void CFont::SetSize(float size)
{
	fontSize = size;
}

void CFont::SetColor(float r, float g, float b, float a)
{
	fontColor[0] = r;
	fontColor[1] = g;
	fontColor[2] = b;
	fontColor[3] = a;
}
