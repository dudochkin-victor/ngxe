//
// Class to encapsulate GLSL program and shader objects and working with them
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include    "GlslProgram.h"
#include    "libTexture.h"
#include    "Vector3D.h"
#include    "Vector2D.h"
#include    "Vector4D.h"
#include    "Matrix3D.h"
#include    "Matrix4x4.h"
#include    "Data.h"
#include    "malloc.h"

GlslProgram :: GlslProgram ()
{
    program        = 0;
    vertexShader   = 0;
    fragmentShader = 0;
    ok             = false;
}

GlslProgram :: ~GlslProgram ()
{
    clear ();
}

bool    GlslProgram :: loadShaders ( const char * vertexFileName, const char * fragmentFileName )
{
    ok = false;

    Data * vertexData = getFile ( vertexFileName );

	if ( vertexData == NULL )
	{
		log += "Cannot open \"";
		log += vertexFileName;
		log += "\"";
	}
	else
    if ( !vertexData -> isOk () || vertexData -> isEmpty () )
    {
    	log += "Error loading vertex shader";

    	delete vertexData;

		vertexData = NULL;
    }

    Data * fragmentData = getFile ( fragmentFileName );

	if ( fragmentData == NULL )
	{
		log += "Cannot open \"";
		log += fragmentFileName;
		log += "\"";
	}
	else
    if ( !fragmentData -> isOk () || fragmentData -> isEmpty () )
    {
    	log += "Error loading fragment shader";

    	delete fragmentData;

    	fragmentData = NULL;
    }

    bool	result = loadShaders ( vertexData, fragmentData );

    delete vertexData;
    delete fragmentData;

    return result;
}

void GlslProgram::test(){
	GLhandleARB program1        = 0;         // program handles
	program1 = glCreateProgramObjectARB();
	checkGlError();
}

bool    GlslProgram :: loadShaders ( Data * vertexShaderData, Data * fragmentShaderData )
{
    ok = false;
                                // check whether we should create program object
    if ( program == 0 )
        program = glCreateProgramObjectARB();

                                // check for errors
    if ( !checkGlError () )
        return false;
                                // create a vertex shader object and a fragment shader object
    if ( vertexShaderData != NULL )
    {
	    vertexShader = glCreateShaderObjectARB ( GL_VERTEX_SHADER_ARB   );

	    log += "Loading vertex shader\n";

    	if ( !loadShader ( vertexShader, vertexShaderData ) )
        	return false;

                                // attach shaders to program object
	    glAttachObjectARB ( program, vertexShader );
    }

    if ( fragmentShaderData != NULL )
    {
	    fragmentShader = glCreateShaderObjectARB ( GL_FRAGMENT_SHADER_ARB );

	    log += "Loading fragment shader\n";

    	if ( !loadShader ( fragmentShader, fragmentShaderData ) )
        	return false;
                                // attach shaders to program object
	    glAttachObjectARB ( program, fragmentShader );
    }

    GLint   linked;

    log += "Linking programs\n";

                                // link the program object and print out the info log
    glLinkProgramARB ( program );

/**/  if ( !checkGlError () )     // check for errors
/**/      return false;

    glGetObjectParameterivARB ( program, GL_OBJECT_LINK_STATUS_ARB, &linked );

    loadLog ( program );

    if ( !linked )
        return false;

    return ok = true;
}

void    GlslProgram :: bind ()
{
    glUseProgramObjectARB ( program );
}

void    GlslProgram :: unbind ()
{
    glUseProgramObjectARB ( 0 );
}

bool    GlslProgram :: loadShader ( GLhandleARB shader, Data * data )
{
    const char * body = (const char *) data -> getPtr ( 0 );
    GLint		 len  = data -> getLength ();
    GLint        compileStatus;

    glShaderSourceARB ( shader, 1, &body,  &len );

                                        // compile the particle vertex shader, and print out
    glCompileShaderARB ( shader );

    if ( !checkGlError() )              // check for OpenGL errors
        return false;

    glGetObjectParameterivARB ( shader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus );

    loadLog ( shader );

    return compileStatus != 0;

}

