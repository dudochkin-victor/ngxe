#ifndef ITEMPACKH
#define ITEMPACKH

#include "Item.h"
#include <map>

class cItemPack {
	std::map< cItem, unsigned long > contents;
public:
	cItemPack( );
	cItemPack( cItem & , unsigned long );
	void clear( );
	unsigned long add( const cItem & , const unsigned long );
	unsigned long remove( const cItem & , const unsigned long );
	unsigned long getAmount( const  cItem & ) const;
	const std::map< cItem, unsigned long > & getItems( ) const;
	cItemPack & operator= ( const cItemPack & );
	cItemPack & operator+= ( const cItemPack & );
	cItemPack operator+ ( const cItemPack & ) const;
};

#endif