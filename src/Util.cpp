/*
 * Util.cpp
 *
 *  Created on: 28.12.2012
 *      Author: blacksmith
 *  TODO:
 *  For image handling
 *
 *  1. Need make a refactoring of code
 *  2. Optimize for SSE
 *  3. Only one public function must be ::loadTexture(...)
 *  4. GPL code must be removed
 *
 */

#include "Util.h"

extern "C"
{
	#include "jpeglib.h"
}
#include "png.h"
#include "stdio.h"
#include "malloc.h"

Util* Util::_self = 0;

Util::Util() : logfile(NULL) {
	// TODO Auto-generated constructor stub

}

Util::~Util() {
	// TODO Auto-generated destructor stub
}

//Universal bitmap identifier
const short BITMAP_ID=0x4D42;

struct BitMap {
	short Type;
	long Size;
	short Reserve1;
	short Reserve2;
	long OffBits;
	long biSize;
	long biWidth;
	long biHeight;
	short biPlanes;
	short biBitCount;
	long biCompression;
	long biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	long biClrUsed;
	long biClrImportant;
};

struct BITMAPFILEHEADER {
	short bfType;
	int bfSize;
	short Reserved1;
	short Reserved2;
	int bfOffBits;
};

struct BITMAPINFOHEADER {
	int biSize;
	long biWidth;
	long biHeight;
	short biPlanes;
	short biBitCount;
};