bool    GlslProgram :: checkGlError ()
{
    bool    retCode = true;
 	GLenum  glErr   = glGetError();

// 	printf("%d\n", glErr);
    if ( glErr == GL_NO_ERROR )
    	return retCode;

    glError = (const char *) gluErrorString ( glErr );		// XXX: what about gluErrorString errors ?
    log += glError;
    return false;
}

void    GlslProgram :: clear ()
{
    glDeleteObjectARB ( program        );                   // it will also detach shaders
    glDeleteObjectARB ( vertexShader   );
    glDeleteObjectARB ( fragmentShader );

    program        = 0;
    vertexShader   = 0;
    fragmentShader = 0;
    ok             = false;
}

void    GlslProgram :: loadLog ( GLhandleARB object )
{
    GLint       logLength     = 0;
    GLsizei     charsWritten  = 0;
    GLcharARB   buffer [2048];
    GLcharARB * infoLog;

    glGetObjectParameterivARB ( object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength );

    if ( !checkGlError() )          // check for OpenGL errors
        return;

    if ( logLength < 1 )
        return;
                                    // try to avoid allocating buffer
    if ( logLength > (int) sizeof ( buffer ) )
    {
        infoLog = (GLcharARB*) malloc ( logLength );

        if ( infoLog == NULL )
        {
            log = "ERROR: Could not allocate log buffer";

            return;
        }
    }
    else
        infoLog = buffer;

    glGetInfoLogARB ( object, logLength, &charsWritten, infoLog );

    log += infoLog;

    if ( infoLog != buffer )
        free ( infoLog );
}

bool    GlslProgram :: setUniformVector ( const char * name, const Vector4D& value )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return false;

    glUniform4fvARB ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector  ( int loc, const Vector4D& value )
{
    glUniform4fvARB ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector ( const char * name, const Vector3D& value )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return false;

    glUniform3fvARB ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector  ( int loc, const Vector3D& value )
{
    glUniform3fvARB ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector ( const char * name, const Vector2D& value )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return false;

    glUniform2fvARB ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformVector  ( int loc, const Vector2D& value )
{
    glUniform2fvARB ( loc, 1, value );

    return true;
}

bool    GlslProgram :: setUniformFloat ( const char * name, float value )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return false;

    glUniform1fARB ( loc, value );

    return true;
}

bool    GlslProgram :: setUniformFloat ( int loc, float value )
{
    glUniform1fARB ( loc, value );

    return true;
}

bool    GlslProgram :: setUniformInt ( const char * name, int value )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return false;

    glUniform1iARB ( loc, value );

    return true;
}

bool    GlslProgram :: setUniformInt ( int loc, int value )
{
    glUniform1iARB ( loc, value );

    return true;
}

bool    GlslProgram :: setUniformMatrix  ( const char * name, const Matrix4x4& value )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix4fvARB ( loc, 1, GL_FALSE, value [0] );

    return true;
}

bool    GlslProgram :: setUniformMatrix  ( const char * name, float value [16] )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix4fvARB ( loc, 1, GL_FALSE, value );

    return true;
}

bool    GlslProgram :: setUniformMatrix  ( const char * name, const Matrix3D& value )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return false;

    glUniformMatrix3fvARB ( loc, 1, GL_FALSE, value [0] );

    return true;
}

int     GlslProgram :: locForUniformName ( const char * name )
{
    return glGetUniformLocationARB ( program, name );
}

Vector4D    GlslProgram :: getUniformVector ( const char * name )
{
    float   values [4];

    int loc = glGetUniformLocationARB ( program, name );

    if ( loc < 0 )
        return Vector4D ( 0, 0, 0, 0 );

    glGetUniformfvARB ( program, loc, values );

    return Vector4D ( values [0], values [1], values [2], values [3] );
}

bool    GlslProgram :: setTexture ( const char * name, int texUnit )
{
    int loc = glGetUniformLocationARB ( program, name );

    if ( loc == -1 )
        return false;

    glUniform1iARB ( loc, texUnit );

    return true;
}

