#ifndef FREE_NEHE_H
#define FREE_NEHE_H

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

//OpenGL Headers 
#include <GL/gl.h>
#include <GL/glu.h>

//Some STL headers
#include <vector>
#include <string>

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
namespace freetype {

//Inside of this namespace, give ourselves the ability
//to write just "vector" instead of "std::vector"
using std::vector;

//Ditto for string.
using std::string;

//This holds all of the information related to any
//freetype font that we want to create.  
struct font_data {
	float h;			///< Holds the height of the font.
	GLuint * textures;	///< Holds the texture id's 
	GLuint list_base;	///< Holds the first display list id

	//The init function will create a font of
	//of the height h from the file fname.
	void init(const char * fname, unsigned int h);

	//Free all the resources assosiated with the font.
	void clean();
};

}

#endif