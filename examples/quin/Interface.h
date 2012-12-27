///////////////////////////////////////////////////////////////////////////////
// Interface.h : Header file for the Interface.
// This is a library that does all the platform-dependant stuff involved in
// initializing OpenGL, and also handles window creation, fullscreen grabbage,
// and user input detection.

// Author: Quin Pendragon (dragon@graduate.uwa.edu.au)
// Date:   2002-10-20

#ifndef Interface_h
#define Interface_h

//these *are* the droids we're looking for!

#ifdef _WINDOWS

//include appropriate windows headers
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

//get rid of name-too-long warnings
#pragma warning(disable : 4786)

//fix MSVC for-loop scope bug
#ifndef for
#define for if (false) { } else for
#endif // for
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif // _WINDOWS

//----------------------------------------------------------------//
//type definitions

//type CALLBACK: this is used to set all callback functions for events.
//Callbacks should be declared like:
//  void Callback(int event, int param1, int param2, int param3)
//with event being a unique constant for the interface, so that a single
//callback function can trap multiple events. The other arguments (param1,
//param2, and param3) have meaning dependant on context.
typedef void (*ICALLBACK) (int, int, int, int);

//generic unsigned long integer type for flags and bitfields and so forth
typedef unsigned long IVALUE;

//----------------------------------------------------------------//
//Valid callback codes and the meanings of the callback parameters//

#define EVENT_INIT             ((IVALUE)  1)
// void EventKeyPress(int event, int unused, int unused, int unused)

#define EVENT_KEYPRESS         ((IVALUE)  2)
// void EventKeyPress(int event, int keyPressed, int isDown, int isRepeat)

#define EVENT_MOUSEMOVE        ((IVALUE)  4)
// void EventMouseMove(int event, int mouseX, int mouseY, int buttonsDown)

#define EVENT_MOUSEPRESS       ((IVALUE)  8)
// void EventMousePress(int event, int mouseX, int mouseY, int buttonsDown)

#define EVENT_WINDOWSIZE       ((IVALUE) 16)
// void EventWindowSize(int event, int newWidth, int newHeight, int unused)

#define EVENT_REPAINT          ((IVALUE) 32)
// void EventRepaint(int event, int unused, int unused, int unused)

#define EVENT_RESET            ((IVALUE) 64)
// void EventReset(int event, int unused, int unused, int unused)

#define EVENT_IDLE             ((IVALUE)128)
// void EventIdle(int event, int unused, int unused, int unused)

#define EVENT_SHUTDOWN         ((IVALUE)1024)
// void EventShutdown(int event, int unused, int unused, int unused)

#define EVENT_ALL              ((IVALUE)0xFFFFFFFF)

//Notes:
// * The buttonsDown parameter of the mouse events is a bitfield. Use the
//   CheckButton function to turn it into a flag for a given mouse button.
// * The buttonsDown parameter also has info on whether shift or control
//   were being held down, which may be retrieved in the same way as buttons.
// * The idle event occurs repeatedly while the application is not handling
//   any other events. Put animation code here.
// * The Reset event indicates that OpenGL has been reset (due to a change
//   between windowed and fullscreen mode, for example) and that any
//   allocated OpenGL resources (fonts, textures etc.) should be recreated.

#define LEFT_BUTTON   1
#define MIDDLE_BUTTON 2
#define RIGHT_BUTTON  3
#define CONTROL_DOWN  4
#define SHIFT_DOWN    5

inline int CheckButton(int buttonsDown, int whichButton /* 1 - 3 */)
{ return (buttonsDown & (1 << (whichButton-1))) > 0 ? 1 : 0; }

//----------------------------------------------------------------//
//the Interface interface itself :)                               //

class IInterface
{
public:
	//Method:  SetFullscreen(bool fullScreen)
	//Purpose: Sets the interface to run in either full screen or windowed
	//         mode, depending on fFullScreen. If the interface is currently
	//         active, this will incur an EVENT_RESET event.
	virtual bool SetFullscreen(bool fullScreen) = 0;

	//Method:  GetFullscreen()
	//Purpose: Returns true iff the interface is set to fullscreen mode
	virtual bool GetFullscreen() = 0;

	//Method:  SetTitle(char *title)
	//Purpose: Sets the title of the window.
	virtual void SetTitle(const char *title) = 0;

