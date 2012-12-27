#ifndef ENTITY_H
#define ENTITY_H

// определить размер тайла
static const int tileSizeX = 75;
static const int tileSizeY = 19;

#include "Interfaces.h"

/** Base entity class */
class CEntity: public IBaseEntity
{
private:
	int		m_iPosX;
	int		m_iPosY;
	SDL_Surface*	m_pSurface;
	bool		m_bVisible;
	int		m_iAlpha;
	string		m_sImage;
	int		m_iTileX;
	int		m_iTileY;

	void SetPosX	( int iPosX )			{ m_iPosX = iPosX; };
	void SetPosY	( int iPosY )			{ m_iPosY = iPosY; };
	void SetPos	( int iPosX, int iPosY )	{ SetPosX( iPosX ); SetPosY( iPosY ); }

protected:
	virtual bool VLoadImageFromFile( const string& sFile);
	void SetAlpha	( int iAlpha )		{ m_iAlpha = iAlpha; }
	void SetVisible	( bool bVisible )	{ m_bVisible = bVisible; }

	SDL_Rect GetRect();

public:
	CEntity();
	virtual ~CEntity();

	virtual void		VThink		( const int& iElapsedTime );
	virtual void		VRender		( SDL_Surface* pDestSurface, SDL_Rect* m_View );

	virtual bool		VLoad		( TiXmlElement* pXMLData );
	virtual TiXmlElement	VGetSaveData();

	//void SetTileX		( int iTileX )		{ m_iTileX = iTileX; SetPosX( iTileX * tileSize ); };
	void SetTileX		( int iTileX )		{ m_iTileX = iTileX; SetPosX( iTileX * tileSizeX+(m_iTileY%2)*tileSizeX/2 ); };
	void SetTileY		( int iTileY )		{
		m_iTileY = iTileY;
		SetPosY( iTileY * tileSizeY );
		SetTileX(m_iTileX);
	};
	void SetTile		( int iTileX, int iTileY )	{ SetTileX( iTileX ); SetTileY( iTileY ); }

	int	GetTileX	()				{ return m_iTileX; }
	int	GetTileY	()				{ return m_iTileY; }

	int GetPosX	()			{ return m_iPosX; }
	int GetPosY	()			{ return m_iPosY; }

	int 	GetHeight	()				{ return m_pSurface->h; }
	int 	GetWidth	()				{ return m_pSurface->w; }
	int 	GetBottom	()				{ return GetPosY() + GetHeight(); }
	int 	GetRight	() 				{ return GetPosX() + GetWidth(); }

	int	GetAlpha	() 				{ return m_iAlpha; }
	bool 	GetVisible	() 				{ return m_bVisible; }
};

#endif // ENTITY_H
