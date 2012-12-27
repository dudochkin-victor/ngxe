///////////////////////////////////////////////////////////////////////////////
// Interface_Win32.h : Implementation file for Interface under Win32.
// Author: Quin Pendragon (dragon@graduate.uwa.edu.au)
// Date:   2002-10-20
//
// A great big THANKYOU goes out to Jeff Molofee (NeHe, of nehe.gamedev.net)
// for the OpenGL tutorials which made this not only possible, but easy :)

#include "Interface.h"

#include <stdio.h>

#define MAX_TITLE	256

//global instance handle
static HINSTANCE g_hInstance;

const char INTERFACE_WNDCLASS[] = "Interface Window Class";

//----------------------------------------------------------------//
// Definition of the CWin32Interface class. This is an
// implementation of the IInterface interface for Windows.

class CWin32Interface : public IInterface
{
public:
	//Constructor
	CWin32Interface(int width, int height, int bps, bool center, bool sizable);

	//implementations of IInterface methods
	virtual bool GetFullscreen();
	virtual bool SetFullscreen(bool fullScreen);
	virtual void SetTitle(char *title);
	virtual void SetCallback(IVALUE events, ICALLBACK fn);
	virtual bool SetSize(int width, int height, int bpp);
	virtual void GetSize(int &width, int &height, int &bpp);
	virtual bool GetDisplayMode(int mode, int &width, int &height, int &bpp);
	virtual bool GetKey(int keycode);
	virtual GLuint CreateBitmapFont(char *fontname, int height, int weight,
		bool italic, bool underline, bool strikeout, bool symbol);
	virtual GLuint CreateOutlineFont(char *fontname, int height, float thickness,
		float tolerance, int weight, bool italic, bool underline,
		bool strikeout, bool symbol);
	virtual void DestroyFont(GLuint index);
	virtual bool Activate();
	virtual void Deactivate();
	virtual void Flip();
	virtual int Clock();

	HWND GetHwnd();

	virtual ~CWin32Interface();

private:
	//private implementation details
	bool RegisterInterfaceWindowClass();
	HWND CreateInterfaceWindow();

	bool AttachOpenGL();
	void DetachOpenGL(bool restoreScreen=true);

	long HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//private member variables
	int m_width;                 //initial width of window
	int m_height;                //initial height of window
	int m_bitdepth;              //desired full screen bit depth
	bool m_center;               //center the window on creation?
	bool m_sizable;              //allow resizing the window?
	bool m_fullscreen;           //is interface fullscreen?
	char m_title[MAX_TITLE];     //name of window

	int m_curheight;             //current height of the window
	int m_curwidth;              //current width of the window

	bool m_trapLoseFocus;        //should we switch back to windowed
	                             //mode when we lose focus?

	HWND  m_hWnd;                //window handle, if we're active
	HDC   m_hDC;                 //device context, if we're active
	HGLRC m_hGLRC;               //OpenGL rendering context

	ICALLBACK m_eventInit;       //callback function for initialization
	ICALLBACK m_eventKeyPress;   //callback function for key presses
	ICALLBACK m_eventMouseMove;  //callback function for mouse moves
	ICALLBACK m_eventMousePress; //callback function for mouse presses
	ICALLBACK m_eventWindowSize; //callback function for window resizes
	ICALLBACK m_eventRepaint;    //callback function for window repainting
	ICALLBACK m_eventReset;      //callback function for when OpenGL is reset
	ICALLBACK m_eventIdle;       //callback function for idle-time processing.
	ICALLBACK m_eventShutdown;   //callback function for shutdown processing.

	//window procedures must be functions. How irritating.
	friend long WINAPI InterfaceWndProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);
};

//----------------------------------------------------------------//
// Implementation of the IInterface interface members.

CWin32Interface::CWin32Interface(int w, int h, int b, bool c, bool s) 
	: m_width(w), m_height(h), m_bitdepth(b), m_center(c), m_sizable(s)
{
	m_hWnd = NULL;
	m_fullscreen = false;
	m_trapLoseFocus = true;
	m_eventInit = NULL;
	m_eventKeyPress = NULL;
	m_eventMouseMove = NULL;
	m_eventMousePress = NULL;
	m_eventWindowSize = NULL;
	m_eventShutdown = NULL;
	m_eventRepaint = NULL;
	m_eventReset = NULL;
	m_eventIdle = NULL;
	strcpy(m_title, "Interface Window");
}


bool CWin32Interface::GetFullscreen()
{
	return m_fullscreen;
}