////Load - load a texture from a file
//bool Load(const char * filename) {
//	//Clear the data if already used
//	if (data)
//		delete[] data;
//	data = NULL;
//	bpp = 0;
//	width = 0;
//	height = 0;
//	format = 0;
//
//	int filenameLength = strlen(filename);
//
//	if (strncmp((filename + filenameLength - 3), "BMP", 3) == 0
//			|| strncmp((filename + filenameLength - 3), "bmp", 3) == 0)
//		return LoadBMP(filename);
//
//	if (strncmp((filename + filenameLength - 3), "PCX", 3) == 0
//			|| strncmp((filename + filenameLength - 3), "pcx", 3) == 0)
//		return LoadPCX(filename);
//
//	if (strncmp((filename + filenameLength - 3), "TGA", 3) == 0
//			|| strncmp((filename + filenameLength - 3), "tga", 3) == 0)
//		return LoadTGA(filename);
//
//	Util::log("%s does not end in \".tga\", \".bmp\" or \"pcx\"",
//			filename);
//	return false;
//}
//
//bool LoadBMP(const char * filename) {
//	FILE * file;											//the texture file
//	BITMAPFILEHEADER fileHeader;							//bitmap file header
//	BITMAPINFOHEADER infoHeader;							//bitmap info header
//
//	//open file for reading
//	file = fopen(filename, "rb");
//	if (file == NULL) {
//		Util::log("Unable to open %s", filename);
//		return false;
//	}
//
//	//read the file header
//	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
//
//	//check it's a bitmap
//	if (fileHeader.bfType != BITMAP_ID) {
//		fclose(file);
//		Util::log("%s is not a legal .BMP", filename);
//		return false;
//	}
//
//	//read in the information header
//	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
//
//	//close the file
//	fclose(file);
//
//	//discover the bpp
//	if (infoHeader.biBitCount == 24)
//		return Load24BitBMP(filename);
//	if (infoHeader.biBitCount == 8)
//		return Load8BitBMP(filename);
//
//	Util::log("%s has an unknown bpp", filename);
//	return false;
//}
//
////Load24BitBMP - load a 24 bit bitmap file
//bool Load24BitBMP(const char * filename) {
//	Util::log("Loading %s in Load24bitBMP()", filename);
//
//	//set bpp and format
//	bpp = 24;
//	format = GL_RGB;
//
//	FILE * file;											//the texture file
//	BITMAPFILEHEADER fileHeader;							//bitmap file header
//	BITMAPINFOHEADER infoHeader;							//bitmap info header
//
//	//open file for reading
//	file = fopen(filename, "rb");
//	if (file == NULL) {
//		Util::log("Unable to open %s", filename);
//		return false;
//	}
//
//	//read the file header
//	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
//
//	//check it's a bitmap
//	if (fileHeader.bfType != BITMAP_ID) {
//		fclose(file);
//		Util::log("%s is not a legal .BMP", filename);
//		return false;
//	}
//
//	//read in the information header
//	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
//
//	//set size
//	width = infoHeader.biWidth;
//	height = infoHeader.biHeight;
//
//	//calculate the stride in bytes: width*bpp/8 plus padding bytes at the end of each row
//	unsigned int stride = width * bpp / 8;
//	if (width % 4 == 1)
//		stride += 1;
//	if (width % 4 == 2)
//		stride += 2;
//	if (width % 4 == 3)
//		stride += 3;
//
//	//point file to the beginning of the data
//	fseek(file, fileHeader.bfOffBits, SEEK_SET);
//
//	//allocate space for the image data
//	data = new unsigned char[stride * height];
//	if (!data) {
//		fclose(file);
//		Util::log("Unable to allocate memory for %s", filename);
//		return false;
//	}
//
//	//read in the data
//	fread(data, 1, stride * height, file);
//
//	//close the file
//	fclose(file);
//
//	//data is in BGR format
//	//swap b and r
//	for (unsigned int row = 0; row < height; row++) {
//		for (unsigned int i = 0; i < width * 3; i += bpp / 8) {
//			//repeated XOR to swap bytes 0 and 2
//			data[(row * stride) + i] ^= data[(row * stride) + i + 2] ^=
//					data[(row * stride) + i] ^= data[(row * stride) + i + 2];
//		}
//	}
//
//	Util::log("Loaded %s correctly.", filename);
//	return true;
//}
//
////Load8BitBMP - load an 8 bit paletted bitmap file
//bool Load8BitBMP(const char * filename) {
//	Util::log("Loading %s in Load8bitBMP()", filename);
//
//	//set bpp and format
//	bpp = 24; //after conversion
//	format = GL_RGB;
//
//	FILE * file;											//the texture file
//	BITMAPFILEHEADER fileHeader;							//bitmap file header
//	BITMAPINFOHEADER infoHeader;							//bitmap info header
//
//	//open file for reading
//	file = fopen(filename, "rb");
//	if (file == NULL) {
//		Util::log("Unable to open %s", filename);
//		return false;
//	}
//
//	//read the file header
//	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
//
//	//check it's a bitmap
//	if (fileHeader.bfType != BITMAP_ID) {
//		fclose(file);
//		Util::log("%s is not a legal .BMP", filename);
//		return false;
//	}
//
//	//read in the information header
//	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
//
//	//set size
//	width = infoHeader.biWidth;
//	height = infoHeader.biHeight;
//
//	//make space for palette
//	unsigned char * palette = new unsigned char[256 * 4];
//	if (!palette) {
//		Util::log("Unable to alllocate memory for palette");
//		return false;
//	}
//
//	//load the palette
//	fread(palette, 256 * 4, 1, file);
//
//	//point file to the beginning of the data
//	fseek(file, fileHeader.bfOffBits, SEEK_SET);
//
//	//calculate the stride in bytes between one row and the next
//	unsigned int stride = width;
//	if (width % 4 != 0)
//		stride += 4 - width % 4;
//
//	//allocate space for color indices
//	unsigned char * indices = new unsigned char[stride * height];
//	if (!indices) {
//		Util::log("Unable to allocate memory for indices");
//		return false;
//	}
//
//	//load indices
//	fread(indices, 1, stride * height, file);
//
//	//close the file
//	fclose(file);
//
//	//allocate space for the image data
//	data = new unsigned char[stride * height * bpp / 8];
//	if (!data) {
//		fclose(file);
//		Util::log("Unable to allocate memory for %s", filename);
//		return false;
//	}
//
//	//calculate the color values - keeping the padding colors
//	for (unsigned int currentRow = 0; currentRow < height; currentRow++) {
//		for (unsigned int i = 0; i < stride; i++) {
//			data[(currentRow * stride + i) * 3 + 0] = palette[indices[currentRow
//					* stride + i] * 4 + 2];
//			data[(currentRow * stride + i) * 3 + 1] = palette[indices[currentRow
//					* stride + i] * 4 + 1];
//			data[(currentRow * stride + i) * 3 + 2] = palette[indices[currentRow
//					* stride + i] * 4 + 0];								//BGR
//		}
//	}
//
//	Util::log("Loaded %s correctly.", filename);
//	return true;
//}
//
////LoadPCX - load a .pcx texture - 256 color, paletted
//bool LoadPCX(const char * filename) {
//	Util::log("Loading %s in LoadPCX()", filename);
//
//	//set bpp and format
//	bpp = 24;
//	format = GL_RGB;
//
//	FILE * file;
//
//	file = fopen(filename, "rb");
//	if (!file) {
//		Util::log("Unable to open %s", filename);
//		return false;
//	}
//
//	//retrieve header, first 4 bytes, first 2 should be 0x0A0C
//	unsigned short header[2];
//	fread(header, 4, 1, file);
//
//	if (header[0] != 0x050A) {
//		Util::log("%s is not a legal .PCX file", filename);
//		fclose(file);
//		return false;
//	}
//
//	//retrieve minimum x value
//	int xMin = fgetc(file);		//loword
//	xMin |= fgetc(file) << 8;	//hiword
//
//	//retrieve minimum y value
//	int yMin = fgetc(file);		//loword
//	yMin |= fgetc(file) << 8;	//hiword
//
//	//retrieve maximum x value
//	int xMax = fgetc(file);		//loword
//	xMax |= fgetc(file) << 8;	//hiword
//
//	//retrieve maximum y value
//	int yMax = fgetc(file);		//loword
//	yMax |= fgetc(file) << 8;	//hiword
//
//	//calculate width and height
//	width = xMax - xMin + 1;
//	height = yMax - yMin + 1;
//
//	//allocate memory for pixel data (paletted)
//	unsigned char * pixelData = new unsigned char[width * height];
//	if (!pixelData) {
//		Util::log(
//				"Unable to allocate %d bytes for the image data of %s",
//				width * height, filename);
//		fclose(file);
//		return false;
//	}
//
//	//set file pointer to beginning of image data
//	fseek(file, 128, SEEK_SET);
//
//	//decode and store the pixel data
//	unsigned int index = 0;
//
//	while (index < (width * height)) {
//		int c = getc(file);
//
//		if (c > 0xBF) {
//			int numRepeat = 0x3F & c;
//			c = getc(file);
//
//			for (int i = 0; i < numRepeat; i++)
//				pixelData[index++] = c;
//		} else
//			pixelData[index++] = c;
//
//		fflush(stdout);
//	}
//
//	//allocate memory for the image palette
//	unsigned char * paletteData = new unsigned char[768];
//
//	//the palette is the last 769 bytes of the file
//	fseek(file, -769, SEEK_END);
//
//	//retrieve first character, should be equal to 12
//	int c = getc(file);
//	if (c != 12) {
//		Util::log(
//				"%s is not a legal .PCX file - the palette data has an illegal header, %d",
//				filename, c);
//		fclose(file);
//		return false;
//	}
//
//	//read and store the palette
//	fread(paletteData, 1, 768, file);
//
//	//close the file
//	fclose(file);
//
//	//allocate memory for the "unpaletted" data
//	data = new unsigned char[width * height * 3];
//	if (!data) {
//		Util::log(
//				"Unable to allocate memory for the expanded data of %s",
//				filename);
//		return false;
//	}
//
//	//calculate the "unpaletted" data - "flipping" the texture top-bottom
//	for (unsigned int j = 0; j < height; j++) {
//		for (unsigned int i = 0; i < width; i++) {
//			data[3 * (j * width + i)] = (unsigned char) paletteData[3
//					* pixelData[(height - 1 - j) * width + i]];
//			data[3 * (j * width + i) + 1] = (unsigned char) paletteData[3
//					* pixelData[(height - 1 - j) * width + i] + 1];
//			data[3 * (j * width + i) + 2] = (unsigned char) paletteData[3
//					* pixelData[(height - 1 - j) * width + i] + 2];
//		}
//	}
//
//	Util::log("Loaded %s correctly.", filename);
//	return true;
//}
//
////Load a TGA texture
//bool LoadTGA(const char * filename) {
//	unsigned char UncompressedTGAHeader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
//			0 };
//	unsigned char CompressedTGAHeader[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0,
//			0 };
//	unsigned char Uncompressed8BitTGAHeader[12] = { 0, 1, 1, 0, 0, 0, 1, 24, 0,
//			0, 0, 0 };
//
//	unsigned char TGAcompare[12];					//Used to compare TGA header
//
//	FILE * file = fopen(filename, "rb");		//Open the TGA file
//
//	if (file == NULL)							//Does the file exist?
//	{
//		if (filename)
//			Util::log("%s does not exist", filename);
//		return false;
//	}
//
//	//read the header
//	fread(TGAcompare, 1, sizeof(TGAcompare), file);
//	fclose(file);
//
//	if (memcmp(UncompressedTGAHeader, TGAcompare, sizeof(UncompressedTGAHeader))
//			== 0) {
//		return LoadUncompressedTrueColorTGA(filename);
//	} else if (memcmp(CompressedTGAHeader, TGAcompare,
//			sizeof(CompressedTGAHeader)) == 0) {
//		return LoadCompressedTrueColorTGA(filename);
//	} else if (memcmp(Uncompressed8BitTGAHeader, TGAcompare,
//			sizeof(Uncompressed8BitTGAHeader)) == 0) {
//		return LoadUncompressed8BitTGA(filename);
//	} else {
//		Util::log("%s is not a recognised type of TGA", filename);
//		return false;
//	}
//
//	return false;
//}
//
////load an 8 bit uncompressed paletted TGA
//bool LoadUncompressed8BitTGA(const char * filename) {
//	unsigned char TGAHeader[12] = { 0, 1, 1, 0, 0, 0, 1, 24, 0, 0, 0, 0 };
//	unsigned char TGAcompare[12];					//Used to compare TGA header
//	unsigned char header[6];				//First 6 useful bytes of the header
//
//	Util::log("Loading %s in LoadUncompressed8BitTGA()", filename);
//
//	FILE * file = fopen(filename, "rb");				//Open the TGA file
//
//	if (file == NULL)								//Does the file exist?
//	{
//		Util::log("%s does not exist.", filename);
//		return false;
//	}
//
//	if (fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||//Are there 12 bytes to read?
//			memcmp(TGAHeader, TGAcompare, sizeof(TGAHeader)) != 0 ||//Is the header correct?
//			fread(header, 1, sizeof(header), file) != sizeof(header))//Read next 6 bytes
//					{
//		fclose(file);				//If anything else failed, close the file
//		Util::log("Could not load %s correctly, general failure.",
//				filename);
//		return false;
//	}
//
//	//save data into class member variables
//	width = header[1] * 256 + header[0];			//determine the image width
//	height = header[3] * 256 + header[2];				//determine image height
//	bpp = header[4];
//
//	if (width <= 0 ||											//if width <=0
//			height <= 0 ||										//or height<=0
//			bpp != 8)												//bpp not 8
//					{
//		fclose(file);											//close the file
//		Util::log(
//				"%s's height or width is less than zero, or the TGA is not 8 bpp.",
//				filename);
//		return false;
//	}
//
//	//set format
//	format = GL_RGB;
//
//	//make space for palette
//	unsigned char * palette = new unsigned char[256 * 3];
//	if (!palette) {
//		Util::log("Unable to allocate memory for palette");
//		return false;
//	}
//
//	//load the palette
//	fread(palette, 256 * 3, 1, file);
//
//	//allocate space for color indices
//	unsigned char * indices = new unsigned char[width * height];
//	if (!indices) {
//		Util::log("Unable to allocate memory for indices");
//		return false;
//	}
//
//	//load indices
//	fread(indices, 1, width * height, file);
//
//	//close the file
//	fclose(file);
//
//	//allocate space for the image data
//	data = new unsigned char[width * height * 3];
//	if (!data) {
//		fclose(file);
//		Util::log("Unable to allocate memory for %s", filename);
//		return false;
//	}
//
//	//calculate the color values
//	for (unsigned int currentRow = 0; currentRow < height; currentRow++) {
//		for (unsigned int i = 0; i < width; i++) {
//			data[(currentRow * width + i) * 3 + 0] = palette[indices[currentRow
//					* width + i] * 3 + 2];
//			data[(currentRow * width + i) * 3 + 1] = palette[indices[currentRow
//					* width + i] * 3 + 1];
//			data[(currentRow * width + i) * 3 + 2] = palette[indices[currentRow
//					* width + i] * 3 + 0];								//BGR
//		}
//	}
//
//	Util::log("Loaded %s correctly.", filename);
//	return true;
//}
//
////load an uncompressed TGA texture (24 or 32 bpp)
//bool LoadUncompressedTrueColorTGA(const char * filename) {
//	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };//Uncompressed TGA header
//	unsigned char TGAcompare[12];					//Used to compare TGA header
//	unsigned char header[6];				//First 6 useful bytes of the header
//	unsigned int bytesPerPixel;						//bytes per pixel
//	unsigned int imageSize;						//Stores Image size when in RAM
//
//	Util::log("Loading %s in LoadUncompressedTGA()", filename);
//
//	FILE * file = fopen(filename, "rb");				//Open the TGA file
//
//	if (file == NULL)								//Does the file exist?
//	{
//		Util::log("%s does not exist.", filename);
//		return false;
//	}
//
//	if (fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||//Are there 12 bytes to read?
//			memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||//Is the header correct?
//			fread(header, 1, sizeof(header), file) != sizeof(header))//Read next 6 bytes
//					{
//		fclose(file);				//If anything else failed, close the file
//		Util::log("Could not load %s correctly, general failure.",
//				filename);
//		return false;
//	}
//
//	//save data into class member variables
//	width = header[1] * 256 + header[0];			//determine the image width
//	height = header[3] * 256 + header[2];				//determine image height
//	bpp = header[4];
//
//	if (width <= 0 ||											//if width <=0
//			height <= 0 ||										//or height<=0
//			bpp != 24 && bpp != 32)							//bpp not 24 or 32
//					{
//		fclose(file);											//close the file
//		Util::log(
//				"%s's height or width is less than zero, or the TGA is not 24 or 32 bpp.",
//				filename);
//		return false;
//	}
//
//	//set format
//	if (bpp == 24)
//		format = GL_RGB;
//	else
//		format = GL_RGBA;
//
//	bytesPerPixel = bpp / 8;							//calc bytes per pixel
//	imageSize = width * height * bytesPerPixel;			//calc memory required
//
//	data = new unsigned char[imageSize];	//reserve the memory for the data
//
//	if (data == NULL)							//Does the storage memory exist?
//	{
//		Util::log("Unable to allocate memory for %s image",
//				filename);
//		fclose(file);
//		return false;
//	}
//
//	//read in the image data
//	if (fread(data, 1, imageSize, file) != imageSize)//Does the image size match the required?
//			{															//If not
//		if (data)												//If data loaded
//			delete[] data;										//free memory
//		Util::log("Could not read %s image data", filename);
//		fclose(file);											//close file
//		return false;
//	}
//
//	fclose(file);
//
//	//data is in BGR format
//	//swap b and r
//	for (int i = 0; i < (int) imageSize; i += bytesPerPixel) {
//		//repeated XOR to swap bytes 0 and 2
//		data[i] ^= data[i + 2] ^= data[i] ^= data[i + 2];
//	}
//
//	Util::log("Loaded %s correctly.", filename);
//	return true;
//}
//
////load a compressed TGA texture (24 or 32 bpp)
//bool LoadCompressedTrueColorTGA(const char * filename) {
//	unsigned char TGAheader[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };//Compressed TGA header
//	unsigned char TGAcompare[12];					//Used to compare TGA header
//	unsigned char header[6];				//First 6 useful bytes of the header
//	unsigned int bytesPerPixel;						//bytes per pixel
//	unsigned int imageSize;						//Stores Image size when in RAM
//
//	Util::log("Loading %s in LoadCompressedTGA()", filename);
//
//	FILE * file = fopen(filename, "rb");				//Open the TGA file
//
//	if (file == NULL)								//Does the file exist?
//	{
//		Util::log("%s does not exist.", filename);
//		return false;
//	}
//
//	if (fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||//Are there 12 bytes to read?
//			memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||//Is the header correct?
//			fread(header, 1, sizeof(header), file) != sizeof(header))//Read next 6 bytes
//					{
//		fclose(file);				//If anything else failed, close the file
//		Util::log("Could not load %s correctly, general failure.",
//				filename);
//		return false;
//	}
//
//	//save data into class member variables
//	width = header[1] * 256 + header[0];			//determine the image width
//	height = header[3] * 256 + header[2];				//determine image height
//	bpp = header[4];
//
//	if (width <= 0 ||											//if width <=0
//			height <= 0 ||										//or height<=0
//			bpp != 24 && bpp != 32)							//bpp not 24 or 32
//					{
//		fclose(file);											//close the file
//		Util::log(
//				"%s's height or width is less than zero, or the TGA is not 24 or 32 bpp.",
//				filename);
//		return false;
//	}
//
//	//set format
//	if (bpp == 24)
//		format = GL_RGB;
//	else
//		format = GL_RGBA;
//
//	bytesPerPixel = bpp / 8;							//calc bytes per pixel
//	imageSize = width * height * bytesPerPixel;			//calc memory required
//
//	data = new unsigned char[imageSize];	//reserve the memory for the data
//	if (!data)									//Does the storage memory exist?
//	{
//		Util::log("Unable to allocate memory for %s image",
//				filename);
//		fclose(file);
//		return false;
//	}
//
//	//read in the image data
//	int pixelCount = height * width;
//	int currentPixel = 0;
//	int currentByte = 0;
//	unsigned char * colorBuffer = new unsigned char[bytesPerPixel];
//
//	do {
//		unsigned char chunkHeader = 0;
//
//		if (fread(&chunkHeader, sizeof(unsigned char), 1, file) == 0) {
//			Util::log("Could not read RLE chunk header");
//			if (file)
//				fclose(file);
//			if (data)
//				delete[] data;
//			return false;
//		}
//
//		if (chunkHeader < 128)	//Read raw color values
//				{
//			chunkHeader++;
//
//			for (short counter = 0; counter < chunkHeader; counter++) {
//				if (fread(colorBuffer, 1, bytesPerPixel, file)
//						!= bytesPerPixel) {
//					Util::log("Unable to read %s image data",
//							filename);
//
//					if (file)
//						fclose(file);
//
//					if (colorBuffer)
//						delete[] colorBuffer;
//
//					if (data)
//						delete[] data;
//
//					return false;
//				}
//
//				//transfer pixel color to data (swapping r and b values)
//				data[currentByte] = colorBuffer[2];
//				data[currentByte + 1] = colorBuffer[1];
//				data[currentByte + 2] = colorBuffer[0];
//
//				if (bytesPerPixel == 4)
//					data[currentByte + 3] = colorBuffer[3];
//
//				currentByte += bytesPerPixel;
//				currentPixel++;
//
//				if (currentPixel > pixelCount) {
//					Util::log("Too many pixels read");
//					if (file)
//						fclose(file);
//					if (colorBuffer)
//						delete[] colorBuffer;
//					if (data)
//						delete[] data;
//					return false;
//				}
//			}
//		} else	//chunkHeader>=128
//		{
//			chunkHeader -= 127;
//
//			if (fread(colorBuffer, 1, bytesPerPixel, file) != bytesPerPixel) {
//				Util::log("Unable to read %s image data", filename);
//
//				if (file)
//					fclose(file);
//				if (colorBuffer)
//					delete[] colorBuffer;
//				if (data)
//					delete[] data;
//				return false;
//			}
//
//			for (short counter = 0; counter < chunkHeader; counter++) {
//				//transfer pixel color to data (swapping r and b values)
//				data[currentByte] = colorBuffer[2];
//				data[currentByte + 1] = colorBuffer[1];
//				data[currentByte + 2] = colorBuffer[0];
//
//				if (bytesPerPixel == 4)
//					data[currentByte + 3] = colorBuffer[3];
//
//				currentByte += bytesPerPixel;
//				currentPixel++;
//
//				if (currentPixel > pixelCount) {
//					Util::log("Too many pixels read");
//					if (file)
//						fclose(file);
//					if (colorBuffer)
//						delete[] colorBuffer;
//					if (data)
//						delete[] data;
//					return false;
//				}
//			}
//		}
//	} while (currentPixel < pixelCount);
//
//	fclose(file);
//
//	Util::log("Loaded %s correctly.", filename);
//	return true;
//}
//
////load in an 8 bit greyscale TGA as an alpha channel
//bool LoadAlphaTGA(const char * filename) {
//	unsigned char TGAHeader[12] = { 0, 1, 1, 0, 0, 0, 1, 24, 0, 0, 0, 0 };
//	unsigned char TGAcompare[12];					//Used to compare TGA header
//	unsigned char header[6];				//First 6 useful bytes of the header
//
//	Util::log("Loading %s in LoadAlphaTGA()", filename);
//
//	if (!(format == GL_RGB || format == GL_RGBA)) {
//		Util::log(
//				"Can only load an alpha channel to RGB / RGBA format images. %s caused error",
//				filename);
//		return false;
//	}
//
//	FILE * file = fopen(filename, "rb");				//Open the TGA file
//
//	if (file == NULL)								//Does the file exist?
//	{
//		Util::log("%s does not exist.", filename);
//		return false;
//	}
//
//	if (fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||//Are there 12 bytes to read?
//			memcmp(TGAHeader, TGAcompare, sizeof(TGAHeader)) != 0 ||//Is the header correct?
//			fread(header, 1, sizeof(header), file) != sizeof(header))//Read next 6 bytes
//					{
//		fclose(file);				//If anything else failed, close the file
//		Util::log("Could not load %s correctly, general failure.",
//				filename);
//		return false;
//	}
//
//	//save data into class member variables
//	unsigned int alphaWidth = header[1] * 256 + header[0];//determine the image width
//	unsigned int alphaHeight = header[3] * 256 + header[2];	//determine image height
//	int alphaBpp = header[4];
//
//	if (alphaWidth <= 0 ||										//if width <=0
//			alphaHeight <= 0 ||								//or height<=0
//			alphaBpp != 8)											//bpp not 8
//					{
//		fclose(file);											//close the file
//		Util::log(
//				"%s's height or width is less than zero, or the TGA is not 8 bpp.",
//				filename);
//		return false;
//	}
//
//	//check it is the same size as the image
//	if (alphaWidth != width || alphaHeight != height) {
//		Util::log("%s is not the same size as the color texture",
//				filename);
//		return false;
//	}
//
//	//make space for palette
//	unsigned char * palette = new unsigned char[256 * 3];
//	if (!palette) {
//		Util::log("Unable to allocate memory for palette");
//		return false;
//	}
//
//	//load the palette
//	fread(palette, 256 * 3, 1, file);
//
//	//we dont use the palette
//	delete[] palette;
//	palette = NULL;
//
//	//allocate space for alpha values
//	unsigned char * values = new unsigned char[width * height];
//	if (!values) {
//		Util::log("Unable to allocate memory for alpha values");
//		return false;
//	}
//
//	//load indices
//	fread(values, 1, alphaWidth * alphaHeight, file);
//
//	//close the file
//	fclose(file);
//
//	//now put in the alpha data
//	if (format == GL_RGBA) {
//		for (unsigned int i = 0; i < width * height; i++) {
//			data[i * 4 + 3] = values[i];
//		}
//	} else if (format == GL_RGB) {
//		unsigned char * tempData = new unsigned char[width * height * 4];
//		if (!tempData) {
//			Util::log(
//					"Unable to allocate memory for Temporary Data");
//			return false;
//		}
//
//		for (unsigned int i = 0; i < width * height; i++) {
//			tempData[i * 4 + 0] = data[i * 3 + 0];
//			tempData[i * 4 + 1] = data[i * 3 + 1];
//			tempData[i * 4 + 2] = data[i * 3 + 2];
//			tempData[i * 4 + 3] = values[i];
//		}
//
//		//update member variables
//		bpp = 32;
//		format = GL_RGBA;
//
//		if (data)
//			delete[] data;
//		data = tempData;
//	}
//
//	Util::log("Loaded %s correctly.", filename);
//	return true;
//}
//


