//////////////////////////////////////////////////////////////////////////////////////////
//	Main.cpp
//	Bump Mapping
//	Downloaded from: www.paulsprojects.net
//	Created:	26th August 2002
//	Modified:	16th November 2002	-	Instead of passing tangents as texture coords 1 and 2
//										use aliased attributes 9 and 10. This way, these texture
//										coordinates will reach the vertex program on a geforce 2
//										which only has 2 texture units.
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "extensions/ARB_multitexture_extension.h"
#include "extensions/ARB_texture_cube_map_extension.h"
#include "extensions/EXT_compiled_vertex_array_extension.h"
#include "extensions/EXT_texture_edge_clamp_extension.h"
#include "extensions/NV_register_combiners_extension.h"
#include "extensions/NV_texture_shader_extension.h"
#include "extensions/NV_vertex_program_extension.h"
#include "FPS_COUNTER.h"
#include "TIMER.h"
#include "Maths/Maths.h"
#include "IMAGE.h"
#include "main.h"
#include "TORUS.h"
#include "Normalisation_CubeMap.h"
#include "NV_vertex_program.h"
#include "Util.h"
#include "View.h"

//errorLog MUST be kept - it is used by other files
FPS_COUNTER fpsCounter;
TIMER timer;

COLOR backgroundColor(0.4f, 0.0f, 0.0f, 1.0f);

GLuint singlePassVertexProgram;
GLuint diffuseVertexProgram, decalVertexProgram, simpleSpecularVertexProgram;
GLuint diffuseDecalVertexProgram, lookUpSpecularVertexProgram;

VECTOR3D worldLightDirection(1.0f, 1.0f, 1.0f);
VECTOR3D worldViewDirection(0.0f, 1.0f, 1.0f);
VECTOR3D objectLightDirection(1.0f, 1.0f, 1.0f);
VECTOR3D objectViewDirection(0.0f, 1.0f, 1.0f);
float angle=0.0f;

IMAGE normalMapImage;
GLuint normalMapTexture;
GLuint signedNormalMap;

GLuint flatNormalMap;
GLuint signedFlatNormalMap;

GLuint normalisationCubeMap;

IMAGE decalImage;
GLuint decalTexture;

GLuint specularRamp;

TORUS torus;

bool showBumps=true;

bool paths1And2Supported=false;

//rendering paths
//single pass or using texture shader or 3 pass GF2 fallback
enum TECHNIQUE{SINGLE_PASS, TEXTURE_LOOKUP, FALLBACK};
TECHNIQUE currentTechnique=FALLBACK;

