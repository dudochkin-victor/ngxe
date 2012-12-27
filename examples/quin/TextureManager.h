///////////////////////////////////////////////////////////////////////////////
// TextureManager.h : Interface for the TextureManager class.
//  Loads textures into OpenGL, and can retrieve their texture identifiers
// based on character strings. Provides a central texture repository which
// handles texture reloading when re-initialising OpenGL.

// Author: Quin Pendragon (dragon@graduate.uwa.edu.au)
// Date:   2003-01-05

#ifndef TextureManager_h
#define TextureManager_h

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	//Method:  LoadTexture()
	//Purpose: Retrieves an OpenGL texture identifier for the given image
	//         filename. Loads the texture if necessary.
	GLuint LoadTexture(const char *texname);

	//Method:  AddTexture(const char *name, const GLuint texID)
	//Purpose: Adds a texture ID (or any other GLuint value you care to specify)
	//         to the TextureManager. If the name is already used, its value
	//         will be overwritten, otherwise a new entry will be added.
	void AddTexture(const char *name, const GLuint texID);

	//Method:  GetTexture(const char *name)
	//Purpose: Retrieves the texture with the given name from the TextureManager, or
	//         0 if no texture with the given name is found.
	GLuint GetTexture(const char *name);

	//Method:  Reset()
	//Purpose: Clears the list of loaded texture identifiers, so that next time
	//         they are required they will be reloaded.
	void Reset();

private:
	void *m_texturelist;
};

#endif // TextureManager_h
