/*
 * Unit.h
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#ifndef UNIT_H
#define UNIT_H

#include "Entity.h"

/** Unit entity */
class CUnit: public CEntity
{
private:
	int m_iPlayer;

public:
	CUnit();
	virtual ~CUnit();

	virtual void			VThink		( const int& iElapsedTime );
	virtual void			VRender		( SDL_Surface* pDestSurface, SDL_Rect* m_View );

	virtual bool			VLoad		( TiXmlElement* pXMLData );
	virtual TiXmlElement	VGetSaveData();
};

#endif // UNIT_H
