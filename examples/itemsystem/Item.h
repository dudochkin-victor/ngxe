#ifndef ITEM_H
#define ITEM_H

class cItem {
	unsigned long type;
public:
	cItem( unsigned long );
	cItem( );
	cItem( const cItem & );
	cItem & operator= ( const cItem & );
	bool operator== ( const cItem & ) const;
	bool operator< ( const cItem & ) const;
	unsigned long getID( ) const;
};

#endif