//Set up variables
bool DemoInit()
{
	SetUpARB_multitexture();
	SetUpARB_texture_cube_map();
	SetUpEXT_compiled_vertex_array();
	SetUpEXT_texture_edge_clamp();
	SetUpNV_register_combiners();
	SetUpNV_vertex_program();
	SetUpNV_texture_shader();

	if(	!GL_ARB_texture_cube_map || !GL_EXT_compiled_vertex_array ||
		!GL_ARB_multitexture || !GL_NV_register_combiners ||
		!GL_NV_vertex_program || !GL_EXT_texture_edge_clamp)
		return false;

	//Get some useful info
	int maxTextureUnitsARB;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTextureUnitsARB);
	int maxGeneralCombinersNV;
	glGetIntegerv(GL_MAX_GENERAL_COMBINERS_NV, &maxGeneralCombinersNV);

	if(	GL_NV_texture_shader &&
		maxTextureUnitsARB>=4 &&
		maxGeneralCombinersNV>=4)
	{
		Util::log("Higher Quality bump mapping supported");
		paths1And2Supported=true;
		currentTechnique=TEXTURE_LOOKUP;
	}
	else
		Util::log("Higher Quality bump mapping unsupported");

	
	//Load Textures
	//normal map - put gloss map in alpha
	normalMapImage.Load("Normal Map.bmp");
	normalMapImage.LoadAlphaTGA("gloss.tga");
	glGenTextures(1, &normalMapTexture);
	glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA8, normalMapImage.width, normalMapImage.height,
					0, normalMapImage.format, GL_UNSIGNED_BYTE, normalMapImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//decal
	decalImage.Load("Decal.bmp");
	glGenTextures(1, &decalTexture);
	glBindTexture(GL_TEXTURE_2D, decalTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA8, decalImage.width, decalImage.height,
					0, decalImage.format, GL_UNSIGNED_BYTE, decalImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Create normalisation cube map
	glGenTextures(1, &normalisationCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	GenerateNormalisationCubeMap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if(paths1And2Supported)
	{
		//Create signed normap map
		//Create space for signed data
		GLbyte * signedData=new GLbyte[normalMapImage.width*normalMapImage.height*3];
		if(!signedData)
		{
			Util::log("Unable to allocate memory for signed normal map data");
			return false;
		}

		//Convert unsigned to signed RGB data, ignoring alpha
		for(unsigned int i=0; i<normalMapImage.width*normalMapImage.height; i++)
		{
			for(unsigned int j=0; j<3; j++)
			{
				signedData[i*3+j]=normalMapImage.data[i*4+j]-128;
			}
		}
	
		glGenTextures(1, &signedNormalMap);
		glBindTexture(GL_TEXTURE_2D, signedNormalMap);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_SIGNED_RGBA8_NV, normalMapImage.width, normalMapImage.height,
						0, GL_RGB, GL_BYTE, signedData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if(signedData)
			delete [] signedData;
		signedData=NULL;
	}

	//make sure texImage2D is finished with normalMapImage before we change the image
	glFinish();

	//Flatten the bumps in the normal map
	for(unsigned int i=0; i<normalMapImage.width*normalMapImage.height; ++i)
	{
		normalMapImage.data[i*4]=128;
		normalMapImage.data[i*4+1]=128;
		normalMapImage.data[i*4+2]=255;
	}

	//create flat normal map with gloss map in alpha
	glGenTextures(1, &flatNormalMap);
	glBindTexture(GL_TEXTURE_2D, flatNormalMap);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA8, normalMapImage.width, normalMapImage.height,
					0, normalMapImage.format, GL_UNSIGNED_BYTE, normalMapImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if(paths1And2Supported)
	{
		//Create signed flat normap map
		//Create space for signed data
		GLbyte * signedData=new GLbyte[normalMapImage.width*normalMapImage.height*3];
		if(!signedData)
		{
			Util::log("Unable to allocate memory for signed normal map data");
			return false;
		}

		//Convert unsigned to signed RGB data, ignoring alpha
		for(unsigned int i=0; i<normalMapImage.width*normalMapImage.height; i++)
		{
			for(unsigned int j=0; j<3; j++)
			{
				signedData[i*3+j]=normalMapImage.data[i*4+j]-128;
			}
		}
	
		glGenTextures(1, &signedFlatNormalMap);
		glBindTexture(GL_TEXTURE_2D, signedFlatNormalMap);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_SIGNED_RGBA8_NV, normalMapImage.width, normalMapImage.height,
						0, GL_RGB, GL_BYTE, signedData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if(signedData)
			delete [] signedData;
		signedData=NULL;
		
		//Create specular ramp texture
		unsigned char specularRampValues[256];
		for(unsigned int i=0; i<256; i++)
		{
			double poweredValue=(double)i/255;
			//raise to 16th power
			poweredValue=	poweredValue*poweredValue*poweredValue*poweredValue*
							poweredValue*poweredValue*poweredValue*poweredValue*
							poweredValue*poweredValue*poweredValue*poweredValue*
							poweredValue*poweredValue*poweredValue*poweredValue;
			specularRampValues[i] = char(poweredValue*255);
		}

		glGenTextures(1, &specularRamp);
		glBindTexture(GL_TEXTURE_2D, specularRamp);
		glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA8, 256, 1,
						0, GL_LUMINANCE, GL_UNSIGNED_BYTE, specularRampValues);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	//reset timer for start
	timer.Reset();
	
	return true;
}

//Set up openGL
bool GLInit()
{
	//set viewport
	int height = 480;
	int width = 640;

	
	glViewport(0, 0, width, height);					//reset viewport

	//set up projection matrix
	glMatrixMode(GL_PROJECTION);							//select projection matrix
	glLoadIdentity();										//reset
	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f);
	
	//load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//other states
	//shading
	glShadeModel(GL_SMOOTH);
	glClearColor(	backgroundColor.r,
					backgroundColor.g,
					backgroundColor.b,
					backgroundColor.a);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Set up vertex arrays for torus
	//Fixed 17th November 2002
	//Instead of passing tangents as texture coords 1 and 2, use aliased attributes 9 and 10.
	//This way, these texture coordinates will reach the vertex program on a geforce 2
	//which only has 2 texture units.
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].position);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].normal);

	//Pass texture coords to unit 0
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].s);

	//Pass tangent,binormal to attributes 9, 10
	glEnableClientState(GL_VERTEX_ATTRIB_ARRAY9_NV);
	glVertexAttribPointerNV(9, 3, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].sTangent);
	
	glEnableClientState(GL_VERTEX_ATTRIB_ARRAY10_NV);
	glVertexAttribPointerNV(10, 3, GL_FLOAT, sizeof(TORUS_VERTEX), &torus.vertices[0].tTangent);
	
	//Use compiled vertex arrays
	glLockArraysEXT(0, torus.numVertices);


	//Load vertex programs
	glGenProgramsNV(1, &singlePassVertexProgram);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, singlePassVertexProgram);
	LoadNV_vertex_program("single pass vertex program.txt", singlePassVertexProgram);

	glGenProgramsNV(1, &diffuseDecalVertexProgram);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, diffuseDecalVertexProgram);
	LoadNV_vertex_program("diffuse decal vertex program.txt", diffuseDecalVertexProgram);

	glGenProgramsNV(1, &lookUpSpecularVertexProgram);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, lookUpSpecularVertexProgram);
	LoadNV_vertex_program("look up specular vertex program.txt", lookUpSpecularVertexProgram);

	glGenProgramsNV(1, &diffuseVertexProgram);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, diffuseVertexProgram);
	LoadNV_vertex_program("diffuse vertex program.txt", diffuseVertexProgram);
	
	glGenProgramsNV(1, &decalVertexProgram);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, decalVertexProgram);
	LoadNV_vertex_program("decal vertex program.txt", decalVertexProgram);

	glGenProgramsNV(1, &simpleSpecularVertexProgram);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, simpleSpecularVertexProgram);
	LoadNV_vertex_program("simple specular vertex program.txt", simpleSpecularVertexProgram);

	//Set Tracking Matrix
	//Modelview Projection in registers c[0]-c[3]
	glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);

	return true;
}