bool CWin32Interface::SetFullscreen(bool fullScreen)
{
	if ((m_fullscreen != 0) == (fullScreen != 0)) return true;
	if (m_hWnd)
	{
		m_trapLoseFocus = false;
		//shut all graphics stuff down
		DetachOpenGL();
		DestroyWindow(m_hWnd);
		m_fullscreen = fullScreen;
		//restart it all
		if (!(m_hWnd = CreateInterfaceWindow()))
		{
			MessageBox(NULL, "Couldn't re-create window while "
				"switching to/from fullscreen!", "Argle Fargle!", MB_OK);
			m_hWnd = NULL;
			return false;
		}
		if (!AttachOpenGL())
		{
			DetachOpenGL(); //undo however far Attach() got.
			DestroyWindow(m_hWnd); m_hWnd = NULL;
			MessageBox(NULL, "Failed to re-initialize OpenGL!!!",
				"Fear is the path to the dark side!", MB_OK);
			return false;
		}
		m_trapLoseFocus = true;
		if (m_eventReset)
			m_eventReset(EVENT_RESET, 0, 0, 0);
	}
	m_fullscreen = fullScreen;
	return true;
}


void CWin32Interface::SetTitle(char *title)
{
	strncpy(m_title, title, MAX_TITLE);
	if (m_hWnd != NULL) SetWindowText(m_hWnd, title);
}


void CWin32Interface::SetCallback(IVALUE events, ICALLBACK fn)
{
	if (events & EVENT_INIT      ) m_eventInit       = fn;
	if (events & EVENT_KEYPRESS  ) m_eventKeyPress   = fn;
	if (events & EVENT_MOUSEMOVE ) m_eventMouseMove  = fn;
	if (events & EVENT_MOUSEPRESS) m_eventMousePress = fn;
	if (events & EVENT_WINDOWSIZE) m_eventWindowSize = fn;
	if (events & EVENT_REPAINT   ) m_eventRepaint    = fn;
	if (events & EVENT_RESET     ) m_eventReset      = fn;
	if (events & EVENT_IDLE      ) m_eventIdle       = fn;
	if (events & EVENT_SHUTDOWN  ) m_eventShutdown   = fn;
}


bool CWin32Interface::SetSize(int width, int height, int bpp)
{
	if (m_width == width && m_height == height && m_bitdepth == bpp) return true;
	//copy across the new settings
	m_width = width;
	m_height = height;
	m_bitdepth = bpp;
	//apply the changes
	if (m_hWnd != NULL)
	{
		if (m_fullscreen) //we have to bugger around with display modes
		{
			m_trapLoseFocus = false;
			//shut all graphics stuff down
			DetachOpenGL(false);
			DestroyWindow(m_hWnd);
			//restart it all
			if (!(m_hWnd = CreateInterfaceWindow()))
			{
				MessageBox(NULL, "Couldn't re-create window while "
					"switching to/from fullscreen!", "Argle Fargle!", MB_OK);
				m_hWnd = NULL;
				return false;
			}
			if (!AttachOpenGL())
			{
				DetachOpenGL(); //undo however far Attach() got.
				SendMessage(m_hWnd, WM_CLOSE, 0, 0);
				MessageBox(NULL, "Failed to re-initialize OpenGL!!!",
					"Hatred is the path to the dark side!", MB_OK);
				return false;
			}
			m_trapLoseFocus = true;
			if (m_eventReset)
				m_eventReset(EVENT_RESET, 0, 0, 0);
		}
		else //just resize the window and reset the viewport
		{
			//find the new size
			WINDOWPLACEMENT wp;
			GetWindowPlacement(m_hWnd, &wp);
			RECT rc = {0, 0, width, height};
			AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, FALSE);
			rc.right -= rc.left; rc.bottom -= rc.top;
			//reset the viewport
			glViewport(0, 0, width, height);
			gluPerspective(45.0f, (float)width / height, 1.0f, 100.0f);
			//resize the window
			MoveWindow(m_hWnd, wp.rcNormalPosition.left, wp.rcNormalPosition.top,
				rc.right, rc.bottom, TRUE);
		}
		//call the resize-event proc
		if (m_eventWindowSize)
			m_eventWindowSize(EVENT_WINDOWSIZE, width, height, 0);
	}
	return true;
}


void CWin32Interface::GetSize(int &width, int &height, int &bpp)
{
	width = m_width;
	height = m_height;
	bpp = m_bitdepth;
}


