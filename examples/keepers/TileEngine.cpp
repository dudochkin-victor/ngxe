/*
 * TileEngine.cpp
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#include "TileEngine.h"
#include "Game.h"
#include "Unit.h"

CGame* g_pGame;

CTileEngine::CTileEngine()
{
	SetTitle( "TileEngine - Loading Core..." );

	// Set the game window size to 5 x 4 images of tileSize x tileSize pixels
	SetSize( 3 * tileSizeX, 16 * tileSizeY );
}

CTileEngine::~CTileEngine()
{
	// Nothing
}

void CTileEngine::AdditionalInit()
{
	SetTitle( "TileEngine - Loading..." );
	g_pGame = new CGame;
    LoadGame( "Media/TestTile.xml" );
    g_pGame->SetViewPoint(36,19);
    g_pGame->SetViewSize(3 * tileSizeX, 16 * tileSizeY);
	SetTitle( "TileEngine" );
}

void CTileEngine::End()
{
	SetTitle( "TileEngine - Quiting..." );

	SaveGame( "savegame.xml" );

	if ( g_pGame != 0 )
		delete g_pGame;
	g_pGame = 0;
}

void CTileEngine::Think( const int& iElapsedTime )
{
	g_pGame->VThink( iElapsedTime );
}

void CTileEngine::Render( SDL_Surface* pDestSurface )
{
	g_pGame->VRender( pDestSurface );
}

void CTileEngine::KeyDown(const int& iKeyEnum)
{

}


void CTileEngine::KeyUp(const int& iKeyEnum)
{

}

void CTileEngine::MouseMoved(const int& iButton, const int& iX, const int& iY, const int& iRelX, const int& iRelY)
{

}

void CTileEngine::MouseButtonUp(const int& iButton, const int& iX, const int& iY, const int& iRelX, const int& iRelY)
{

}

void CTileEngine::MouseButtonDown(const int& iButton, const int& iX, const int& iY, const int& iRelX, const int& iRelY)
{
	/*int iUnit = (iButton == SDL_BUTTON_RIGHT ? 1 : 0);
	CUnit* pUnit = g_pGame->GetUnitManager()->GetUnit( iUnit );
	if (pUnit!=0)
	{
		pUnit->SetTileX( (iX - iX % tileSizeX) / tileSizeX );
		pUnit->SetTileY( (iY - iY % tileSizeY) / tileSizeY );
	}*/
	g_pGame->SetViewPoint(iX+36,iY+19);
}

bool CTileEngine::LoadGame( const string& sFile )
{
	TiXmlDocument doc;
	if ( doc.LoadFile( sFile.c_str() ) == false ) {
		return false;
	}

	return g_pGame->VLoad( doc.FirstChildElement( "map" ) );
}

bool CTileEngine::SaveGame( const string& sFile )
{
	TiXmlDocument doc;

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	doc.InsertEndChild( g_pGame->VGetSaveData() );

	return doc.SaveFile( sFile.c_str() );
}
