#include "ItemDatabase.h"

#include <deque>
#include <fstream>

namespace cItemDatabase {

	std::deque< sItemData > item_database_entries;
	bool item_database_initialized = false;
};

using namespace cItemDatabase;

const sItemData & cItemDatabase::getData( const cItem & i ) { 
	if( item_database_initialized ) { 
		unsigned long type = i.getID( );
		if( type >= item_database_entries.size( ) ) {
			throw IDBERR_INVALID_ITEM;
		}
		else {
			return( item_database_entries[i.getID( )] );
		}
	}
	else { throw IDBERR_NOT_INITIALIZED; }
}
cItem cItemDatabase::create( const std::string & s ) {
	if( !item_database_initialized ) { throw IDBERR_NOT_INITIALIZED; }
	unsigned long i;
	for( i = item_database_entries.size( )-1; i >= 0; --i ) {
		if( item_database_entries[i].name == s ) { return( cItem(i) ); }
	}
	throw IDBERR_INVALID_NAME;
}
void cItemDatabase::initialize( const std::string & s ) {

	item_database_entries.clear( );

	//FILE LOADING SEQUENCE

	std::fstream file;
	file.open( s.c_str( ), std::ios::in );

	unsigned long item_count, current_item = 0;
	file >> item_count;

	sItemData item;
	char buffer[256];

	for( current_item = 0; current_item < item_count; ++current_item ) {
		file.getline(buffer, 256, '%');
		file.getline(buffer, 256, '%');
		item.name = buffer;
		file.getline(buffer, 256, '%');
		file.getline(buffer, 256, '%');
		item.description = buffer;
		file >> item.value;
		file >> item.weight;
		item_database_entries.push_back(item);
	}
	//END OF FILE LOADING SEQUENCE

	item_database_initialized = true;
}
void cItemDatabase::unload( void ) { 
	item_database_entries.clear( );
	item_database_initialized = false; 
}



