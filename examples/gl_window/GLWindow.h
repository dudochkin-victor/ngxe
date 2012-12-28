#pragma once

class GLWindow
{
private:
	HWND	hWnd;
	HDC		hDC;
	HGLRC	hRC;
	bool	fullscreen;

public:
	GLWindow();
	~GLWindow();

	bool Initialize(int width, int height, bool fullscreen);
	void Terminate();

	void Swap(){ SwapBuffers(hDC); }

	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
