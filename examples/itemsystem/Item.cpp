#include "Item.h"

cItem::cItem( void ) : type( 0 ) { }
cItem::cItem( unsigned long t ) : type( t ) { }
unsigned long cItem::getID( void ) const { return( type ); }

cItem::cItem( const cItem &copy ) : type( copy.type ) { }
bool cItem::operator ==( const cItem &i ) const { return( type == i.type ); }
bool cItem::operator <( const cItem &i ) const { return( type < i.type ); }
cItem &cItem::operator =( const cItem &copy ) {
	type = copy.type;
	return( *this );
}