/*
 * save jpeg images
 */
int save_jpeg(const char *name, ngxTexture &tex, int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *file = fopen(name,"wb");
    if(!file)
    {
        fprintf(stderr,"Texture::load_jpeg(): error create \"%s\" file\n",name);
        return 0;
    }
    unsigned char *data_buffer = new unsigned char[tex.width * tex.height * 3];
    for(int i = 0, j = 0; i < tex.width * tex.height * 4; i += 4, j += 3)
    {
        data_buffer[j + 0] = tex.data[i + 0];
        data_buffer[j + 1] = tex.data[i + 1];
        data_buffer[j + 2] = tex.data[i + 2];
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo,file);
    cinfo.image_width = tex.width;
    cinfo.image_height = tex.height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo,quality,TRUE);
    jpeg_start_compress(&cinfo,TRUE);
    int row_stride = tex.width * 3;
    while(cinfo.next_scanline < cinfo.image_height)
    {
        JSAMPROW row_pointer;
        row_pointer = &data_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo,&row_pointer,1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    delete [] data_buffer;
    fclose(file);
    return 1;
}
/*
 * GPL
 */
unsigned char *LoadTGA(char *name,int *width,int *height) {
    unsigned char   rep,*data,*buffer,*ptr,info[18];
    int Width,Height,Components,Size,i,j,k,l;
    FILE    *FileTGA = fopen(name,"rb");
    if(!FileTGA) return 0;
    fread(&info,1,18,FileTGA);  // Read header
    Width = info[12] + info[13] * 256;
    Height = info[14] + info[15] * 256;
    switch(info[16]) {      // Read only 32 && 24 bit per pixel
        case 32:
            Components = 4; // 32 bit per pixel (RGBA)
            break;
        case 24:
            Components = 3; // 24 bit per pixel (RGB)
            break;
        default:
            fclose(FileTGA);
            return 0;
    }
    Size = Width * Height * Components;
    buffer = (unsigned char*)malloc(Size);  // Buffer for RGB or RGBA image
    data = (unsigned char*)malloc(Width * Height * 4);  // Output RGBA image
    if(!data || !buffer) {
        fclose(FileTGA);
        return 0;
    }
    fseek(FileTGA,info[0],SEEK_CUR);
    i = 0;
    ptr = buffer;
    switch(info[2]) {
        case 2:     // Unmapped RGB image
            fread(buffer,1,Size,FileTGA);
            break;
        case 10:    // Run length encoded
            while(i < Size) {
                fread(&rep,1,1,FileTGA);
                if(rep & 0x80) {
                    rep ^= 0x80;
                    fread(ptr,1,Components,FileTGA);
                    ptr += Components;
                    for(j = 0; j < rep * Components; j++) {
                        *ptr = *(ptr - Components);
                        ptr ++;
                    }
                    i += Components * (rep + 1);
                }
                else {
                    k = Components * (rep + 1);
                    fread(ptr,1,k,FileTGA);
                    ptr += k;
                    i += k;
                }
            }
            break;
        default:
            fclose(FileTGA);
            free(buffer);
            free(data);
            return 0;
    }
    for(i = 0, j = 0; i < Size; i += Components, j += 4) {  // BGR -> RGBA
        data[j] = buffer[i + 2];
        data[j + 1] = buffer[i + 1];
        data[j + 2] = buffer[i];
        if(Components == 4) data[j + 3] = buffer[i + 3];
        else data[j + 3] = 255;
    }
    if(!(info[17] & 0x20))
        for(j = 0, k = Width * 4; j < Height / 2; j ++)
            for(i = 0; i < Width * 4; i ++) {
                l = data[j * k + i];
                data[j * k + i] = data[(Height - j - 1) * k + i];
                data[(Height - j - 1) * k + i] = l;
            }
    fclose(FileTGA);
    free(buffer);
    *width = Width;
    *height = Height;
    return data;
}

