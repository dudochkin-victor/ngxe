/*
 * Game.h
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#ifndef GAME_H
#define GAME_H

#include "Interfaces.h"
#include "TileManager.h"
#include "UnitManager.h"

/** Game class */
class CGame//: public IBaseEntity
{
private:
	CTileManager m_oTileManager;
	CUnitManager m_OUnitManager;
	SDL_Rect m_View; // координаты окна отображения
public:
	CGame();
	virtual ~CGame();

	virtual void			VThink		( const int& iElapsedTime );
	virtual void			VRender		( SDL_Surface* pDestSurface );

	virtual bool			VLoad		( TiXmlElement* pXMLData );
	virtual TiXmlElement	VGetSaveData();

	CTileManager* GetTileManager() { return &m_oTileManager; }
	CUnitManager* GetUnitManager() { return &m_OUnitManager; }

	// Методы перемещения окна отображения
	void SetViewSize(int width, int height){ m_View.w = width; m_View.h = height;};
	void SetViewPoint(int x, int y){ m_View.x = x; m_View.y = y; };
};

#endif // GAME_H
