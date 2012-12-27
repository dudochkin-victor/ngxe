/*
 * Game.cpp
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#include "Game.h"

CGame::CGame()
{

}

CGame::~CGame()
{

}

void CGame::VThink( const int& iElapsedTime )
{
	m_oTileManager.VThink( iElapsedTime );
	m_OUnitManager.VThink( iElapsedTime );
}

/**
 * рендерим только видимую часть
 */
void CGame::VRender( SDL_Surface* pDestSurface )
{
	m_oTileManager.VRender( pDestSurface, &m_View );
	m_OUnitManager.VRender( pDestSurface, &m_View );
}

bool CGame::VLoad( TiXmlElement* pXMLData )
{
	if ( m_oTileManager.VLoad( pXMLData->FirstChildElement( "tiles" ) ) == false )
		return false;

	if ( m_OUnitManager.VLoad( pXMLData->FirstChildElement( "units" ) ) == false )
		return false;

	return true;
}

TiXmlElement CGame::VGetSaveData()
{
	TiXmlElement GameElm( "map" );
	GameElm.InsertEndChild( m_oTileManager.VGetSaveData() );
	GameElm.InsertEndChild( m_OUnitManager.VGetSaveData() );

	return GameElm;
}
