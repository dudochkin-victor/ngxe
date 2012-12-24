#include "main.h"
#include "ItemDatabase.h"
#include "ItemPack.h"
#include "Item.h"

#include <iostream>

const char c_quit = 'q';
const char c_inventory = 'i';
const char c_empty = 'e';
const char c_description = 'd';
const char c_add = 'a';
const char c_remove = 'r';

using namespace std;

cItemPack inventory;

int main( void ) {
	cItemDatabase::initialize( string( "../data/items.txt" ) );
	mainMenu( );
	cItemDatabase::unload( );
	return 0;
}

void mainMenu( void ) {

	cout << "Welcome " << endl << endl;
	
	char command = '\0';

	while( command != c_quit ) {

		cout << endl << "------------------------------------------------------------------- " << endl;
		cout << endl << "Please select an operation : " << endl;
		cout << c_inventory << " : display your inventory" << endl;
		cout << c_empty << " : empty your inventory" << endl;
		cout << c_description << " : look closer at an item" << endl;
		cout << c_add << " : add an item your inventory" << endl;
		cout << c_remove << " : drop an item" << endl;
		cout << c_quit << " : leave the item manager" << endl;

		cin >> command;

		switch( command ) {
		case c_quit : break;
		case c_inventory : displayInventory( ); break;
		case c_empty : 
			inventory = cItemPack( );
			cout << endl << "Your inventory is now empty" << endl;
			break;
		case c_description : displayDescription( ); break;
		case c_add : addObject( ); break;
		case c_remove : dropObject( ); break;
		default: cout << endl << "Sorry, this is not a recognized command" << endl;
		}
	}

	cout << endl << "Good bye." << endl;
}

void displayInventory( void ) { 
	map<cItem,unsigned long>::iterator i;
	map<cItem,unsigned long> item_list = inventory.getItems( );

	cout << endl << "You have";

	bool hasItems = false;

	for( i = item_list.begin( ); i != item_list.end( ); ++i ) {
		if( i->second > 0 ) {
			hasItems = true;
			sItemData s = cItemDatabase::getData( i->first );
			cout << endl << s.name << " x" << i->second;;
		}
	}

	if( !hasItems ) {
		cout << " no items right now!";
	}

	cout << endl;
}
void displayDescription( void ) { 

	std::string name;
	cItem object( 0 );
	sItemData info;

	cout << endl << "What item do you want to look closer at?" << endl;
	cin >> name;

	try {
		object = cItemDatabase::create( name );
		if( inventory.getAmount( object ) == 0 ) {
			cout << endl << "Sorry, you do not have this item in your inventory!" << endl;
		} else {
			info = cItemDatabase::getData( object );
			cout << endl << info.name << endl << info.description << endl;
			cout << "Value : " << info.value << " gold coins; Weight : " << info.weight << " stones" << endl;
		}
	}
	catch( eDatabaseError e ) {
		cout << endl << "Sorry, you do not have this item in your inventory!" << endl;
	}
}
void addObject( void ) { 

	string name;
	cItem object( 0 );

	cout << endl << "What item do you want to add to your inventory?" << endl;
	cout << endl << "(choices: cube, paper, scissors, coin)" << endl;
	cin >> name; 

	try {
		object = cItemDatabase::create( name );
		inventory.add( object, 1 );
		cout << endl << "Object added!";
	}
	catch( eDatabaseError e ) {
		cout << endl << "This object does not exist!" << endl;
	}
}
void dropObject( void ) { 

	string name;
	cItem object( 0 );

	cout << endl << "What item do you want to drop?" << endl;
	cin >> name; 

	try {
		object = cItemDatabase::create( name );
		if( inventory.remove( object, 1 ) == 0 ) {
		cout << endl << "Object dropped!" << endl;
		} else {
		cout << endl << "Sorry, you do not have this item in your inventory!" << endl;
		}
	}
	catch( eDatabaseError e ) {
		cout << endl << "Sorry, you do not have this item in your inventory!" << endl;
	}
}