	//Method:  SetCallback(IVALUE events, ICALLBACK fn)
	//Purpose: Sets the callback function given as the callback for the events
	//         specified. For example, if there is a function defined as:
	//           void MouseHandler(int evt, int x, int y, int buttons);
	//         which should be called for EVENT_MOUSEPRESS and EVENT_MOUSEMOVE,
	//         then the appropriate code to hook in this function is:
	//           SetCallback(EVENT_MOUSEPRESS | EVENT_MOUSEMOVE, MouseHandler);
	virtual void SetCallback(IVALUE events, ICALLBACK fn) = 0;

	//Method:  GetKey(int keycode)
	//Purpose: Returns true iff the given key is down. The key code should be
	//         either an ascii value ('a', 'B', etc.) or a virtual key code
	//         such as VK_LEFT, VK_ENTER etc. Dunno how this will port to linux.
	virtual bool GetKey(int keycode) = 0;

	//Method:  SetSize(int width, int height, int bpp)
	//Purpose: Sets the current window size. If the interface is active and
	//         in fullscreen, then this will incur an EVENT_RESET event.
	virtual bool SetSize(int width, int height, int bpp) = 0;

	//Method:  GetSize(int &width, int &height, int &bpp)
	//Purpose: Returns the current window size.
	virtual void GetSize(int &width, int &height, int &bpp) = 0;

	//Method:  GetDisplayMode(int mode, int &width, int &height, int &bpp)
	//Purpose: Retrieves information about the mode'th available display mode.
	//         Display mode indices start at 0. Returns 'false' if N is greater
	//         than or equal to the number of display modes.
	virtual bool GetDisplayMode(int mode, int &width, int &height, int &bpp)=0;

	//Method:  CreateBitmapFont(char *fontname, int height, int weight=400,
	//           bool italic=false, bool underline=false, bool strikeout=false,
	//           bool is_symbol_charset=false)
	//Purpose: Provides platform-independant font creation. Font weight should
	//         be between 1 (lightest)and 1000 (heaviest), or 0 for don't care.
	//         is_symbol_charset should be set to true when using Wingdings-type
	//         character sets. Returns the index of the display list created.
	virtual GLuint CreateBitmapFont(char *fontname, int height, int weight=400,
		bool italic=false, bool underline=false, bool strikeout=false,
		bool is_symbol_charset=false) = 0;

	//Method:  CreateOutlineFont(char *fontname, int height, float thickness,
	//           float tolerance, int weight, bool italic, bool underline,
	//           bool strikeout, bool is_symbol_charset)
	//Purpose: Provides platform-independant font creation. Font weight should
	//         be between 1 (lightest)and 1000 (heaviest), or 0 for don't care.
	//         is_symbol_charset should be set to true when using Wingdings-type
	//         character sets. Returns the index of the display list created
	virtual GLuint CreateOutlineFont(const char *fontname, int height,
		float thickness = 0.5f, float tolerance = 0.1f, int weight=400,
		bool italic=false, bool underline=false, bool strikeout=false,
		bool is_symbol_charset=false) = 0;

	//Method:  DestroyFont(GLuint index)
	//Purpose: Frees a previously allocated font. Works with both bitmap
	//         and outline fonts. Must be called on the return value of
	//         either CreateBitmapFont() or CreateOutlineFont().
	virtual void DestroyFont(GLuint index) = 0;

	//Method:  Activate()
	//Purpose: Shows the interface window, not returning until the user closes
	//         the window. One or more callbacks may be called while the
	//         interface is activated.
	virtual bool Activate() = 0;

	//Method:  Deactivate()
	//Purpose: Destroys the interface window, returning control to the function
	//         which called Activate() at some stage not long afterwards.
	virtual void Deactivate() = 0;

	//Method:  Flip()
	//Purpose: Displays the current drawing surface.
	virtual void Flip() = 0;

	//Method:  Time()
	//Purpose: Returns the 'current time', in milliseconds
	virtual int Clock() = 0;

	//Virtual destructor
	virtual ~IInterface() { }
};

//Initialization functions: Use these to create an instance of IInterface
IInterface *CreateInterface(int width, int height,
				int preferred_bit_depth = 32,
				bool center=true, bool sizable=false);

#endif // Interface_h
