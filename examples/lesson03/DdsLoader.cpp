//
// Class to load images from DDS files,
// based on ogl_dds_texture_loader.cpp by Kevin Harris (kevin@codesampler.com)
//
// Author: Alex V. Boreskoff
//

#include    <stdio.h>

#ifdef  _WIN32
    #include    <windows.h>
#endif

#ifdef	MACOSX
	#include	<OpenGL/gl.h>
	#include	<OpenGL/glu.h>
	#include	<OpenGL/glext.h>
	#include	<GLUT/glut.h>
#else
	#include    <GL/glew.h>
	#include    <GL/gl.h>
	#include    <GL/glu.h>
	#include    <GL/glut.h>
#endif

#include    <string.h>
#include    "Data.h"
#include    "DdsLoader.h"
#include    "CompressedTexture.h"
#include    "DdsDefs.h"

const unsigned long FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
const unsigned long FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
const unsigned long FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

Texture * DdsLoader :: load ( Data * data )
{
    DDS_HEADER      ddsd;
    char            filecode [4];
    int             factor;
    int             bufferSize;
    int             format;

                                        // Verify the file is a true .dds file
    data -> seekAbs  ( 0 );
    data -> getBytes ( filecode, 4 );

    if( strncmp( filecode, "DDS ", 4 ) != 0 )
        return NULL;

                                        // Get the surface descriptor
    data -> getBytes ( &ddsd, sizeof ( ddsd ) );

    int numComponents = 0;

    if ( ddsd.ddspf.dwFlags & DDS_ALPHA_PIXELS )        // ARGB
        numComponents = 4;
    else                                                // RGB
        numComponents = 3;

    if ( (ddsd.ddspf.dwFlags & DDS_FOURCC) == 0 )       // not compressed
    {
    	return loadUncompressed ( data, ddsd, numComponents );
    }

    //
    // This .dds loader supports the loading of compressed formats DXT1, DXT3
    // and DXT5.
    //

    int numBlocks = ((ddsd.dwWidth + 3)/4) * ((ddsd.dwHeight + 3)/4);   // number of 4*4 texel blocks
    int blockSize = 0;

    switch ( ddsd.ddspf.dwFourCC )
    {
        case FOURCC_DXT1:
                            // DXT1's compression ratio is 8:1
            if ( numComponents == 4 )
                format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            else
                format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;

            factor    = 2;
            blockSize = numBlocks * 8;                              // get size of byte image (8 bytes per block)
            break;

        case FOURCC_DXT3:
                            // DXT3's compression ratio is 4:1
            format     = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            factor     = 4;
            blockSize  = numBlocks * 16;                            // get size of byte image (16 bytes per block)
            break;

        case FOURCC_DXT5:
                            // DXT5's compression ratio is 4:1
            format    = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            factor    = 4;
            blockSize = numBlocks * 16;                         // get size of byte image (16 bytes per block)
            break;

        default:
            return NULL;
    }

                                            // check for RGBA-only formats
    if ( format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT )
        numComponents = 4;
    else
        numComponents = 3;

    if( ddsd.dwPitchOrLinearSize != 0 )
        if( ddsd.dwMipMapCount > 1 )
            bufferSize = ddsd.dwPitchOrLinearSize * factor;
        else
            bufferSize = ddsd.dwPitchOrLinearSize;
    else
        bufferSize = blockSize;

    int mipMapsCount = ddsd.dwMipMapCount;

    if ( mipMapsCount < 1 )
        mipMapsCount = 1;

    CompressedTexture * texture = new CompressedTexture ( ddsd.dwWidth, ddsd.dwHeight, numComponents, format, mipMapsCount, bufferSize );

    data -> getBytes ( texture -> getData (), bufferSize );

    return texture;
}

