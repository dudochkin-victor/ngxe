// stdafx.cpp : source file that includes just the standard includes
//	Assign5.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "StdAfx.h"

// TODO: reference any additional headers you need in STDAFX.H
#ifndef WIN32
// auxDIBImageLoad - Since Linux doesn't have the glAux libraries and headers
// we have to handle image loading ourselves.  This function just reads in
// ASCII formatted PPM files.
AUX_RGBImageRec* auxDIBImageLoad(const char* filename)
{
	char buffer[256];
	std::ifstream fin(filename);
	AUX_RGBImageRec* pImg = new AUX_RGBImageRec;
	int nMax;
	
	if (fin.is_open())
	{
		// Discard first 2 lines.  Can check the first
		// line to validate the format.
		fin.getline(buffer, 255);
		// Second line is just a comment
		fin.getline(buffer, 255);
		
		fin >> pImg->sizeX >> pImg->sizeY >> nMax;
				
		// The following is for ASCII format PPMs only
		// to use this for binary PPMs, you need to set the
		// ifstream mode to not skip whitespace and to read binary.
		// You then would read in one unsigned char at a time.
		
		pImg->data = new GLubyte[pImg->sizeX * pImg->sizeY * 3]; 
		
		for (int y=0;y<pImg->sizeY;y++)
		{
			for (int x=0;x<pImg->sizeX;x++)
			{
				int nIndex = ((pImg->sizeY-y-1)*pImg->sizeX + x)*3;
				int nRed, nGreen, nBlue;
				
				if (fin >> nRed >> nGreen >> nBlue)
				{
					pImg->data[nIndex] = nRed;
					pImg->data[nIndex+1] = nGreen;
					pImg->data[nIndex+2] = nBlue;
				}
								
			}
		}
		fin.close();
	}

	return pImg;
}
#endif
// and not in this file
