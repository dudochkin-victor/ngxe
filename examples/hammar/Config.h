/*
	Config.h
*/

#include "Resource.h"

//// Dialog procedure
//bool CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch(uMsg)
//	{
//	case WM_INITDIALOG:
//		// Set the default settings
//		CheckDlgButton(hWnd, IDC_MEDRES, BST_CHECKED);
//		CheckDlgButton(hWnd, IDC_TRUECOLOR, BST_CHECKED);
//		CheckDlgButton(hWnd, IDC_FULLSCREEN, prefs::fullscreen?BST_CHECKED:BST_UNCHECKED);
//		CheckDlgButton(hWnd, IDC_TEX2, BST_CHECKED);
//		CheckDlgButton(hWnd, IDC_POLY3, BST_CHECKED);
//		return FALSE;
//
//	case WM_COMMAND:
//		switch(LOWORD(wParam))
//		{
//		case IDOK:
//			// Run the demo
//			EndDialog(hWnd, 1);
//			return TRUE;
//		case IDCANCEL:
//			// Don't run the demo
//			EndDialog(hWnd, 0);
//			return TRUE;
//		// Res
//		case IDC_LORES:
//			// Low resolution
//			prefs::screenx = 640;
//			prefs::screeny = 480;
//			CheckDlgButton(hWnd, IDC_LORES, BST_CHECKED);
//			CheckDlgButton(hWnd, IDC_MEDRES, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_HIRES, BST_UNCHECKED);
//			return TRUE;
//		case IDC_MEDRES:
//			// Med resolution
//			prefs::screenx = 800;
//			prefs::screeny = 600;
//			CheckDlgButton(hWnd, IDC_LORES, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_MEDRES, BST_CHECKED);
//			CheckDlgButton(hWnd, IDC_HIRES, BST_UNCHECKED);
//			return TRUE;
//		case IDC_HIRES:
//			// High resolution
//			prefs::screenx = 1024;
//			prefs::screeny = 768;
//			CheckDlgButton(hWnd, IDC_LORES, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_MEDRES, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_HIRES, BST_CHECKED);
//			return TRUE;
//
//		// Color depth
//		case IDC_HICOLOR:
//			// 16 bit
//			prefs::bitdepth = 16;
//			CheckDlgButton(hWnd, IDC_HICOLOR, BST_CHECKED);
//			CheckDlgButton(hWnd, IDC_TRUECOLOR, BST_UNCHECKED);
//			return TRUE;
//		case IDC_TRUECOLOR:
//			// 32 bit
//			prefs::bitdepth = 16;
//			CheckDlgButton(hWnd, IDC_HICOLOR, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_TRUECOLOR, BST_CHECKED);
//			return TRUE;
//
//		// Fullscreen
//		case IDC_FULLSCREEN:
//			prefs::fullscreen = !prefs::fullscreen;
//			CheckDlgButton(hWnd, IDC_FULLSCREEN, prefs::fullscreen?BST_CHECKED:BST_UNCHECKED);
//			return TRUE;
//
//		// Texture quality
//		case IDC_TEX1:
//			// No interpolation
//			prefs::texquality = 0;
//			CheckDlgButton(hWnd, IDC_TEX1, BST_CHECKED);
//			CheckDlgButton(hWnd, IDC_TEX2, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_TEX3, BST_UNCHECKED);
//			return TRUE;
//
//		case IDC_TEX2:
//			// Linear interpolation
//			prefs::texquality = 1;
//			CheckDlgButton(hWnd, IDC_TEX1, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_TEX2, BST_CHECKED);
//			CheckDlgButton(hWnd, IDC_TEX3, BST_UNCHECKED);
//			return TRUE;
//
//		case IDC_TEX3:
//			// Linear interpolation w/mipmaps
//			prefs::texquality = 2;
//			CheckDlgButton(hWnd, IDC_TEX1, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_TEX2, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_TEX3, BST_CHECKED);
//			return TRUE;
//
//		// Polygon count
//		case IDC_POLY1:
//			// Few polygons
//			prefs::polylevel = 0;
//			CheckDlgButton(hWnd, IDC_POLY1, BST_CHECKED);
//			CheckDlgButton(hWnd, IDC_POLY2, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY3, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY4, BST_UNCHECKED);
//			return TRUE;
//
//		case IDC_POLY2:
//			// Medium
//			prefs::polylevel = 1;
//			CheckDlgButton(hWnd, IDC_POLY1, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY2, BST_CHECKED);
//			CheckDlgButton(hWnd, IDC_POLY3, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY4, BST_UNCHECKED);
//			return TRUE;
//
//		case IDC_POLY3:
//			// Many
//			prefs::polylevel = 2;
//			CheckDlgButton(hWnd, IDC_POLY1, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY2, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY3, BST_CHECKED);
//			CheckDlgButton(hWnd, IDC_POLY4, BST_UNCHECKED);
//			return TRUE;
//
//		case IDC_POLY4:
//			// LOTS
//			prefs::polylevel = 3;
//			CheckDlgButton(hWnd, IDC_POLY1, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY2, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY3, BST_UNCHECKED);
//			CheckDlgButton(hWnd, IDC_POLY4, BST_CHECKED);
//			return TRUE;
//		}
//		break;
//	};
//
//	return FALSE;
//}