/* Check to see if a file is a PNG file using png_sig_cmp().  png_sig_cmp()
 * returns zero if the image is a PNG and nonzero if it isn't a PNG.
 *
 * The function check_if_png() shown here, returns nonzero (true)
 * if the file can be opened and is a PNG, 0 (false) otherwise.
 *
 * If this call is successful, and you are going to keep the file open,
 * you should call png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK); once
 * you have created the png_ptr, so that libpng knows your application
 * has read that many bytes from the start of the file.  Make sure you
 * don't call png_set_sig_bytes() with more than 8 bytes read or give it
 * an incorrect number of bytes read, or you will either have read too
 * many bytes (your fault), or you are telling libpng to read the wrong
 * number of magic bytes (also your fault).
 *
 * Many applications already read the first 2 or 4 bytes from the start
 * of the image to determine the file type, so it would be easiest just
 * to pass the bytes to png_sig_cmp() or even skip that if you know
 * you have a PNG file, and call png_set_sig_bytes().
 */
#define PNG_BYTES_TO_CHECK 8
int check_if_png(const char *file_name, FILE **fp)
{
	unsigned char buffer[PNG_BYTES_TO_CHECK];

	/* Open the prospective PNG file. */
	if ((*fp = fopen(file_name, "rb")) == NULL)
		return -1;

	/* Read in some of the signature bytes */
	if (fread(buffer, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK)
		return -1;

	/* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
	 * Return nonzero (true) if they match */
	return(png_sig_cmp(buffer, (png_size_t)0, PNG_BYTES_TO_CHECK));
}

/* Read a PNG file.  You may want to return an error code if the read
 * fails (depending upon the failure).
 */
int load_png(const char *file_name, ngxTexture &tex)
{
    png_structp png_ptr;
    png_infop info_ptr, end_info;
    unsigned int sig_read = PNG_BYTES_TO_CHECK;
    int bit_depth, color_type, interlace_type;
    FILE *fp;
	int rowbytes;
	png_bytep *row_pointers;
	int row;
	png_uint_32 w, h;

	if (check_if_png(file_name, &fp))
		return (-1);

	/* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also supply the
     * the compiler header file version, so that we know if the application
     * was compiled with a compatible version of the library.  REQUIRED
     */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	                                 NULL, NULL);

	if (png_ptr == NULL)
	{
		fclose(fp);
		return (-1);
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	end_info = png_create_info_struct(png_ptr);
	if (!info_ptr || !end_info)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL,
								(png_infopp)NULL);
		return (-1);
	}


	/* Set error handling if you are using the setjmp/longjmp method (this is
     * the normal method of doing things with libpng).  REQUIRED unless you
     * set up your own error handlers in the png_create_read_struct() earlier.
     */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,
								(png_infopp)NULL);
		fclose(fp);
		return (-1);
	}

	/* Set up the input control if you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);


    /* OK, you're doing it the hard way, with the lower-level functions */

    /* The call to png_read_info() gives us all of the information from the
     * PNG file before the first IDAT (image data chunk).  REQUIRED
     */
    png_read_info(png_ptr, info_ptr);


    png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth,
	             &color_type, &interlace_type, NULL, NULL);

	tex.width = w;
	tex.height = h;


	/**** Set up the data transformations you want.  Note that these are all
	 **** optional.  Only call them if you want/need them.  Many of the
	 **** transformations only work on specific types of images, and many
	 **** are mutually exclusive.
	 ****/
	if (color_type == PNG_COLOR_TYPE_PALETTE)
               png_set_palette_to_rgb(png_ptr);

	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

