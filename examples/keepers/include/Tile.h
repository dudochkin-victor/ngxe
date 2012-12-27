#ifndef TILE_H
#define TILE_H

#include "Entity.h"

/** Tile entity */
class CTile: public CEntity
{
public:
	CTile();
	virtual ~CTile();

	virtual void			VThink		( const int& iElapsedTime );
	virtual void			VRender		( SDL_Surface* pDestSurface, SDL_Rect* m_View );

	virtual bool			VLoad		( TiXmlElement* pXMLData );
	virtual TiXmlElement	VGetSaveData();
};

#endif // TILE_H