bool CWin32Interface::GetDisplayMode(int mode, int &width, int &height, int &bpp)
{
	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);
	BOOL ret = EnumDisplaySettings(NULL, mode, &dm);
	if (ret)
	{
		width = dm.dmPelsWidth;
		height = dm.dmPelsHeight;
		bpp = dm.dmBitsPerPel;
	}
	return (ret != 0);
}

GLuint CWin32Interface::CreateBitmapFont(char *fontname, int height,
		int weight, bool italic, bool underline, bool strikeout, bool symbol)
{
	//create the font
	HFONT font = CreateFont(height, 0, 0, 0, weight, italic, underline,
		strikeout, symbol ? SYMBOL_CHARSET : ANSI_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
		fontname);
	//generate a set of display lists for the font
	GLuint index = glGenLists(256);
	HFONT old = (HFONT)SelectObject(m_hDC, font);
	wglUseFontBitmaps(m_hDC, 0, 256, index);
	SelectObject(m_hDC, old);
	DeleteObject(font);
	//done!
	return index;
}


GLuint CWin32Interface::CreateOutlineFont(char *fontname, int height,
		float thickness, float tolerance, int weight, bool italic,
		bool underline, bool strikeout, bool symbol)
{
	GLYPHMETRICSFLOAT gmf[256];
	//create the font
	HFONT font = CreateFont(height, 0, 0, 0, weight, italic, underline,
		strikeout, symbol ? SYMBOL_CHARSET : ANSI_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
		fontname);
	//generate a set of display lists for the font
	GLuint index = glGenLists(256);
	HFONT old = (HFONT)SelectObject(m_hDC, font);
	wglUseFontOutlines(m_hDC, 0, 256, index, tolerance, thickness,
		WGL_FONT_POLYGONS, gmf);
	SelectObject(m_hDC, old);
	DeleteObject(font);
	//done!
	return index;
}

void CWin32Interface::DestroyFont(GLuint index)
{
	glDeleteLists(index, 256);
}


bool CWin32Interface::GetKey(int keycode)
{
	return (GetAsyncKeyState(keycode) & 0x800) != 0;
}


bool CWin32Interface::Activate()
{
	MSG msg;
	//create our window
	if (!RegisterInterfaceWindowClass()) return false;
	if (!(m_hWnd = CreateInterfaceWindow())) return false;
	//set up OpenGL or die
	if (!AttachOpenGL())
	{
		DetachOpenGL(); //undo however far Attach() got.
		DestroyWindow(m_hWnd); m_hWnd = NULL;
		MessageBox(NULL, "Failed to initialize OpenGL!!!",
			"Anger is the path to the dark side!", MB_OK);
		return false;
	}
	//call our initialization function, if any
	if (m_eventInit != NULL)
		m_eventInit(EVENT_INIT, 0, 0, 0);
	//message loop until we get a WM_CLOSE
	while(m_hWnd != NULL)
	{
		while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		if (m_eventIdle != NULL)
		{	
			m_eventIdle(EVENT_IDLE, 0, 0, 0);
			Sleep(10);
		}
		else
			Sleep(100);
	}
	return true;
}


void CWin32Interface::Deactivate()
{
	if (m_hWnd != NULL) PostMessage(m_hWnd, WM_CLOSE, 0, 0);
}


void CWin32Interface::Flip()
{
	if (m_hDC != NULL) SwapBuffers(m_hDC);
}


int CWin32Interface::Clock()
{
	return GetTickCount();
}


CWin32Interface::~CWin32Interface()
{
	if (m_hWnd != NULL)
	{
		DetachOpenGL();
		DestroyWindow(m_hWnd);
	}

}

//----------------------------------------------------------------//
// Implementation of private CWin32Interface interface members.

//Registers the window class, if needed.
bool CWin32Interface::RegisterInterfaceWindowClass()
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	//check to see if we've already registered
	if (GetClassInfo(g_hInstance, INTERFACE_WNDCLASS, &wc)) return true;
	//if not, go ahead and do so
	wc.lpszClassName = INTERFACE_WNDCLASS;
	wc.lpfnWndProc = InterfaceWndProc;
	wc.hInstance = g_hInstance;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbWndExtra = sizeof(CWin32Interface *);
	wc.hbrBackground = NULL;//(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	return RegisterClass(&wc) != 0;
}