//	if(htons(1)!=1)
//		png_set_swap(png_ptr);

	/* Update images info for the transformations */
	png_read_update_info(png_ptr, info_ptr);

	/* Allocate the memory to hold the image using the fields of info_ptr. */
	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	row_pointers = (png_bytep *) png_malloc(png_ptr, tex.height * sizeof(png_bytep));
	tex.data = (unsigned char *) malloc(tex.height * rowbytes);

	for (row = 0; row<tex.height; row++)
		row_pointers[tex.height-row-1] = (png_bytep) &((tex.data)[row * rowbytes]);


	/* Now it's time to read the image.  One of these methods is REQUIRED */
	png_read_image(png_ptr, row_pointers);

	/* read rest of file, and get additional chunks in info_ptr - REQUIRED */
	png_read_end(png_ptr, end_info);

	/* free libpng allocated memory */
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

	/* free row pointers */
	png_free(png_ptr, row_pointers);

    switch (color_type) {
		case PNG_COLOR_TYPE_GRAY:
			tex.format = GL_LUMINANCE;
			switch (bit_depth) {
				case 1:
				case 2:
				case 4:
				case 8:
					tex.type = GL_UNSIGNED_BYTE;
					break;
				case 16:
					tex.type = GL_UNSIGNED_SHORT;
					break;
			}
			break;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
			tex.format = GL_LUMINANCE_ALPHA;
		        switch (bit_depth) {
					case 1:
					case 2:
					case 4:
					case 8:
						tex.type = GL_UNSIGNED_BYTE;
						break;
					case 16:
						tex.type = GL_UNSIGNED_SHORT;
					break;
			}
			break;

		case PNG_COLOR_TYPE_PALETTE:
			tex.format = GL_RGB;
			tex.type = GL_UNSIGNED_BYTE;
			break;

		case PNG_COLOR_TYPE_RGB:
			tex.format = GL_RGB;
			switch (bit_depth) {
				case 8:
					tex.type = GL_UNSIGNED_BYTE;
					break;
                case 16:
                	tex.type = GL_UNSIGNED_SHORT;
					break;
			}
			break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			tex.format = GL_RGBA;
			switch (bit_depth) {
				case 8:
					tex.type = GL_UNSIGNED_BYTE;
					break;
				case 16:
					tex.type = GL_UNSIGNED_SHORT;
					break;
			}
			break;
	}

	/* At this point you have read the entire image */

    /* clean up after the read, and free any memory allocated - REQUIRED */
    //png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

    /* close the file */
    fclose(fp);

    /* that's it */
    return (0);
}

