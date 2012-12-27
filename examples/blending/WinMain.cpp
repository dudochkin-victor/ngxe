#include "glwindow.h"
#include <GL/glaux.h>

CGLWindow window;
GLuint texID;

GLuint LoadTextureWithAlphaMask(char *texname, char *maskname)
{
	GLuint texID;

	AUX_RGBImageRec *tex = auxDIBImageLoad(texname);
	AUX_RGBImageRec *mask = auxDIBImageLoad(texname);

	if(tex->sizeX!=mask->sizeX || tex->sizeY!=mask->sizeY){
		MessageBox(NULL, "Texture doesn't match the mask!", "Error", MB_OK);
		return 0;
	}

	UCHAR *data = new UCHAR[4*tex->sizeX*tex->sizeY];
	ZeroMemory(data, 4*tex->sizeX*tex->sizeY);

	for(int i=0;i<tex->sizeX*tex->sizeY;i++){
		data[i*4] = tex->data[i*3];
		data[i*4+1] = tex->data[i*3+1];
		data[i*4+2] = tex->data[i*3+2];
		data[i*4+3] = mask->data[i*3];
	}

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->sizeX, tex->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete [] data;
	delete [] tex->data;
	delete [] mask->data;

	return texID;
}

void Initialize()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	texID = LoadTextureWithAlphaMask("texture.bmp", "mask.bmp");

	glClearColor(0, 0.5f, 0.0f, 1);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex2f(-0.8f,-0.8f);
	glTexCoord2f(1,0); glVertex2f( 0.8f,-0.8f);
	glTexCoord2f(1,1); glVertex2f( 0.8f, 0.8f);
	glTexCoord2f(0,1); glVertex2f(-0.8f, 0.8f);
	glEnd();

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				default:
					break;
			}
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			window.Resize(LOWORD(lParam), HIWORD(lParam));
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	MSG msg;
	bool done = false;

	window.Initialize(800, 600, 32, false);

	Initialize();

	while(!done){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message==WM_QUIT){
				done = true;
			}
			else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			Render();
			window.Swap();
		}
	}

	window.Terminate();
	
	return msg.wParam;
}