//Creates the interface window, optionally centering it on the screen.
HWND CWin32Interface::CreateInterfaceWindow()
{
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	style |= m_sizable ? WS_THICKFRAME : WS_DLGFRAME;
	style |= m_fullscreen ? WS_MAXIMIZE : 0;
	RECT rcWnd = { 0, 0, m_width, m_height };
	if (m_fullscreen) style = WS_POPUP;
	else
	{
		AdjustWindowRect(&rcWnd, style, false);
		rcWnd.right -= rcWnd.left; rcWnd.left = CW_USEDEFAULT;
		rcWnd.bottom -= rcWnd.top; rcWnd.top = CW_USEDEFAULT;
	}
	style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	HWND hWnd = CreateWindow(INTERFACE_WNDCLASS, m_title, style,
		rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom,
		NULL, NULL, g_hInstance, (LPVOID)this);
	if (hWnd == NULL) return NULL; // :(

	//center the window if necessary
	if (m_center && !m_fullscreen)
	{
		WINDOWPLACEMENT wp;
		int cxscreen = GetSystemMetrics(SM_CXSCREEN);
		int cyscreen = GetSystemMetrics(SM_CYSCREEN);
		int windowX = (cxscreen - m_width) / 2;
		int windowY = (cyscreen - m_height) / 2;
		GetWindowPlacement(hWnd, &wp);
		wp.rcNormalPosition.right -= wp.rcNormalPosition.left - windowX;
		wp.rcNormalPosition.bottom -= wp.rcNormalPosition.top - windowY;
		wp.rcNormalPosition.left = windowX;
		wp.rcNormalPosition.top = windowY;
		SetWindowPlacement(hWnd, &wp);
	}

	m_curheight = m_height;
	m_curwidth = m_width;

	//finally, show the window and return true.
	ShowWindow(hWnd, SW_SHOW);
	return hWnd;
}


//Creates and attaches an OpenGL context to the window
bool CWin32Interface::AttachOpenGL()
{
	//if we're fullscreen, find a display format suitable and switch modes
	if (m_fullscreen)
	{
		ShowCursor(FALSE);
		DEVMODE devmode;
		ZeroMemory(&devmode, sizeof(devmode));
		devmode.dmSize = sizeof(devmode);
		devmode.dmPelsWidth = m_width;
		devmode.dmPelsHeight = m_height;
		devmode.dmBitsPerPel = m_bitdepth;
		devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&devmode, CDS_FULLSCREEN)
			!= DISP_CHANGE_SUCCESSFUL)
			return false;
	}
	//set up a pixel format descriptor for our required display
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = m_bitdepth;
	pfd.cDepthBits = m_bitdepth;
	pfd.iLayerType = PFD_MAIN_PLANE;
	//get a DC for the window
	if (!(m_hDC = GetDC(m_hWnd)))
	{
		MessageBox(NULL, "Couldn't get a device context!?!",
			"Initialization Error", MB_OK);
		return false;
	}
	//try to find an appropriate pixel format
	GLuint fmt = ChoosePixelFormat(m_hDC, &pfd);
	if (!fmt)
	{
		MessageBox(NULL, "Couldn't find appropriate pixel format!?!",
			"Initialization Error", MB_OK);
		return false;
	}
	//try to set our hDC to the given pixel format
	if (!SetPixelFormat(m_hDC, fmt, &pfd))
	{
		MessageBox(NULL, "Couldn't set pixel format!?!",
			"Initialization Error", MB_OK);
		return false;
	}
	//try to create an OpenGL rendering context
	if (!(m_hGLRC = wglCreateContext(m_hDC)))
	{
		MessageBox(NULL, "Couldn't create OpenGL Rendering Context!?!",
			"Initialization Error", MB_OK);
		return false;
	}
	//try to activate the rendering context
	if (!(wglMakeCurrent(m_hDC, m_hGLRC)))
	{
		MessageBox(NULL, "Couldn't select OpenGL context!?!",
			"Initialization Error", MB_OK);
		return false;
	}
	//woohoo! we're done!
	return true;
}