void unload_png(char **image_data)
{
	free(*image_data);
}

/////////////////////
//typedef struct {    // Huffman coding tables
//    unsigned char bits[16];
//    unsigned char hval[256];
//    unsigned char size[256];
//    unsigned short code[256];
//} jpeg_huffman_table_t;
//
//struct {            // Jpeg file parameter
//    FILE    *file;  // file
//    int     width;  // width image
//    int     height; // height image
//    unsigned char *data;   // image
//    int     data_precision; // bit per component
//    int     num_components; // number component
//    int     restart_interval;   // restart interval
//    struct {
//        int     id; // identifier
//        int     h;  // horizontal sampling factor
//        int     v;  // vertical sampling factor
//        int     t;  // Quantization table selector
//        int     td; // DC table selector
//        int     ta; // AC table selector
//    } component_info[3];
//    jpeg_huffman_table_t hac[4];    // AC table
//    jpeg_huffman_table_t hdc[4];    // DC table
//    int     qtable[4][64];  // Quantization table
//    struct {
//        int     ss,se;  // progressive jpeg spectral selection
//        int     ah,al;  // progressive jpeg successive approx
//    } scan;
//    int     dc[3];
//    int     curbit;
//    unsigned char   curbyte;
//} jpeg_file_s;
//
//int jpeg_read_byte() {  // read byte
//    fread(&jpeg_file_s.curbyte,1,1,jpeg_file_s.file);
//    jpeg_file_s.curbit = 0;
//    return jpeg_file_s.curbyte;
//}
//
//int jpeg_read_word() {  // read word
//    unsigned short i;
//    fread(&i,2,1,jpeg_file_s.file);
//    i = ((i << 8) & 0xFF00) + ((i >> 8) & 0x00FF);
//    return i;
//}
//
//int jpeg_read_bit() {   // read bit
//    register int i;
//    if(jpeg_file_s.curbit == 0) {
//        jpeg_read_byte();
//        if(jpeg_file_s.curbyte == 0xFF) {
//            while(jpeg_file_s.curbyte == 0xFF) jpeg_read_byte();
//            if(jpeg_file_s.curbyte >= 0xD0 && jpeg_file_s.curbyte <= 0xD7)
//                memset(jpeg_file_s.dc,0,sizeof(int) * 3);
//            if(jpeg_file_s.curbyte == 0) jpeg_file_s.curbyte = 0xFF;
//            else jpeg_read_byte();
//        }
//    }
//    i = (jpeg_file_s.curbyte >> (7 - jpeg_file_s.curbit++)) & 0x01;
//    if(jpeg_file_s.curbit == 8) jpeg_file_s.curbit = 0;
//    return i;
//}
//
//int jpeg_read_bits(int num) {   // read num bit
//    register int i,j;
//    for(i = 0, j = 0; i < num; i++) {
//        j <<= 1;
//        j |= jpeg_read_bit();
//    }
//    return j;
//}
//
//int jpeg_bit2int(int bit,int i) {   // convert bit code to int
//    if((i & (1 << (bit - 1))) > 0) return i;
//    return -(i ^ ((1 << bit) - 1));
//}
//
//int jpeg_huffmancode(jpeg_huffman_table_t *table) { // get Huffman code
//    register int i,size,code;
//    for(size = 1, code = 0, i = 0; size < 17; size++) {
//        code <<= 1;
//        code |= jpeg_read_bit();
//        while(table->size[i] <= size) {
//            if(table->code[i] == code) return table->hval[i];
//            i++;
//        }
//    }
//    return code;
//}
//
//void jpeg_idct(float *data) {   // aa&n algorithm inverse DCT
//    float   t0,t1,t2,t3,t4,t5,t6,t7;
//    float   t10,t11,t12,t13;
//    float   z5,z10,z11,z12,z13;
//    float   *dataptr;
//    int i;
//    dataptr = data;
//    for(i = 0; i < 8; i++) {
//        t0 = dataptr[8 * 0];
//        t1 = dataptr[8 * 2];
//        t2 = dataptr[8 * 4];
//        t3 = dataptr[8 * 6];
//        t10 = t0 + t2;
//        t11 = t0 - t2;
//        t13 = t1 + t3;
//        t12 = - t13 + (t1 - t3) * 1.414213562;
//        t0 = t10 + t13;
//        t3 = t10 - t13;
//        t1 = t11 + t12;
//        t2 = t11 - t12;
//        t4 = dataptr[8 * 1];
//        t5 = dataptr[8 * 3];
//        t6 = dataptr[8 * 5];
//        t7 = dataptr[8 * 7];
//        z13 = t6 + t5;
//        z10 = t6 - t5;
//        z11 = t4 + t7;
//        z12 = t4 - t7;
//        t7 = z11 + z13;
//        t11 = (z11 - z13) * 1.414213562;
//        z5 = (z10 + z12) * 1.847759065;
//        t10 = - z5 + z12 * 1.082392200;
//        t12 = z5 - z10 * 2.613125930;
//        t6 = t12 - t7;
//        t5 = t11 - t6;
//        t4 = t10 + t5;
//        dataptr[8 * 0] = t0 + t7;
//        dataptr[8 * 7] = t0 - t7;
//        dataptr[8 * 1] = t1 + t6;
//        dataptr[8 * 6] = t1 - t6;
//        dataptr[8 * 2] = t2 + t5;
//        dataptr[8 * 5] = t2 - t5;
//        dataptr[8 * 4] = t3 + t4;
//        dataptr[8 * 3] = t3 - t4;
//        dataptr++;
//    }
//    dataptr = data;
//    for(i = 0; i < 8; i++) {
//        t10 = dataptr[0] + dataptr[4];
//        t11 = dataptr[0] - dataptr[4];
//        t13 = dataptr[2] + dataptr[6];
//        t12 = - t13 + (dataptr[2] - dataptr[6]) * 1.414213562;
//        t0 = t10 + t13;
//        t3 = t10 - t13;
//        t1 = t11 + t12;
//        t2 = t11 - t12;
//        z13 = dataptr[5] + dataptr[3];
//        z10 = dataptr[5] - dataptr[3];
//        z11 = dataptr[1] + dataptr[7];
//        z12 = dataptr[1] - dataptr[7];
//        t7 = z11 + z13;
//        t11 = (z11 - z13) * 1.414213562;
//        z5 = (z10 + z12) * 1.847759065;
//        t10 = - z5 + z12 * 1.082392200;
//        t12 = z5 - z10 * 2.613125930;
//        t6 = t12 - t7;
//        t5 = t11 - t6;
//        t4 = t10 + t5;
//        dataptr[0] = t0 + t7;
//        dataptr[7] = t0 - t7;
//        dataptr[1] = t1 + t6;
//        dataptr[6] = t1 - t6;
//        dataptr[2] = t2 + t5;
//        dataptr[5] = t2 - t5;
//        dataptr[4] = t3 + t4;
//        dataptr[3] = t3 - t4;
//        dataptr += 8;
//    }
//}
//
//int jpeg_readmarkers() {    // read jpeg markers
//    int marker,length,i,j,k,l,m;
//    jpeg_huffman_table_t *hptr;
//    for(;;) {
//        marker = jpeg_read_byte();
//        if(marker != 0xFF) return 0;
//        marker = jpeg_read_byte();
//        if(marker != 0xD8) {
//            length = jpeg_read_word();
//            length -= 2;
//            switch(marker) {
//                case 0xC0:  // Baseline
//                    jpeg_file_s.data_precision = jpeg_read_byte();
//                    jpeg_file_s.height = jpeg_read_word();
//                    jpeg_file_s.width = jpeg_read_word();
//                    jpeg_file_s.num_components = jpeg_read_byte();
//                    if(length - 6 != jpeg_file_s.num_components * 3) return 0;
//                    for(i = 0; i < jpeg_file_s.num_components; i++) {
//                        jpeg_file_s.component_info[i].id = jpeg_read_byte();
//                        j = jpeg_read_byte();
//                        jpeg_file_s.component_info[i].h = (j >> 4) & 0x0F;
//                        jpeg_file_s.component_info[i].v = j & 0x0F;
//                        jpeg_file_s.component_info[i].t = jpeg_read_byte();
//                    }
//                    break;
//                case 0xC1:  // Extended sequetial, Huffman
//                case 0xC2:  // Progressive, Huffman
//                case 0xC3:  // Lossless, Huffman
//                case 0xC5:  // Differential sequential, Huffman
//                case 0xC6:  // Differential progressive, Huffman
//                case 0xC7:  // Differential lossless, Huffman
//                case 0xC8:  // Reserved for JPEG extensions
//                case 0xC9:  // Extended sequential, arithmetic
//                case 0xCA:  // Progressive, arithmetic
//                case 0xCB:  // Lossless, arithmetic
//                case 0xCD:  // Differential sequential, arithmetic
//                case 0xCE:  // Differential progressive, arithmetic
//                case 0xCF:  // Differential lossless, arithmetic
//                    return 0;
//                    break;
//                case 0xC4:  // Huffman table
//                    while(length > 0) {
//                        k = jpeg_read_byte();
//                        if(k & 0x10) hptr = &jpeg_file_s.hac[k & 0x0F];
//                        else hptr = &jpeg_file_s.hdc[k & 0x0F];
//                        for(i = 0, j = 0; i < 16; i++) {
//                            hptr->bits[i] = jpeg_read_byte();
//                            j += hptr->bits[i];
//                        }
//                        length -= 17;
//                        for(i = 0; i < j; i++)
//                            hptr->hval[i] = jpeg_read_byte();
//                        length -= j;
//                        for(i = 0, k = 0, l = 0; i < 16; i++) {
//                            for(j = 0; j < hptr->bits[i]; j++, k++) {
//                                hptr->size[k] = i + 1;
//                                hptr->code[k] = l++;
//                            }
//                            l <<= 1;
//                        }
//                    }
//                    break;
//                case 0xDB:  // Quantization table
//                    while(length > 0) {
//                        j = jpeg_read_byte();
//                        k = (j >> 4) & 0x0F;
//                        for(i = 0; i < 64; i++) {
//                            if(k) jpeg_file_s.qtable[j][i] = jpeg_read_word();
//                            else  jpeg_file_s.qtable[j][i] = jpeg_read_byte();
//                        }
//                        length -= 65;
//                        if(k) length -= 64;
//                    }
//                    break;
//                case 0xD9:  // End of image (EOI)
//                    return 0;
//                    break;
//                case 0xDA:  // Start of scan (SOS)
//                    j = jpeg_read_byte();
//                    for(i = 0; i < j; i++) {
//                        k = jpeg_read_byte();
//                        m = jpeg_read_byte();
//                        for(l = 0; l < jpeg_file_s.num_components; l++)
//                            if(jpeg_file_s.component_info[l].id == k) {
//                                jpeg_file_s.component_info[l].td = (m >> 4) & 0x0F;
//                                jpeg_file_s.component_info[l].ta = m & 0x0F;
//                            }
//                    }
//                    jpeg_file_s.scan.ss = jpeg_read_byte();
//                    jpeg_file_s.scan.se = jpeg_read_byte();
//                    k = jpeg_read_byte();
//                    jpeg_file_s.scan.ah = (k >> 4) & 0x0F;
//                    jpeg_file_s.scan.al = k & 0x0F;
//                    return 1;
//                    break;
//                case 0xDD:  // Restart interval
//                    jpeg_file_s.restart_interval = jpeg_read_word();
//                    break;
//                default:
//                    fseek(jpeg_file_s.file,length,SEEK_CUR);
//                    break;
//            }
//        }
//    }
//}
//
//void jpeg_decompress() {    // decompress jpeg file (Baseline algorithm)
//    register int x,y,i,j,k,l,c;
//    int X,Y,H,V,plane,scaleh[3],scalev[3];
//    static float vector[64],dct[64];
//    static const int jpeg_zigzag[64] = {
//         0, 1, 5, 6,14,15,27,28,
//         2, 4, 7,13,16,26,29,42,
//         3, 8,12,17,25,30,41,43,
//         9,11,18,24,31,40,44,53,
//        10,19,23,32,39,45,52,54,
//        20,22,33,38,46,51,55,60,
//        21,34,37,47,50,56,59,61,
//        35,36,48,49,57,58,62,63 };
//    static const float aanscale[8] = {  // 1.0, k = 0; cos(k * PI / 16) * sqrt(2), k = 1...7
//        1.0, 1.387039845, 1.306562965, 1.175875602,
//        1.0, 0.785694958, 0.541196100, 0.275899379 };
//    scaleh[0] = 1;
//    scalev[0] = 1;
//    if(jpeg_file_s.num_components == 3) {
//        scaleh[1] = jpeg_file_s.component_info[0].h / jpeg_file_s.component_info[1].h;
//        scalev[1] = jpeg_file_s.component_info[0].v / jpeg_file_s.component_info[1].v;
//        scaleh[2] = jpeg_file_s.component_info[0].h / jpeg_file_s.component_info[2].h;
//        scalev[2] = jpeg_file_s.component_info[0].v / jpeg_file_s.component_info[2].v;
//    }
//    memset(jpeg_file_s.dc,0,sizeof(int) * 3);
//    for(Y = 0; Y < jpeg_file_s.height; Y += jpeg_file_s.component_info[0].v << 3) {
//        if(jpeg_file_s.restart_interval > 0) jpeg_file_s.curbit = 0;
//        for(X = 0; X < jpeg_file_s.width; X += jpeg_file_s.component_info[0].h << 3) {
//            for(plane = 0; plane < jpeg_file_s.num_components; plane++)
//                for(V = 0; V < jpeg_file_s.component_info[plane].v; V++)
//                    for(H = 0; H < jpeg_file_s.component_info[plane].h; H++) {
//                        i = jpeg_huffmancode(&jpeg_file_s.hdc[jpeg_file_s.component_info[plane].td]);
//                        i &= 0x0F;
//                        vector[0] = jpeg_file_s.dc[plane] + jpeg_bit2int(i,jpeg_read_bits(i));
//                        jpeg_file_s.dc[plane] = vector[0];
//                        i = 1;
//                        while(i < 64) {
//                            j = jpeg_huffmancode(&jpeg_file_s.hac[jpeg_file_s.component_info[plane].ta]);
//                            if(j == 0) while(i < 64) vector[i++] = 0;
//                            else {
//                                k = i + ((j >> 4) & 0x0F);
//                                while(i < k) vector[i++] = 0;
//                                j &= 0x0F;
//                                vector[i++] = jpeg_bit2int(j,jpeg_read_bits(j));
//                            }
//                        }
//                        k = jpeg_file_s.component_info[plane].t;
//                        for(y = 0, i = 0; y < 8; y++)
//                            for(x = 0; x < 8; x++, i++) {
//                                j = jpeg_zigzag[i];
//                                dct[i] = vector[j] * jpeg_file_s.qtable[k][j] *
//                                    aanscale[x] * aanscale[y];
//                            }
//                        jpeg_idct(dct);
//                        for(y = 0; y < 8; y++)
//                            for(x = 0; x < 8; x++) {
//                                c = ((int)dct[(y << 3) + x] >> 3) + 128;
//                                if(c < 0) c = 0;
//                                else if(c > 255) c = 255;
//                                if(scaleh[plane] == 1 && scalev[plane] == 1) {
//                                    i = X + x + (H << 3);
//                                    j = Y + y + (V << 3);
//                                    if(i < jpeg_file_s.width && j < jpeg_file_s.height)
//                                        jpeg_file_s.data[((j * jpeg_file_s.width + i) << 2) + plane] = c;
//                                }
//                                else for(l = 0; l < scalev[plane]; l++)
//                                    for(k = 0; k < scaleh[plane]; k++) {
//                                        i = X + (x + (H << 3)) * scaleh[plane] + k;
//                                        j = Y + (y + (V << 3)) * scalev[plane] + l;
//                                        if(i < jpeg_file_s.width && j < jpeg_file_s.height)
//                                            jpeg_file_s.data[((j * jpeg_file_s.width + i) << 2) + plane] = c;
//                                }
//                            }
//                    }
//        }
//    }
//}
//
//void jpeg_ycbcr2rgb() { // convert YCbCr image to RGB
//    int i,Y,Cb,Cr,R,G,B;
//    for(i = 0; i < jpeg_file_s.width * jpeg_file_s.height << 2; i += 4) {
//        Y = jpeg_file_s.data[i];
//        Cb = jpeg_file_s.data[i + 1] - 128;
//        Cr = jpeg_file_s.data[i + 2] - 128;
//        R = Y                + 1.40200 * Cr;
//        G = Y - 0.34414 * Cb - 0.71414 * Cr;
//        B = Y + 1.77200 * Cb;
//        if(R < 0) R = 0;
//        else if(R > 255) R = 255;
//        if(G < 0) G = 0;
//        else if(G > 255) G = 255;
//        if(B < 0) B = 0;
//        else if(B > 255) B = 255;
//        jpeg_file_s.data[i] = R;
//        jpeg_file_s.data[i + 1] = G;
//        jpeg_file_s.data[i + 2] = B;
//        jpeg_file_s.data[i + 3] = 255;
//    }
//}
//
//void jpeg_gray2rgb() {  // grayscale image to RGB
//    int i,j;
//    for(i = 0; i < jpeg_file_s.width * jpeg_file_s.height << 2; i += 4) {
//        j = jpeg_file_s.data[i];
//        jpeg_file_s.data[i + 1] = j;
//        jpeg_file_s.data[i + 2] = j;
//        jpeg_file_s.data[i + 3] = 255;
//    }
//}
//

