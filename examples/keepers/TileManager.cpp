/*
 * TileManager.cpp
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#include "TileManager.h"
#include "Tile.h"

CTileManager::CTileManager()
{
	pDefaultTile = new CEntity;
}

CTileManager::~CTileManager()
{
	while( m_vvpTiles.size() > 0 )
	{
		while ( m_vvpTiles.back().size() > 0 )
		{
			if ( m_vvpTiles.back().back() != 0 ) {
				delete m_vvpTiles.back().back();
			}
			m_vvpTiles.back().pop_back();
		}
		m_vvpTiles.pop_back();
	}
}

void CTileManager::AddTile( CTile* pTile )
{
	if ( (unsigned int)pTile->GetTileX() < m_vvpTiles.size() && (unsigned int)pTile->GetTileY() < m_vvpTiles[pTile->GetTileX()].size() )
		m_vvpTiles[ pTile->GetTileY() ][ pTile->GetTileX() ] = pTile;
}
/**
 * отображаем только то что входит в область отображения
 */
void CTileManager::VRender( SDL_Surface* pDestSurface, SDL_Rect* m_View )
{
	for ( unsigned int y = 0; y < m_vvpTiles.size(); ++y )
	{
		for ( unsigned int x = 0; x < m_vvpTiles[ y ].size(); ++x ) {
			if ( m_vvpTiles[y][x] != 0 )
				m_vvpTiles[y][x]->VRender( pDestSurface, m_View );
			else {
			/**
			 * здесь надо заполнить область отображения тайлом
			 * по умлочанию с учетом смещения
			 */
				pDefaultTile->SetTile(x,y);
				pDefaultTile->VRender( pDestSurface, m_View );
			}
		}
	}
}

void CTileManager::VThink( const int& iElapsedTime )
{

}


bool CTileManager::VLoad( TiXmlElement* pXMLData )
{
	/**
	 * читаем ширину и высоту карты
	 */
	width = atoi( pXMLData->Attribute( "width" ) );
	height = atoi( pXMLData->Attribute( "height" ) );

	m_vvpTiles.resize( height );

	for( unsigned int y = 0; y < (unsigned int)height; ++y )
	{
		m_vvpTiles[y].resize(width);
	}
	/**
	 * Нужно прочитать спрайт по умолчанию
	 * <defaultTile>
	 * <image>Media/default.bmp</image>
	 * </defaultTile>
	 */
	bool bLoadOK;
	TiXmlNode* pXMLTile = 0;
	CTile* pTile;

	pXMLTile = pXMLData->FirstChildElement( "defaultTile" );

	bLoadOK = pDefaultTile->VLoad( pXMLTile->ToElement() );

	if ( bLoadOK == false ) {
		return false;
	}


	while( (pXMLTile = pXMLData->IterateChildren( pXMLTile ))!=NULL ) {
		pTile = new CTile;
		bLoadOK = pTile->VLoad( pXMLTile->ToElement() );
		if ( bLoadOK == false ) {
			return false;
		}
		AddTile( pTile );
	}

	return true;
}

TiXmlElement CTileManager::VGetSaveData()
{
	TiXmlElement pXMLTiles( "tiles" );

	for ( unsigned int y = 0; y < m_vvpTiles.size(); ++y )
	{
		for ( unsigned int x = 0; x < m_vvpTiles[ y ].size(); ++x ) {
			if ( m_vvpTiles[y][x] != 0 )
				pXMLTiles.InsertEndChild( m_vvpTiles[y][x]->VGetSaveData() );
		}
	}

    return pXMLTiles;
}

CTile* CTileManager::GetTile( const int& iX, const int& iY )
{
	if ( (iX < 0 || (unsigned int)iX > m_vvpTiles.size()) && (iY < 0 || (unsigned int)iY > m_vvpTiles[0].size()) )
        return 0;

	return m_vvpTiles[iY][iX];
}
