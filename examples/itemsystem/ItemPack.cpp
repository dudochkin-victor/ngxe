#include "ItemPack.h"

cItemPack::cItemPack( void ) { }

cItemPack::cItemPack( cItem & i, unsigned long a ) {
	contents[i] = a;
}
void cItemPack::clear( void ) {
	contents.clear( );
}
unsigned long cItemPack::add( const cItem & i, const unsigned long a ) {
	return( contents[i] += a );
}
unsigned long cItemPack::remove( const cItem & i, const unsigned long a ) {
	unsigned long t = contents[i];
	if( a > t ) { contents[i] = 0; return( a-t ); }
	else { contents[i] = t-a; return( 0 ); }
}
unsigned long cItemPack::getAmount( const cItem & i ) const {
	std::map<cItem,unsigned long>::const_iterator j;
	j = contents.find( i );
	if( j == contents.end( ) ) { return( 0 ); }
	else { return( j->second ); }
}
cItemPack & cItemPack::operator=( const cItemPack & o ) {
	contents = o.contents;
	return( *this );
}
cItemPack & cItemPack::operator+=( const cItemPack & o ) {
	std::map<cItem,unsigned long>::const_iterator i;
	for( i = o.contents.begin( ); i != o.contents.end( ); ++i ) {
		add( i->first, i->second );
	}
	return( *this );
}
cItemPack cItemPack::operator+( const cItemPack & o ) const {
	return( cItemPack(*this) += o );
}
const std::map<cItem,unsigned long> & cItemPack::getItems( void ) const {
	return( contents );
}