//bool load_jpeg(const char *name, ngxTexture &tex) {
//    memset(&jpeg_file_s,0,sizeof(jpeg_file_s));
//    jpeg_file_s.file = fopen(name,"rb");
//    if(!jpeg_file_s.file) return 0;
//    if(!jpeg_readmarkers()) return 0;
//    jpeg_file_s.data = (unsigned char*)malloc(jpeg_file_s.width * jpeg_file_s.height << 2);
//    if(!jpeg_file_s.data) return 0;
//    jpeg_decompress();
//    if(jpeg_file_s.num_components == 1) jpeg_gray2rgb();
//    if(jpeg_file_s.num_components == 3) jpeg_ycbcr2rgb();
//    fclose(jpeg_file_s.file);
//    *width = jpeg_file_s.width;
//    *height = jpeg_file_s.height;
//    return jpeg_file_s.data;
//}


void FlipVertically(ngxTexture &tex) {
	//dont flip zero or 1 height images
	if (tex.height == 0 || tex.height == 1)
		return;

	int rowsToSwap = 0;
	//see how many rows to swap
	if (tex.height % 2 == 1)
		rowsToSwap = (tex.height - 1) / 2;
	else
		rowsToSwap = tex.height / 2;

	//create space for a temporary row
	GLubyte * tempRow = new GLubyte[tex.width * tex.bpp / 8];
	if (!tempRow) {
		Util::log(
				"Unable to flip image, unable to create space for temporary row");
		return;
	}

	//loop through rows to swap
	for (int i = 0; i < rowsToSwap; ++i) {
		//copy row i into temp
		memcpy(tempRow, &tex.data[i * tex.width * tex.bpp / 8], tex.width * tex.bpp / 8);
		//copy row height-i-1 to row i
		memcpy(&tex.data[i * tex.width * tex.bpp / 8],
				&tex.data[(tex.height - i - 1) * tex.width * tex.bpp / 8], tex.width * tex.bpp / 8);
		//copy temp into row height-i-1
		memcpy(&tex.data[(tex.height - i - 1) * tex.width * tex.bpp / 8], tempRow,
				tex.width * tex.bpp / 8);
	}

	//free tempRow
	if (tempRow)
		delete[] tempRow;
	tempRow = NULL;
}

