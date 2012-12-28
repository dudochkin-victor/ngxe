//////////////////////////////////////////////////////////////////////////////////////////
//	jpeg.cpp
//	Load jpeg image
//	This is not part of the common code since it requires a jpeg library and headers
//	Downloaded from: www.paulsprojects.net
//	Created:	8th August 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include <GL/gl.h>
#include "IMAGE.h"
#include "jpeg.h"
#include "Util.h"

//Load a jpg texture into image
bool LoadJPG(IMAGE * image, char * filename)
{
	Util::log("Loading %s in LoadJPG()", filename);

	//Clear the data in image
	if(image->data)
		delete [] image->data;
	image->data=NULL;
	image->bpp=0;
	image->format=0;
	image->height=0;
	image->width=0;


	struct jpeg_decompress_struct cinfo;

	FILE * file = fopen(filename, "rb");				//Open the JPG file
	
	if(file == NULL)								//Does the file exist?
	{
		Util::log("%s does not exist.", filename);
		return false;
	}

	//Create an error handler
	jpeg_error_mgr jerr;

	//point the compression object to the error handler
	cinfo.err=jpeg_std_error(&jerr);

	//Initialize the decompression object
	jpeg_create_decompress(&cinfo);

	//Specify the data source
	jpeg_stdio_src(&cinfo, file);

	
	
	//Decode the jpeg data into the image
	//Read in the header
	jpeg_read_header(&cinfo, true);

	//start to decompress the data
	jpeg_start_decompress(&cinfo);

	//get the number of color channels
	int channels=cinfo.num_components;

	//Fill in class variables
	image->bpp=channels*8;
	image->width=cinfo.image_width;
	image->height=cinfo.image_height;

	if(image->bpp==24)
		image->format=GL_RGB;
	if(image->bpp==32)
		image->format=GL_RGBA;

	//Allocate memory for image
	image->data=new GLubyte[image->width*image->height*channels];
	if(!image->data)
	{
		Util::log("Unable to allocate memory for temporary texture data");
		return false;
	}
	
	//Create an array of row pointers
	unsigned char ** rowPtr = new unsigned char * [image->height];
	if(!rowPtr)
	{
		Util::log("Unable to allocate memory for row pointers");
		return false;
	}

	for(unsigned int i=0; i<image->height; ++i)
		rowPtr[i]=&(image->data[i*image->width*channels]);

	//Extract the pixel data
	int rowsRead=0;
	while(cinfo.output_scanline < cinfo.output_height)
	{
		//read in this row
		rowsRead+=jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	}

	//release memory used by jpeg
	jpeg_destroy_decompress(&cinfo);

	fclose(file);

	//delete row pointers
	if(rowPtr)
		delete [] rowPtr;
	rowPtr=NULL;

	//Flip the texture vertically
	image->FlipVertically();

	Util::log("%s loaded successfully", filename);
	return true;
}
