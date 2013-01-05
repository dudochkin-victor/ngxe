//
// Class to load images from DDS files
//
// Author: Alex V. Boreskoff
//

#ifndef	__DDS_LOADER__
#define	__DDS_LOADER__

#include	"TextureLoader.h"

struct	DDS_HEADER;

class	DdsLoader : public TextureLoader
{
public:
	DdsLoader () : TextureLoader () {}

	virtual	Texture * load ( Data * data );
	bool	  		  loadCubemap ( Data * data, Texture * images [6] );

protected:
	Texture * loadUncompressed ( Data * data, const DDS_HEADER& ddsd, int numComponents );
};

#endif
