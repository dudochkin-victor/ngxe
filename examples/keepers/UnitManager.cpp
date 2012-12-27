/*
 * UnitManager.cpp
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#include "UnitManager.h"
#include "Unit.h"

#include <iostream>
using namespace std;

CUnitManager::CUnitManager()
{

}

CUnitManager::~CUnitManager()
{
	while( m_vpUnits.size() > 0 )
	{
		if ( m_vpUnits.back() != 0 ) {
			delete m_vpUnits.back();
		}
		m_vpUnits.pop_back();
	}
}

bool CUnitManager::VLoad( TiXmlElement* pXMLData )
{
	TiXmlNode* pXMLUnit = 0;
	CUnit* pUnit;
	bool bLoadOK;
	while( (pXMLUnit = pXMLData->IterateChildren( pXMLUnit ))!= NULL ) {
		pUnit = new CUnit;
		bLoadOK = pUnit->VLoad( pXMLUnit->ToElement() );
		if ( bLoadOK == false ) {
			return false;
		}
		AddUnit( pUnit );
	}

	return true;
}

TiXmlElement CUnitManager::VGetSaveData()
{
	TiXmlElement pXMLUnits( "units" );

	for ( unsigned int i = 0; i < m_vpUnits.size(); ++i ) {
		pXMLUnits.InsertEndChild( m_vpUnits[ i ]->VGetSaveData() );
	}

    return pXMLUnits;
}

void CUnitManager::AddUnit( CUnit* pUnit )
{
	m_vpUnits.push_back( pUnit );
}

CUnit* CUnitManager::GetUnit( unsigned int iIndex )
{
	if ( m_vpUnits.size() == 0 || iIndex > m_vpUnits.size() )
		return NULL;

	return m_vpUnits[ iIndex ];
}

void CUnitManager::VThink( const int& iElapsedTime )
{
	for ( unsigned int i = 0; i < m_vpUnits.size(); ++i )
	{
		m_vpUnits[ i ]->VThink(  iElapsedTime );
	}
}

void CUnitManager::VRender( SDL_Surface* pDestSurface, SDL_Rect* m_View )
{
	for ( unsigned int i = 0; i < m_vpUnits.size(); ++i )
	{
		m_vpUnits[ i ]->VRender( pDestSurface, m_View );
	}
}
