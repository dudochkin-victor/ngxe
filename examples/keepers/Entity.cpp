/*
 * Entity.cpp
 *
 *  Created on: 29.12.2009
 *      Author: blacksmith
 */

#include "Entity.h"

CEntity::CEntity()
{
	m_pSurface	= 0;
	SetVisible( true );
	SetPos( 0, 0 );
	SetTile( 0, 0 );
	SetAlpha( 255 );
}


CEntity::~CEntity()
{
	if ( m_pSurface != 0 )
		SDL_FreeSurface( m_pSurface );
}

bool CEntity::VLoad( TiXmlElement* pXMLData )
{
	string sImage = pXMLData->FirstChild( "image" )->FirstChild()->Value();
	bool bLoadOK = VLoadImageFromFile( sImage );
	return bLoadOK;
}

TiXmlElement CEntity::VGetSaveData()
{
	TiXmlElement ImageElm( "image" );
	TiXmlText ImageTxt( m_sImage.c_str() );
	ImageElm.InsertEndChild( ImageTxt );

	return ImageElm;
}

void CEntity::VThink( const int& iElapsedTime )
{

}

void CEntity::VRender( SDL_Surface *pDestSurface, SDL_Rect* m_View ) {
    if ( ( m_pSurface == 0 ) || ( m_bVisible == false) || ( m_iAlpha == 0 ) ) // If the surface is invalid or it's 100% transparent.
		return;

	/**
	 * проверяем входит ли нижняя граница за минусом дельты и
	 * верхняя с плюсом дельты в область отображения
	 */
	if ((this->GetRight() >= m_View->x) &&
		(this->GetPosX()<=m_View->x+m_View->w) &&
		(this->GetBottom() >= m_View->y)&&
		(this->GetPosY()<=m_View->y+m_View->h)
		)
	{

		SDL_Rect SDestRect;
		SDestRect.x = m_iPosX-m_View->x;
		SDestRect.y = m_iPosY-m_View->y;
		SDestRect.w = m_pSurface->w;
		SDestRect.h = m_pSurface->h;

		if ( m_iAlpha != 255 )
			SDL_SetAlpha( m_pSurface, SDL_SRCALPHA, m_iAlpha );

		SDL_BlitSurface( m_pSurface, &m_pSurface->clip_rect, pDestSurface, &SDestRect );
	}
}

bool CEntity::VLoadImageFromFile( const string& sFile) {
	if ( m_pSurface != 0 )
		SDL_FreeSurface( m_pSurface );

	SDL_Surface *pTempSurface;
	pTempSurface = SDL_LoadBMP( sFile.c_str() );

	m_sImage = sFile;

	if ( pTempSurface == 0 )
	{
		char czError[256];
		sprintf( czError, "Image '%s' could not be opened. Reason: %s", sFile.c_str(), SDL_GetError() );
		fprintf( stderr, "\nERROR: %s\n", czError );
		return false;
	}
	else
	{
		if( SDL_SetColorKey( pTempSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB( pTempSurface->format, 255, 0, 255) ) == -1 )
		{
			char czWarning[256];
			sprintf( czWarning, "Image '%s' can't be color keyed. Reason: %s", sFile.c_str(), SDL_GetError() );
			fprintf( stderr, "\nWARNING: %s\n", czWarning );
		}
	}

    m_pSurface	= pTempSurface;

	return true;
}

SDL_Rect CEntity::GetRect()
{
	SDL_Rect stTemp = { GetPosX(), GetPosY(), GetWidth(), GetHeight() };
	return stTemp;
}
