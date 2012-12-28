/*
	DEMOText.h
*/

struct text
{
	float start;
	float duration;
	float x, y;
	float xspd, yspd;
	float xscl, yscl;
	const char *msg;
};

text demotext[] =
{
	/*
	 BEG  DUR  X    Y   XSP  YSP XSX YSX     TXT 
	*/	
	{  2,  6,  20,  80,  50,   0,  0,  0, "smooth"},
	{  3,  6, 300,  20, -50,   0,  0,  0, "transitions"},
	{  9,  6, 400, 300, -50,   0,  0,  0, "beautiful"},
	{ 10,  6,  60, 200,  50,   0,  0,  0, "flowers"},
	{ 16,  6,  20, 300,  50,   0,  0,  0, "shiny"},
	{ 17,  6, 400, 100, -50,   0,  0,  0, "objects"},
	{ 23,  6, 400, 180, -50,   0,  0,  0, "unique"},
	{ 23,  6,  40,  80,  50,   0,  0,  0, "thoughts"},
	{ 30,  9, 220, 200,   0,   0,  0,  0, "the thing 2003"},

	{ 49,  6,  60, 100,  50,   0,  0,  0, "bright"},
	{ 50,  6, 460, 300, -50,   0,  0,  0, "emotions"},
	{ 56,  6,  60, 350,  50,   0,  0,  0, "soft"},
	{ 57,  6, 360, 250, -50,   0,  0,  0, "music"},
	{ 63,  6, 360, 250, -50,   0,  0,  0, "peaceful"},
	{ 64,  6,  60, 150,  50,   0,  0,  0, "colors"},
	{ 69,  6,  60, 150,  50,   0,  0,  0, "living"},
	{ 70,  6, 460,  50, -50,   0,  0,  0, "creatures"},

	{ 78,  6,  60, 100,  50,   0,  0,  0, "electric"},
	{ 79,  6, 360,  50, -50,   0,  0,  0, "connections"},
	{ 85,  6, 320, 350, -50,   0,  0,  0, "magical"},
	{ 86,  6,  20, 300,  50,   0,  0,  0, "words"},
	{ 92,  6,  60, 150,  50,   0,  0,  0, "open"},
	{ 93,  6, 360, 100, -50,   0,  0,  0, "doors"},

	// End scroller
	{105, 20,  20,   0,   0,  42,  0,  0, "the thing 2003"},
	
	{107, 20,  20,   0,   0,  42,  0,  0, "code and graphics:"},
	{108, 20,  20,   0,   0,  42,  0,  0, "Andreas Hammar (neoztar)"},

	{110, 20,  20,   0,   0,  42,  0,  0, "You have seen:"},
	{111, 20,  20,   0,   0,  42,  0,  0, "- runtime generated textures, models and fonts"},
	{112, 20,  20,   0,   0,  42,  0,  0, "- blending effects"},
	{113, 20,  20,   0,   0,  42,  0,  0, "- metaballs"},
	{114, 20,  20,   0,   0,  42,  0,  0, "- fonts"},
	{115, 20,  20,   0,   0,  42,  0,  0, "- billboarded lines"},
	{116, 20,  20,   0,   0,  42,  0,  0, "- environment mapping"},
	{117, 20,  20,   0,   0,  42,  0,  0, "- motion/radial blur"},

	{119, 20,  20,   0,   0,  42,  0,  0, "Thanks to:"},
	{120, 20,  20,   0,   0,  42,  0,  0, "- NeHe for creating a great contest"},
	{121, 20,  20,   0,   0,  42,  0,  0, "- Xerxes for the cool soundtrack"},
	{122, 20,  20,   0,   0,  42,  0,  0, "- H@nZ for making school a lot funnier"},
	{123, 20,  20,   0,   0,  42,  0,  0, "- Bazzzoca for his theories on women and sportswear"},
	{124, 20,  20,   0,   0,  42,  0,  0, "+ Everyone in my class at GVS!!!"},

	{126, 20,  20,   0,   0,  42,  0,  0, "No animals were harmed during the"},
	{127, 20,  20,   0,   0,  42,  0,  0, "production of this demo"},

	{130, 20,  20,   0,   0,  42,  0,  0, "-- www.neoztar.tk --"}
};

const int numtext = sizeof(demotext)/sizeof(text);