//Load a jpg texture into image
bool load_jpeg(const char * filename, ngxTexture &tex)
{
	Util::log("Loading %s in LoadJPG()", filename);

	//Clear the data in image
	if(tex.data)
		delete [] tex.data;
	tex.data=NULL;
	tex.bpp=0;
	tex.format=0;
	tex.height=0;
	tex.width=0;


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
	tex.bpp=channels*8;
	tex.width=cinfo.image_width;
	tex.height=cinfo.image_height;

	if(tex.bpp==24)
		tex.format=GL_RGB;
	if(tex.bpp==32)
		tex.format=GL_RGBA;

	//Allocate memory for image
	tex.data=new GLubyte[tex.width*tex.height*channels];
	if(!tex.data)
	{
		Util::log("Unable to allocate memory for temporary texture data");
		return false;
	}

	//Create an array of row pointers
	unsigned char ** rowPtr = new unsigned char * [tex.height];
	if(!rowPtr)
	{
		Util::log("Unable to allocate memory for row pointers");
		return false;
	}

	for(unsigned int i=0; i<tex.height; ++i)
		rowPtr[i]=&(tex.data[i*tex.width*channels]);

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
	FlipVertically(tex);

	Util::log("%s loaded successfully", filename);
	return true;
}

bool save_tga(const char *name, ngxTexture &tex) {
    int i,j;
    unsigned char *buffer;
    FILE    *FileTGA = fopen(name,"wb");
    if(!FileTGA) return 0;
    buffer = (unsigned char*)malloc(18 + tex.width * tex.height * 4);
    memset(buffer,0,18);
    buffer[2] = 2;
    buffer[12] = tex.width & 0xFF;
    buffer[13] = tex.width >> 8;
    buffer[14] = tex.height & 0xFF;
    buffer[15] = tex.height >> 8;
    buffer[16] = 32;
    buffer[17] = 0x20;
    memcpy(buffer + 18, tex.data, tex.width * tex.height * 4);
    for(i = 18; i < 18 + tex.width * tex.height * 4; i += 4) {
        j = buffer[i];
        buffer[i] = buffer[i + 2];
        buffer[i + 2] = j;
    }
    fwrite(buffer,1,18 + tex.width * tex.height * 4,FileTGA);
    fclose(FileTGA);
    free(buffer);
    return true;
}

/*
 *
 */
 bool Util::LoadTexture(const char *name, ngxTexture &tex)
{
//    if(strstr(name,".tga")) return load_tga(name, tex);
    if(strstr(name,".png")) return load_png(name, tex);
    if(strstr(name,".jpg")) return load_jpeg(name, tex);
//    if(strstr(name,".dds")) return load_dds(name, tex);

    // throw Exception fprintf(stderr,"Texture::load(): unknown format of \"%s\" file\n",name);
    return NULL;
}
/*
 *
 */
bool Util::SaveTexture(const char *name, ngxTexture &tex)
{
    if(strcmp(name,".tga")) return save_tga(name, tex);
    if(strcmp(name,".jpg")) return save_jpeg(name, tex ,80);

    // throw Exception  fprintf(stderr,"Texture::save(): unknown format of \"%s\" file\n",name);
    return 0;
}
