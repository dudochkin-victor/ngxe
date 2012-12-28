/*
	SOUND.h
*/

#include "resource.h"

// Structure to hold some information about the memory buffer
typedef struct 
{
	int length;
	int pos;
	void *data;
} MEMFILE;

// Loads the song from a resource
unsigned int memopen(char *name)
{
	MEMFILE *memfile;

	//memfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);
	memfile = new MEMFILE;

	// hey look some load from resource code!
	HRSRC		rec;
	HGLOBAL		handle;

	rec = FindResource(NULL, name, "XM");
	handle = LoadResource(NULL, rec);
		
	memfile->data = LockResource(handle);
	memfile->length = SizeofResource(NULL, rec);
	memfile->pos = 0;

	return (unsigned int)memfile;
}

// Free memory
void memclose(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	//free(memfile);
	delete memfile;
}

// Retrieve data from the buffer
int memread(void *buffer, int size, unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (memfile->pos + size >= memfile->length)
		size = memfile->length - memfile->pos;

	memcpy(buffer, (char *)memfile->data+memfile->pos, size);
	memfile->pos += size;
	
	return size;
}

// Seek to another position in the buffer
void memseek(unsigned int handle, int pos, signed char mode)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (mode == SEEK_SET) 
		memfile->pos = pos;
	else if (mode == SEEK_CUR) 
		memfile->pos += pos;
	else if (mode == SEEK_END)
		memfile->pos = memfile->length + pos;

	if (memfile->pos > memfile->length)
		memfile->pos = memfile->length;
}

// Get the current position in the buffer
int memtell(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	return memfile->pos;
}

// Module pointer
#ifndef MUTE
FMUSIC_MODULE *mod;
#endif

// Init & Stop
void startsound()
{
#ifndef MUTE
	// Set the callback functions to use
	FSOUND_File_SetCallbacks(memopen, memclose, memread, memseek, memtell);

	// Initialize the sound system
	if(!FSOUND_Init(44100, 0))
		throw("Failed to init sound system");

	// Load the song
	mod = FMUSIC_LoadSong(MAKEINTRESOURCE(IDR_MUSIC), NULL);

	// Check for errors
	if(!mod)
		throw("Failed to load song");

	// Start playback
	FMUSIC_PlaySong(mod);
#endif
}

void stopsound()
{
#ifndef MUTE
	// Free memory
	FMUSIC_FreeSong(mod);
	FSOUND_Close();
#endif
}
