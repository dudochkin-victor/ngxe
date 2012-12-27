/*
 * Unit.cpp
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#include "Unit.h"

CUnit::CUnit()
{

}

CUnit::~CUnit()
{

}

bool CUnit::VLoad( TiXmlElement* pXMLData )
{
	CEntity::VLoad( pXMLData );

	m_iPlayer = atoi( pXMLData->Attribute( "player" ) );

	TiXmlElement* XMLPosition = pXMLData->FirstChildElement( "position" );
	int iTileX = atoi( XMLPosition->FirstChild( "x" )->FirstChild()->Value() );
	int iTileY = atoi( XMLPosition->FirstChild( "y" )->FirstChild()->Value() );
	SetTile( iTileX, iTileY );

	return true;
}

TiXmlElement CUnit::VGetSaveData()
{
	TiXmlElement UnitElm( "unit" );
	UnitElm.SetAttribute( "player", m_iPlayer );
	UnitElm.InsertEndChild( CEntity::VGetSaveData() );
	TiXmlElement PositionElm( "position" );

	char buf[8];

	TiXmlElement PosXElm( "x" );
	sprintf( buf, "%d", GetTileX() );
	TiXmlText PosXTxt( buf );
	PosXElm.InsertEndChild( PosXTxt );
	PositionElm.InsertEndChild( PosXElm );

	TiXmlElement PosYElm( "y" );
	sprintf( buf, "%d", GetTileY() );
	TiXmlText PosYTxt( buf );
	PosYElm.InsertEndChild( PosYTxt );
	PositionElm.InsertEndChild( PosYElm );

	UnitElm.InsertEndChild( PositionElm );

	return UnitElm;
}

void CUnit::VThink( const int& iElapsedTime )
{
	CEntity::VThink( iElapsedTime );
}

void CUnit::VRender( SDL_Surface* pDestSurface, SDL_Rect* m_View )
{
	CEntity::VRender( pDestSurface, m_View );
}