//Perform per frame updates
void UpdateFrame(unsigned char key, int x, int y)
{
	angle=(float)timer.GetTime()/25;

	objectLightDirection=worldLightDirection.GetRotatedY(-angle);
	objectViewDirection=worldViewDirection.GetRotatedY(-angle);

	//update vertex program registers
	//Light direction in c[4]
	glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 4, VECTOR4D(objectLightDirection.GetNormalized()));
	//view direction in c[5]
	glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 5, VECTOR4D(objectViewDirection.GetNormalized()));

//	//pause & unpause
//	if(window.isKeyPressed('P'))
//		timer.Pause();
//
//	if(window.isKeyPressed('U'))
//		timer.Unpause();
//
//	//change technique
//	if(window.isKeyPressed('1') && paths1And2Supported)
//		currentTechnique=SINGLE_PASS;
//
//	if(window.isKeyPressed('2') && paths1And2Supported)
//		currentTechnique=TEXTURE_LOOKUP;
//
//	if(window.isKeyPressed('3'))
//		currentTechnique=FALLBACK;
//
//	//toggle bumpy
//	if(window.isKeyPressed('B'))
//		showBumps=true;
//
//	if(window.isKeyPressed('F'))
//		showBumps=false;
}

//draw a frame
void RenderFrame()
{
	//For some reason, vertex attribute arrays only seem to work if we sandwich the Clear
	//With Enable/Disable vertex program (at least for the first frame)
	glEnable(GL_VERTEX_PROGRAM_NV);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_VERTEX_PROGRAM_NV);

	glLoadIdentity();										//reset modelview matrix

	gluLookAt(	10*worldViewDirection.x, 10*worldViewDirection.y, 10*worldViewDirection.z,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	if(currentTechnique==SINGLE_PASS)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		SetSinglePassStates();
	
		//draw torus
		glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);

		glPopAttrib();
	}

	if(currentTechnique==TEXTURE_LOOKUP)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		SetDiffuseDecalStates();
	
		//draw torus
		glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);

		glPopAttrib();

		//add specular
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);

		SetLookUpSpecularStates();
	
		//draw torus
		glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);

		glPopAttrib();
	}

	if(currentTechnique==FALLBACK)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		SetDiffuseStates();
	
		//draw torus
		glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);

		glPopAttrib();


		//modulate by decal
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);

		SetDecalStates();

		glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);

		glPopAttrib();

		//add specular
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		SetSimpleSpecularStates();

		glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, torus.indices);

		glPopAttrib();
	}



	fpsCounter.Update();											//update frames per second counter
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
//	window.StartTextMode();
//	window.Print(0, 28, "FPS: %.2f", fpsCounter.GetFps());			//print the fps
//	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
//	if(currentTechnique==SINGLE_PASS)
//		window.Print(0, 48, "Single Pass Method");
//	if(currentTechnique==TEXTURE_LOOKUP)
//		window.Print(0, 48, "NV_texture_shader Method");
//	if(currentTechnique==FALLBACK)
//	window.Print(0, 48, "Fallback 3-pass Method");
//	window.EndTextMode();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

