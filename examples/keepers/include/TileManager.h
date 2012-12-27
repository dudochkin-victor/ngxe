/*
 * TileManager.h
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <vector>
using std::vector;

#include "Interfaces.h"
#include "Tile.h"

/** Tile mananger */
class CTileManager//: public IBaseEntity
{
protected:
	vector< vector<CTile*> > m_vvpTiles;

	void AddTile( CTile* pTile );
	int width, height; // размеры карты
	CEntity* pDefaultTile;
public:
	CTileManager();
	virtual ~CTileManager();

	virtual void			VThink		( const int& iElapsedTime );
	virtual void			VRender		( SDL_Surface* pDestSurface, SDL_Rect* m_View );

	virtual bool			VLoad		( TiXmlElement* pXMLData );
	virtual TiXmlElement	VGetSaveData();

	CTile* GetTile( const int& iX, const int& iY );
};

#endif // TILEMANAGER_H
