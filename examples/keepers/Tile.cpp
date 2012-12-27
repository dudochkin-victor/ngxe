/*
 * Tile.cpp
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#include "Tile.h"

CTile::CTile()
{

}

CTile::~CTile()
{

}

bool CTile::VLoad( TiXmlElement* pXMLData )
{
	//TODO: Вот если не грузанулось то вернуть false
	CEntity::VLoad( pXMLData );

	int iTileX = atoi( pXMLData->Attribute( "x" ) );
	int iTileY = atoi( pXMLData->Attribute( "y" ) );
	SetTile( iTileX, iTileY );

	return true;
}

TiXmlElement CTile::VGetSaveData()
{
	TiXmlElement Tile( "tile" );
	Tile.SetAttribute( "x", GetTileX() );
	Tile.SetAttribute( "y", GetTileY() );

	Tile.InsertEndChild( CEntity::VGetSaveData() );

	return Tile;
}

void CTile::VThink( const int& iElapsedTime )
{
	CEntity::VThink( iElapsedTime );
}

void CTile::VRender( SDL_Surface* pDestSurface, SDL_Rect* m_View )
{
	CEntity::VRender( pDestSurface, m_View );
}