//Detaches and destroys the OpenGL context.
void CWin32Interface::DetachOpenGL(bool restoreScreen)
{
	if (m_fullscreen)
	{
		if (restoreScreen)
			ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
	wglMakeCurrent(NULL, NULL);
	if (m_hGLRC != NULL) { wglDeleteContext(m_hGLRC); m_hGLRC = NULL; }
	if (m_hDC != NULL) { ReleaseDC(m_hWnd, m_hDC); m_hDC = NULL; }
}


//Helper for HandleMessage(): Makes a flag field for mouse/meta buttons down
//given the wParam from a mouse move or click message.
inline int MakeButton(WPARAM wParam)
{
	int btns = 0;
	if (wParam & MK_LBUTTON) btns |= 1;
	if (wParam & MK_MBUTTON) btns |= 2;
	if (wParam & MK_RBUTTON) btns |= 4;
	if (wParam & MK_CONTROL) btns |= 8;
	if (wParam & MK_SHIFT  ) btns |= 16;
	return btns;
}

//Handles messages for the interface window.
long CWin32Interface::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		if (m_trapLoseFocus && m_fullscreen) SetFullscreen(false);
		break;

	case WM_PAINT:
		if (m_eventRepaint != NULL)
			m_eventRepaint(EVENT_REPAINT, 0, 0, 0);
		ValidateRect(m_hWnd, NULL);
		break;

	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (m_eventMousePress != NULL)
		{
			SetCapture(m_hWnd);
			POINTS p = MAKEPOINTS(lParam);
			m_eventMousePress(EVENT_MOUSEPRESS,
				p.x, p.y, MakeButton(wParam));
		}
		break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		if (m_eventMousePress != NULL)
		{
			ReleaseCapture();
			POINTS p = MAKEPOINTS(lParam);
			m_eventMousePress(EVENT_MOUSEPRESS,
				p.x, p.y, MakeButton(wParam));
		}
		break;

	case WM_MOUSEMOVE:
		if (m_eventMouseMove != NULL)
		{
			POINTS p = MAKEPOINTS(lParam);
			m_eventMouseMove(EVENT_MOUSEMOVE,
				p.x, p.y, MakeButton(wParam));
		}
		break;

	case WM_KEYDOWN:
		if (m_eventKeyPress != NULL)
			m_eventKeyPress(EVENT_KEYPRESS, wParam, 1, (lParam & (1 << 30)));
		break;

	case WM_KEYUP:
		if (m_eventKeyPress != NULL)
			m_eventKeyPress(EVENT_KEYPRESS, wParam, 0, 0);
		break;

	case WM_SIZE:
		if (m_eventWindowSize != NULL)
			m_eventWindowSize(EVENT_WINDOWSIZE,
				LOWORD(lParam), HIWORD(lParam), 0);
		break;

	case WM_CLOSE:
		if (m_eventShutdown != NULL)
			m_eventShutdown(EVENT_SHUTDOWN, 0, 0, 0);
		DetachOpenGL();
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
		break;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

//Returns the current HWND for the interface
HWND CWin32Interface::GetHwnd()
{
	return m_hWnd;
}

//----------------------------------------------------------------//
// Placeholder window proc - forwards all messages to the object
// which owns the window.

long WINAPI InterfaceWndProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			//on WM_CREATE, set a window paramater to point to the owner class
			LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
			SetWindowLong(hWnd, 0, (LONG)lpcs->lpCreateParams);
		}
		break;

	default:
		{
			//redirect all messages to the class window proc.
			CWin32Interface *pInt = (CWin32Interface *)GetWindowLong(hWnd, 0);
			if ((pInt == NULL) || (pInt->m_hWnd != hWnd))
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			return pInt->HandleMessage(uMsg, wParam, lParam);
		}
	}
	return 0;
}


//----------------------------------------------------------------//
// Creation functions: These create instances of IInterface.

IInterface *CreateInterface(int width, int height, int bitdepth,
				bool center, bool sizable)
{
	//return an instance of CWin32Interface
	CWin32Interface *pi = new CWin32Interface(width, height,
		bitdepth, center, sizable);
	return pi;
}

//----------------------------------------------------------------//
// Hack to allow retrieval of window handle

HWND GetHwnd(IInterface *iface)
{
	return ((CWin32Interface *)iface)->GetHwnd();
}

//----------------------------------------------------------------//
// Finally, handle bootstrapping. Windows uses WinMain, not main()
// like all real operating systems, so redirect WinMain to main().

extern int main(int argc, char **argv);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE null, LPSTR lpCmdLine,
			int nobody_ever_takes_any_notice_of_this_variable)
{
	g_hInstance = hInstance;
	//change the current directory to the executable's directory
	char current_dir[MAX_PATH] = "";
	GetModuleFileName(hInstance, current_dir, MAX_PATH);
	for (int i=strlen(current_dir)-1; current_dir[i] != '\\'; --i)
		current_dir[i] = 0;
	SetCurrentDirectory(current_dir);
	//fabricate some arguments
	char *argv[1];
	char txt[10] = { "Interface" };
	argv[0] = txt;
	return main(1, argv);
}
