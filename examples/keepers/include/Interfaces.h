#ifndef IBASE_ENTITY_H
#define IBASE_ENTITY_H

#include "SDL.h"
#include "tinyxml.h"

#include <string>
using std::string;

class IBaseEntity
{
public:
	virtual void			VThink( const int& iElapsedTime )		= 0;
	virtual void			VRender( SDL_Surface* pDestSurface, SDL_Rect* m_View )	= 0;

	virtual bool			VLoad( TiXmlElement* pXMLData )		= 0;
	virtual TiXmlElement	VGetSaveData()				= 0;
};

#endif // IBASE_ENTITY_H