//	if(window.isKeyPressed(VK_F1))
//	{
//		window.SaveScreenshot();
//		window.SetKeyReleased(VK_F1);
//	}

	// All done drawing.  Let's show it.
	glutSwapBuffers();

	//check for any opengl errors
//	window.CheckGLError();
}

void SetSinglePassStates(void)
{
	//Bind textures
	//Unit 0 - normal map
	glEnable(GL_TEXTURE_2D);
	if(showBumps)
		glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	else
		glBindTexture(GL_TEXTURE_2D, flatNormalMap);

	//Unit 1 - normalisation cube map
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	
	//Unit 2 - normalisation cube map
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	
	//Unit 3 - decal texture
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, decalTexture);

	glActiveTextureARB(GL_TEXTURE0_ARB);

	

	//Set up Register combiners
	//3 general combiners
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 4);

	//combiner 0 does	tex0.rgb dot tex1.rgb -> spare0.rgb,
	//					tex0.rgb dot tex2.rgb -> spare1.rgb
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE0_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_TEXTURE1_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_C_NV, GL_TEXTURE0_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_D_NV, GL_TEXTURE2_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_SPARE0_NV, GL_SPARE1_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_TRUE, GL_TRUE, GL_FALSE);


	//combiner 1 does	spare1.rgb * spare1.rgb -> spare1.rgb
	//					spare0.rgb * tex3.rgb   -> spare0.rgb
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_A_NV, GL_SPARE1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_B_NV, GL_SPARE1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_C_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_D_NV, GL_TEXTURE3_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	glCombinerOutputNV(	GL_COMBINER1_NV, GL_RGB, GL_SPARE1_NV, GL_SPARE0_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);


	//combiner 2 does	spare1.rgb * spare1.rgb -> spare1.rgb
	glCombinerInputNV(	GL_COMBINER2_NV, GL_RGB, GL_VARIABLE_A_NV, GL_SPARE1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER2_NV, GL_RGB, GL_VARIABLE_B_NV, GL_SPARE1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	glCombinerOutputNV(	GL_COMBINER2_NV, GL_RGB, GL_SPARE1_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//combiner 3 does	spare1.rgb * spare1.rgb -> spare1.rgb
	glCombinerInputNV(	GL_COMBINER3_NV, GL_RGB, GL_VARIABLE_A_NV, GL_SPARE1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER3_NV, GL_RGB, GL_VARIABLE_B_NV, GL_SPARE1_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	glCombinerOutputNV(	GL_COMBINER3_NV, GL_RGB, GL_SPARE1_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);

	//final combiner outputs spare1.rgb*spare1.rgb*tex0.a+spare0.rgb
	//First do spare1.rgb*spare1.rgb in EF multiplier
	glFinalCombinerInputNV(GL_VARIABLE_E_NV, GL_SPARE1_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_F_NV, GL_SPARE1_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	//Now do rest
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_E_TIMES_F_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_TEXTURE0_ARB, GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	glEnable(GL_REGISTER_COMBINERS_NV);
	
	
	//Set up vertex program
	glEnable(GL_VERTEX_PROGRAM_NV);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, singlePassVertexProgram);
}


void SetDiffuseDecalStates(void)
{
	//Bind textures
	//Unit 0 - normal map
	glEnable(GL_TEXTURE_2D);
	if(showBumps)
		glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	else
		glBindTexture(GL_TEXTURE_2D, flatNormalMap);

	//Unit 1 - normalisation cube map
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);

	//Unit 2 - decal map
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, decalTexture);
	glActiveTextureARB(GL_TEXTURE0_ARB);


	//Set up Register combiners
	//1 general combiner
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	//combiner 0 does	tex0.rgb dot tex1.rgb -> spare0.rgb,
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE0_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_TEXTURE1_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_TRUE, GL_FALSE, GL_FALSE);

	//final combiner outputs spare0.rgb*texture2.rgb
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_TEXTURE2_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	glEnable(GL_REGISTER_COMBINERS_NV);

	//Set up vertex program
	glEnable(GL_VERTEX_PROGRAM_NV);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, diffuseDecalVertexProgram);
}