bool    GlslProgram :: setTexture ( int loc, int texUnit )
{
    if ( loc < 0 )
        return false;

    glUniform1iARB ( loc, texUnit );

    return true;
}

bool    GlslProgram :: bindAttributeTo  ( int loc, const char * name )
{
    glBindAttribLocationARB ( program, loc, name );

    return true;
}

bool    GlslProgram :: setAttribute ( const char * name, const Vector4D& value )
{
    int index = glGetAttribLocationARB ( program, name );

    if ( index < 0 )
        return false;

    glVertexAttrib4fvARB ( index, value );

    return true;
}

bool    GlslProgram :: setAttribute ( int index, const Vector4D& value )
{
    glVertexAttrib4fvARB ( index, value );

    return true;
}

int     GlslProgram :: indexForAttrName ( const char * name )
{
    return glGetAttribLocationARB ( program, name );
}

Vector4D    GlslProgram :: getAttribute ( const char * name )
{
    int index = glGetAttribLocationARB ( program, name );

    if ( index < 0 )
        return Vector4D ( 0, 0, 0, 0 );

    float   buf [4];

    glGetVertexAttribfvARB ( index, GL_CURRENT_VERTEX_ATTRIB_ARB, buf );

    return Vector4D ( buf [0], buf [1], buf [2], buf [3] );
}

Vector4D    GlslProgram :: getAttribute ( int index )
{
    float   buf [4];

    glGetVertexAttribfvARB ( index, GL_CURRENT_VERTEX_ATTRIB_ARB, buf );

    return Vector4D ( buf [0], buf [1], buf [2], buf [3] );
}

bool    GlslProgram :: isSupported ()
{
    return GL_ARB_shading_language_100 &&
           GL_ARB_shader_objects && GL_ARB_vertex_shader && GL_ARB_fragment_shader;

}

string  GlslProgram :: version ()
{
#ifdef	MACOSX
	return "1.051";
#else
    const char * slVer = (const char *) glGetString ( GL_SHADING_LANGUAGE_VERSION_ARB );

    if ( glGetError() != GL_NO_ERROR )
        return "1.051";

    return string ( slVer );
#endif
}

int     GlslProgram :: maxVertexUniformComponents ()
{
    GLint maxVertexUniformComponents;

    glGetIntegerv ( GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB, &maxVertexUniformComponents );

    return maxVertexUniformComponents;
}

int     GlslProgram :: maxVertexAttribs ()
{
    GLint maxVertexAttribs;

    glGetIntegerv ( GL_MAX_VERTEX_ATTRIBS_ARB, &maxVertexAttribs );

    return maxVertexAttribs;
}

int     GlslProgram :: maxFragmentTextureUnits ()
{
    GLint maxFragmentTextureUnits;

    glGetIntegerv ( GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxFragmentTextureUnits );

    return maxFragmentTextureUnits;
}

int     GlslProgram :: maxVertexTextureUnits ()
{
    GLint maxVertexTextureUnits;

    glGetIntegerv ( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB, &maxVertexTextureUnits );

    return maxVertexTextureUnits;
}

int     GlslProgram :: maxCombinedTextureUnits ()
{
    GLint maxCombinedTextureUnits;

    glGetIntegerv ( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB, &maxCombinedTextureUnits );

    return maxCombinedTextureUnits;
}

int     GlslProgram :: maxVaryingFloats ()
{
    GLint maxVaryingFloats;

    glGetIntegerv ( GL_MAX_VARYING_FLOATS_ARB, &maxVaryingFloats );

    return maxVaryingFloats;
}

int     GlslProgram :: maxFragmentUniformComponents ()
{
    GLint maxFragmentUniformComponents;

    glGetIntegerv ( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB, &maxFragmentUniformComponents );

    return maxFragmentUniformComponents;
}

int     GlslProgram :: maxTextureCoords ()
{
    GLint maxTextureCoords;

    glGetIntegerv ( GL_MAX_TEXTURE_COORDS_ARB, &maxTextureCoords );

    return maxTextureCoords;
}

