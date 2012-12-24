#ifndef ITEMDATABASEH
#define ITEMDATABASEH

#include "Item.h"
#include <string>

struct sItemData {
	std::string name, description;
	unsigned long value, weight;
};

enum eDatabaseError {
	IDBERR_NOT_INITIALIZED,
	IDBERR_INVALID_ITEM,
	IDBERR_INVALID_NAME,
};

namespace cItemDatabase {

	const sItemData & getData( const cItem & );
	cItem create( const std::string & );
	void initialize( const std::string & );
	void unload( void );
};

#endif