void SetLookUpSpecularStates(void)
{
	//Set Up Texture Shaders
	
	//Unit 0 - texture 2d with signed normal map
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_TEXTURE_2D);
	if(showBumps)
		glBindTexture(GL_TEXTURE_2D, signedNormalMap);
	else
		glBindTexture(GL_TEXTURE_2D, signedFlatNormalMap);

	//Unit 1 - dot product
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_DOT_PRODUCT_NV);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_PREVIOUS_TEXTURE_INPUT_NV, GL_TEXTURE0_ARB);

	//Unit 2 - dot product texture 2d into specular ramp
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, specularRamp);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_DOT_PRODUCT_TEXTURE_2D_NV);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_PREVIOUS_TEXTURE_INPUT_NV, GL_TEXTURE0_ARB);

	//Unit 3 - texture 2d with normal map (for gloss)
	glActiveTextureARB(GL_TEXTURE3_ARB);
	glTexEnvi(GL_TEXTURE_SHADER_NV, GL_SHADER_OPERATION_NV, GL_TEXTURE_2D);
	if(showBumps)
		glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	else
		glBindTexture(GL_TEXTURE_2D, flatNormalMap);

	glActiveTextureARB(GL_TEXTURE0_ARB);

	glEnable(GL_TEXTURE_SHADER_NV);

	

	//Set up Register combiners
	//1 general combiner
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	//final combiner outputs texture2*tex3.a
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_TEXTURE2_ARB, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_TEXTURE3_ARB, GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	glEnable(GL_REGISTER_COMBINERS_NV);
	
	
	//Set up vertex program
	glEnable(GL_VERTEX_PROGRAM_NV);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, lookUpSpecularVertexProgram);
}




void SetDiffuseStates(void)
{
	//Bind textures
	//Unit 0 - normal map
	glEnable(GL_TEXTURE_2D);
	if(showBumps)
		glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	else
		glBindTexture(GL_TEXTURE_2D, flatNormalMap);

	//Unit 1 - normalisation cube map
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	glActiveTextureARB(GL_TEXTURE0_ARB);


	//Set up Register combiners
	//1 general combiner
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 1);

	//combiner 0 does	tex0.rgb dot tex1.rgb -> spare0.rgb,
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE0_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_TEXTURE1_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_TRUE, GL_FALSE, GL_FALSE);

	//final combiner outputs spare0.rgb
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	glEnable(GL_REGISTER_COMBINERS_NV);

	//Set up vertex program
	glEnable(GL_VERTEX_PROGRAM_NV);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, diffuseVertexProgram);
}


