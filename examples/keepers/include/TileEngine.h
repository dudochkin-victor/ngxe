/*
 * TileEngine.h
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#include <string>
using std::string;

#include "Engine.h"

class CTileEngine: public CEngine
{
protected:
	void AdditionalInit();

	void Think			( const int& iElapsedTime );
	void Render			( SDL_Surface* pDestSurface );

	void KeyUp			(const int& iKeyEnum);
	void KeyDown		(const int& iKeyEnum);

	void End();

	void MouseMoved		(const int& iButton, const int& iX, const int& iY, const int& iRelX, const int& iRelY);
	void MouseButtonUp	(const int& iButton, const int& iX, const int& iY, const int& iRelX, const int& iRelY);
	void MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iRelX, const int& iRelY);
public:
	CTileEngine();
	virtual ~CTileEngine();

	bool LoadGame( const string& sFile );
	bool SaveGame( const string& sFile );
};