bool	DdsLoader :: loadCubemap ( Data * data, Texture * images [6] )
{
    DDS_HEADER      ddsd;
    char            filecode [4];
    int             factor;
    int             bufferSize;
    int             format;
    int				i;
                                        // Verify the file is a true .dds file
    data -> seekAbs  ( 0 );
    data -> getBytes ( filecode, 4 );

    if( strncmp ( filecode, "DDS ", 4 ) != 0 )
        return false;

                                        // Get the surface descriptor
    data -> getBytes ( &ddsd, sizeof ( ddsd ) );

    int numComponents = 0;

    if ( ddsd.ddspf.dwFlags & DDS_ALPHA_PIXELS )        // ARGB
        numComponents = 4;
    else                                                // RGB
        numComponents = 3;

	if ( (ddsd.dwFlags & DDS_CAPS) == 0 )				// for cubemap must have caps fields
		return false;

	if ( (ddsd.dwCaps1 & (DDS_TEXTURE | DDS_COMPLEX)) != (DDS_TEXTURE | DDS_COMPLEX) )
		return false;

	if ( (ddsd.dwCaps2 & DDS_CUBEMAP) == 0 )
		return false;

	int	sides [6] = {
		DDS_CUBEMAP_POSITIVEX, DDS_CUBEMAP_NEGATIVEX,
		DDS_CUBEMAP_POSITIVEY, DDS_CUBEMAP_NEGATIVEY,
		DDS_CUBEMAP_POSITIVEZ, DDS_CUBEMAP_NEGATIVEZ
	};

    int mipMapsCount = ddsd.dwMipMapCount;

    if ( mipMapsCount < 1 )
        mipMapsCount = 1;

    if ( (ddsd.ddspf.dwFlags & DDS_FOURCC) == 0 )       // not compressed
    {
    	for ( i = 0; i < 6; i++ )
    		if ( ddsd.dwCaps2 & sides [i] )
		    	images [i] = loadUncompressed ( data, ddsd, numComponents );
		    else
		    	images [i] = NULL;

		return true;
    }

    //
    // This .dds loader supports the loading of compressed formats DXT1, DXT3
    // and DXT5.
    //

    int numBlocks = ((ddsd.dwWidth + 3)/4) * ((ddsd.dwHeight + 3)/4);   // number of 4*4 texel blocks
    int blockSize = 0;

    switch ( ddsd.ddspf.dwFourCC )
    {
        case FOURCC_DXT1:
                            // DXT1's compression ratio is 8:1
            if ( numComponents == 4 )
                format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            else
                format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;

            factor    = 2;
            blockSize = numBlocks * 8;                              // get size of byte image (8 bytes per block)
            break;

        case FOURCC_DXT3:
                            // DXT3's compression ratio is 4:1
            format        = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            numComponents = 4;
            factor        = 4;
            blockSize     = numBlocks * 16;                         // get size of byte image (16 bytes per block)
            break;

        case FOURCC_DXT5:
                            // DXT5's compression ratio is 4:1
            format        = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            numComponents = 4;
            factor        = 4;
            blockSize     = numBlocks * 16;                         // get size of byte image (16 bytes per block)
            break;

        default:
            return NULL;
    }

    if( ddsd.dwPitchOrLinearSize != 0 )
        bufferSize = ddsd.dwPitchOrLinearSize;
    else
        bufferSize = blockSize;

	int	w = (int)ddsd.dwWidth;
	int	h = (int)ddsd.dwHeight;
	int	blocks;

	for ( i = 1; i < mipMapsCount; i++ )
	{
		w /= 2;
		h /= 2;

		if ( w < 1 )
			w = 1;

		if ( h < 1 )
			h = 1;

		blocks      = ((w + 3)/4) * ((h + 3)/4);   							// number of 4*4 texel blocks
		bufferSize += blocks * factor * 4;
	}

	for ( i = 0; i < 6; i++ )
		if ( ddsd.dwCaps2 & sides [i] )
		{
			images [i] = new CompressedTexture ( ddsd.dwWidth, ddsd.dwHeight, numComponents, format, mipMapsCount, bufferSize );

	    	data -> getBytes ( images [i] -> getData (), bufferSize );
	    }
	    else
	    	images [i] = NULL;

    return true;
}

Texture * DdsLoader :: loadUncompressed ( Data * data, const DDS_HEADER& ddsd, int numComponents )
{
	int	w            = (int)ddsd.dwWidth;
	int	h            = (int)ddsd.dwHeight;
	int bytesPerLine = w * numComponents;
	int	i;

    if ( (bytesPerLine & 3) != 0 )                  // do dword alignment
    	bytesPerLine += 4 - (bytesPerLine & 3);

    byte    * buf     = new byte [bytesPerLine];
    Texture * texture = new Texture ( w, h, numComponents );

    for ( i = 0; i < h; i++ )
    {
    	data -> getBytes ( buf, bytesPerLine );

                                                        // rearrange components
        byte * dest = texture -> getData () + i * w * numComponents;
        byte * src  = buf;

        for ( register int j = 0; j < w; j++ )
        {
        	dest [0] = src [2];                     // red
            dest [1] = src [1];                     // green
            dest [2] = src [0];                     // blue

            if ( numComponents == 4 )
            	dest [3] = src [3];                 // alpha

            dest += numComponents;
            src  += numComponents;
        }
    }

													// do mipmap adjustement
	for ( i = 1; i < ddsd.dwMipMapCount; i++ )
	{
		w /= 2;
		h /= 2;

		if ( w < 1 )
			w = 1;

		if ( h < 1 )
			h = 1;

		bytesPerLine = w * numComponents;

	    if ( (bytesPerLine & 3) != 0 )                // do dword alignment
    		bytesPerLine += 4 - (bytesPerLine & 3);

    	data -> seekCur ( bytesPerLine * h );
	}

	delete buf;

    return texture;
}