void SetDecalStates(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, decalTexture);

	//Set up vertex program
	glEnable(GL_VERTEX_PROGRAM_NV);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, decalVertexProgram);
}


void SetSimpleSpecularStates(void)
{
	//Bind textures
	//Unit 0 - normal map
	glEnable(GL_TEXTURE_2D);
	if(showBumps)
		glBindTexture(GL_TEXTURE_2D, normalMapTexture);
	else
		glBindTexture(GL_TEXTURE_2D, flatNormalMap);

	//Unit 1 - normalisation cube map
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normalisationCubeMap);
	
	glActiveTextureARB(GL_TEXTURE0_ARB);

	

	//Set up Register combiners
	//2 general combiners
	glCombinerParameteriNV(GL_NUM_GENERAL_COMBINERS_NV, 2);

	//combiner 0 does	tex0.rgb dot tex1.rgb -> spare0.rgb,
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_A_NV, GL_TEXTURE0_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER0_NV, GL_RGB, GL_VARIABLE_B_NV, GL_TEXTURE1_ARB,
						GL_EXPAND_NORMAL_NV, GL_RGB);
	
	glCombinerOutputNV(	GL_COMBINER0_NV, GL_RGB, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_TRUE, GL_FALSE, GL_FALSE);


	//combiner 1 does	2*((spare0.rgb * spare0.rgb)-0.5) -> spare0.rgb
	//					spare0.b   * tex0.a		-> spare0.a
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_A_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_RGB, GL_VARIABLE_B_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	glCombinerOutputNV(	GL_COMBINER1_NV, GL_RGB, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_SCALE_BY_TWO_NV, GL_BIAS_BY_NEGATIVE_ONE_HALF_NV,
						GL_FALSE, GL_FALSE, GL_FALSE);


	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_A_NV, GL_SPARE0_NV,
						GL_UNSIGNED_IDENTITY_NV, GL_BLUE);
	glCombinerInputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_VARIABLE_B_NV, GL_TEXTURE0_ARB,
						GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	
	glCombinerOutputNV(	GL_COMBINER1_NV, GL_ALPHA, GL_SPARE0_NV, GL_DISCARD_NV, GL_DISCARD_NV,
						GL_NONE, GL_NONE, GL_FALSE, GL_FALSE, GL_FALSE);



	//final combiner outputs spare0.rgb*spare0.rgb*spare0.a
	//first do spare0.rgb * spare0.rgb in EF multiplier
	glFinalCombinerInputNV(GL_VARIABLE_E_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_F_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	
	glFinalCombinerInputNV(GL_VARIABLE_A_NV, GL_E_TIMES_F_NV, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_B_NV, GL_SPARE0_NV, GL_UNSIGNED_IDENTITY_NV, GL_ALPHA);
	glFinalCombinerInputNV(GL_VARIABLE_C_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);
	glFinalCombinerInputNV(GL_VARIABLE_D_NV, GL_ZERO, GL_UNSIGNED_IDENTITY_NV, GL_RGB);

	glEnable(GL_REGISTER_COMBINERS_NV);
	
	
	//Set up vertex program
	glEnable(GL_VERTEX_PROGRAM_NV);
	glBindProgramNV(GL_VERTEX_PROGRAM_NV, simpleSpecularVertexProgram);
}



//ENTRY POINT FOR APPLICATION
//CALL WINDOW CREATION ROUTINE, DEAL WITH MESSAGES, WATCH FOR INTERACTION
int main(int argc, char **argv) {
	Util::log("%s" , "TEST");
	View * v = new View(&argc, argv);
	// Open a window
	v->createWindow("bumpmapping", 640, 480);

	//init variables etc, then GL
	if(!DemoInit())
	{
		Util::log("Demo Initiation failed");
		return 0;
	}
	else
		Util::log("Demo Initiation Successful");

	if(!GLInit())
	{
		Util::log("OpenGL Initiation failed");
		return 0;
	}
	else
		Util::log("OpenGL Initiation Successful");

	v->onKeyboard(&UpdateFrame);
	v->onDraw(&RenderFrame);
	v->start();
	DemoShutdown();
	
	Util::log("Exiting...");
	return 0;								//Exit The Program
}
