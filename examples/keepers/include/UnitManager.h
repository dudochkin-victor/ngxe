/*
 * UnitManager.h
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#ifndef UNITMANAGER_H
#define UNITMANAGER_H

#include <vector>
using std::vector;

#include "Interfaces.h"
#include "Unit.h"

/** Unit mananger */
class CUnitManager//: public IBaseEntity
{
protected:
	vector< CUnit* > m_vpUnits;
	void AddUnit( CUnit* pUnit );

public:
	CUnitManager();
	virtual ~CUnitManager();

	virtual void			VThink		( const int& iElapsedTime );
	virtual void			VRender		( SDL_Surface* pDestSurface, SDL_Rect* m_View );

	virtual bool			VLoad		( TiXmlElement* pXMLData );
	virtual TiXmlElement	VGetSaveData();

	CUnit* GetUnit( unsigned int iIndex );
};

#endif // UNITMANAGER_H
