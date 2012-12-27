///////////////////////////////////////////////////////////////////////////////
// main.cpp : main file for 'Superstars' demo
// Author: Quin Pendragon
// Date:   2003-01-27

//#include "Interface.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "TextureManager.h"
#include "vect.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

//#define WINDOWED

//demo resolution
const int WIDTH = 800;
const int HEIGHT = 600;
const int BPP = 32;

const int START_POS = 0;

//other constants
const float PI = 3.1415926f;

const float PERSONHEIGHT = 8.0f;
const float CELLSPACING = 2.4f;

//----------------------------------------------------------------------------//
//helpful structures and small classes

//STATUS - holds info about the current position in the demo
typedef struct {
	//current-time info
	int pos;    //current pattern
	int row;    //current row within pattern (out of 64)
	int subrow; //current position within row (out of 256)
	float songtime;
	//screen size
	int width;
	int height;
} STATUS;

//LIGHT - All information needed to define a light source
typedef struct LIGHT {
	bool enabled;       //true iff the light is enabled
	bool directional;   //true iff the light is at an infinite distance
	float spotExponent; //exponent controlling intensity dropoff vs. angle
	float spotCutoff;   //maximum angle at which the spotlight lights objects
	float attConstant;  //constant part of the attenuation equation
	float attLinear;    //linear part of the attenuation equation
	float attQuadratic; //quadratic part of the attenuation equation
	colour colAmbient;  //ambient light intensity
	colour colDiffuse;  //diffuse light intensity
	colour colSpecular; //specular light intensity
	vect position;      //light position
	vect spotDirection; //direction of spotlight
	LIGHT() {
		enabled = false;
		directional = true;
		position = vect(0, 1, 0);
		spotDirection = vect(0, -1, 0);
		spotExponent = 10.0f;
		spotCutoff = 180.0f;
		attConstant = 1;
		attLinear = 0;
		attQuadratic = 0;
		colAmbient = colour(0.2f);
		colDiffuse = colour(0.8f);
		colSpecular = colour(0.0f);
	}
} LIGHT;

//VERTEX - Ye standard vertex. Contains position in object space,
//         vertex normal, and texture coordinates
typedef struct VERTEX {
	float u, v;       //texture coordinates
	float nx, ny, nz; //normal
	float x, y, z;    //position
	VERTEX() :
			u(0), v(0), nx(0), ny(0), nz(0), x(0), y(0), z(0) {
	}
	VERTEX(float x1, float y1, float z1, //constructor
			float nx1, float ny1, float nz1, float u1, float v1) :
			x(x1), y(y1), z(z1), nx(nx1), ny(ny1), nz(nz1), u(u1), v(v1) {
	}

} VERTEX;

//TRIANGLE - An tryangle.
typedef struct TRIANGLE {
	unsigned int vertex1;
	unsigned int vertex2;
	unsigned int vertex3;
	TRIANGLE() :
			vertex1(0), vertex2(0), vertex3(0) {
	}
	TRIANGLE(unsigned int v1, unsigned int v2, unsigned int v3) :
			vertex1(v1), vertex2(v2), vertex3(v3) {
	}
} TRIANGLE;

//vectors of the above
typedef vector<VERTEX> vertexvector;
typedef vector<TRIANGLE> trianglevector;

//MESH - holds a bunch of vertices, and a bunch of indexed triangles
typedef struct {
	vertexvector vertices;
	trianglevector triangles;
	//renders the triangles
	void Render() {
		glInterleavedArrays(GL_T2F_N3F_V3F, 0,
				&vertices.begin());
		glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT,
				&triangles.begin());
	}
	//fills in the indexes appropriately for a square mesh of rows x cols verts.
	void FillIndicesSquareMesh(int rows, int cols) {
		triangles.clear();
		rows--;
		cols--;
		int stride = cols + 1;
		TRIANGLE t;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				int pos = j + i * stride;
				t.vertex1 = pos + stride;
				t.vertex2 = pos;
				t.vertex3 = pos + 1;
				triangles.push_back(t);
				t.vertex1 = pos + stride;
				t.vertex2 = pos + 1;
				t.vertex3 = pos + stride + 1;
				triangles.push_back(t);
			}
		}
	}
	//calculates normals for the mesh vertices.
	void CalculateNormals() {
		//zero normals for all vertices
		for (int vert = vertices.size() - 1; vert >= 0; --vert) {
			vertices[vert].nx = 0.0f;
			vertices[vert].ny = 0.0f;
			vertices[vert].nz = 0.0f;
		}
		//for each triangle, calculate normal and add to each vertex normal
		for (int tri = triangles.size() - 1; tri >= 0; --tri) {
			//calculate the normal
			TRIANGLE& t = triangles[tri];
			vect v1(vertices[t.vertex2].x - vertices[t.vertex1].x,
					vertices[t.vertex2].y - vertices[t.vertex1].y,
					vertices[t.vertex2].z - vertices[t.vertex1].z);
			vect v2(vertices[t.vertex3].x - vertices[t.vertex1].x,
					vertices[t.vertex3].y - vertices[t.vertex1].y,
					vertices[t.vertex3].z - vertices[t.vertex1].z);
			vect normal = (v1 * v2).normalize();
			//add it to each vertex's normal
			vertices[t.vertex1].nx += normal.x;
			vertices[t.vertex1].ny += normal.y;
			vertices[t.vertex1].nz += normal.z;
			vertices[t.vertex2].nx += normal.x;
			vertices[t.vertex2].ny += normal.y;
			vertices[t.vertex2].nz += normal.z;
			vertices[t.vertex3].nx += normal.x;
			vertices[t.vertex3].ny += normal.y;
			vertices[t.vertex3].nz += normal.z;
		}
		//normalize the vertex normals
		for (int vert = vertices.size() - 1; vert >= 0; --vert) {
			VERTEX& v = vertices[vert];
			float mag = 1.0f
					/ (float) sqrt(v.nx * v.nx + v.ny * v.ny + v.nz * v.nz);
			v.nx *= mag;
			v.ny *= mag;
			v.nz *= mag;
		}
	}

} MESH;

//Path - defines a piecewise-linear path in 3D space.
class Path {
	typedef vector<vect> pointvector;
	typedef vector<float> floatvector;

public:
	//add a point to the path
	void AddPoint(const float time, const vect& point) {
		int pos = times.size() - 1;
		while (pos > 0 && times[pos] >= time)
			--pos;
		if (pos == times.size() - 1) {
			points.push_back(point);
			times.push_back(time);
		} else {
			points.insert(points.begin() + pos + 1, point);
			times.insert(times.begin() + pos + 1, time);
		}
	}

	//get the position along the path at a given time
	vect GetPos(const float time) {
		int pos = times.size() - 1, size = pos;
		if (pos < 0)
			return vect(0, 0, 0);
		while (pos >= 0 && times[pos] > time)
			--pos;
		if (pos < 0)
			return points[0];
		if (pos == size)
			return points[pos];
		float t = (time - times[pos]) / (times[pos + 1] - times[pos]);
		return points[pos] * (1.0f - t) + points[pos + 1] * t;
	}

	//clears the path
	void Clear() {
		points.clear();
		times.clear();
	}

private:
	pointvector points;
	floatvector times;
};

//ParticleSwarm - Maintains a swarm of particles.
class ParticleSwarm {
public:
	typedef vector<vect> vectvector;

	ParticleSwarm() :
			m_targetDist(0), m_attract(1.0f), m_repel(0.0f) {
	}

	//adds a particle to the swarm.
	void AddParticle(const vect& pos, const vect& vel) {
		m_pos.push_back(pos);
		m_vel.push_back(vel);
	}

	//sets the preferred particle distance and spring strengths
	void SetConstants(const float targetDist, const float attract,
			const float repel) {
		m_targetDist = (targetDist > 0.0f) ? targetDist : 0.0f;
		m_attract = attract;
		m_repel = repel;
	}

	//steps the simulation
	void Step(const float timediff) {
		m_centerofmass = 0.0f;
		int numparticles = m_pos.size();
		for (int i = 0; i < numparticles; i++) {
			for (int j = i + 1; j < numparticles; j++) {
				vect diff = m_pos[i] - m_pos[j];
				float mag = diff.magnitude();
				float force = (mag > m_targetDist) ? m_attract : -m_repel;
				diff *= force * timediff / mag;
				m_vel[i] -= diff;
				m_vel[j] += diff;
			}
		}
		for (int i = 0; i < numparticles; i++)
			m_centerofmass += m_pos[i] += timediff * m_vel[i];
		if (numparticles > 0)
			m_centerofmass /= numparticles;
	}

	//returns the position of the given particle
	vect GetPos(const int particle) {
		return m_pos[particle];
	}

	//returns the velocity of the given particle
	vect GetVel(const float particle) {
		return m_vel[particle];
	}

	//returns the number of particles
	int GetNumParticles() {
		return m_pos.size();
	}

	//returns the center of mass
	vect& GetCenterOfMass() {
		return m_centerofmass;
	}

private:
	vect m_centerofmass;
	vectvector m_pos;
	vectvector m_vel;
	float m_targetDist;
	float m_attract;
	float m_repel;
};

//----------------------------------------------------------------------------//
//spherical variables

//global pointer to the IInterface object
//IInterface *iface;

//global variables
//HMUSIC g_music;     //the BASS music handle
STATUS g_status;    //the current demo status
GLuint g_font;      //a vector font, for text output
float g_fov;       //the current FOV used for building projection matrices
float g_aspect;    //the current aspect ratio

TextureManager g_texman; //the texture management unit

//paths used
Path g_camForPatterns34;
Path g_camForPatterns56;
Path g_camForPatterns9to16;
Path g_camForPatterns17to22;
Path g_camForPatterns23to26_from;
Path g_camForPatterns23to26_targ;
Path g_camForPatterns27to30;

//the particle swarm for the flying-shiny-people scene
ParticleSwarm g_particles;

//HWND retrieval function. Breaks platform independance. pfooey.
//extern int GetHwnd(IInterface *iface);

//----------------------------------------------------------------------------//
//helper functions

//do the standard OpenGL initialization
void StandardInitGL() {
	int width = 0, height = 1, bpp = 0;
//	iface->GetSize(width, height, bpp);

	g_fov = 45.0f;
	g_aspect = (float) width / height;

	g_status.width = width;
	g_status.height = height;

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fov, g_aspect, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

//set the camera to the given position and Euler angles
void SetCameraPos(const vect& pos, float roll, float pitch, float yaw) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(-roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(-pos.x, -pos.y, -pos.z);
}

//set the camera to the given position, looking at the given target. broken.
void SetCameraPos(const vect& pos, const vect& target, const vect& target_up =
		vect(0, 1, 0)) {
	float t[16];
	//find a basis for the new position
	vect fwd = (target - pos).normalize();
	vect right = (fwd * target_up).normalize();
	vect up = right * fwd;
	//set up a new projection matrix
	t[0] = right.x;
	t[1] = up.x;
	t[2] = -fwd.x;
	t[3] = 0.0f;
	t[4] = right.y;
	t[5] = up.y;
	t[6] = -fwd.y;
	t[7] = 0.0f;
	t[8] = right.z;
	t[9] = up.z;
	t[10] = -fwd.z;
	t[11] = 0.0f;
	t[12] = (pos % right);
	t[13] = (pos % up);
	t[14] = (pos % fwd);
	t[15] = 1.0f;
	//plunck it in
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(t);
}

//makes the camera randomly orbit within the box between corner1 and corner2,
//looking at the given point. The camera stays near the 
void CamRandomOrbit(const float time, const vect& point, const vect& corner1,
		const vect& corner2) {
	vect pos((float) sin(time * 1.31f), (float) cos(time * 1.52f),
			(float) cos(time * 0.86f));
	pos.x = corner1.x + (0.5f + 0.5f * pos.x) * (corner2.x - corner1.x);
	pos.y = corner1.y + (0.5f + 0.5f * pos.y) * (corner2.y - corner1.y);
	pos.z = corner1.z + (0.5f + 0.5f * pos.z) * (corner2.z - corner1.z);
	SetCameraPos(pos, point);
}

//render some text saying what track we're at.
void RenderStatusText(bool toggle = false) {
	static bool render = false;
	static char text[256];

	render = render ^ toggle;

	if (render) {
		glPushAttrib(GL_LIGHTING_BIT | GL_TRANSFORM_BIT);
		sprintf(text, "pos = %02d, row = %02d.%03d, time = %02f", g_status.pos,
				g_status.row, g_status.subrow, g_status.songtime);
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(0.1f, 0.1f, 0.1f);
		glTranslatef(-9.0f, 8.0f, 0.0f);
		glListBase(g_font);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
	}
}

//activate the given OpenGL light source, with the given parameters
void ActivateLight(GLuint index, LIGHT *light, bool enabled = true) {
	if (enabled) {
		int n = index;
		float vec4d[4] = { 0, 0, 0, 1 };
		glEnable(n);
		//set light colours
		*((vect *) &vec4d) = light->colAmbient;
		glLightfv(n, GL_AMBIENT, vec4d);
		*((vect *) &vec4d) = light->colDiffuse;
		glLightfv(n, GL_DIFFUSE, vec4d);
		*((vect *) &vec4d) = light->colSpecular;
		glLightfv(n, GL_SPECULAR, vec4d);
		//set the light position
		vec4d[3] = light->directional ? 0.0f : 1.0f;
		*((vect *) &vec4d) = light->position;
		glLightfv(n, GL_POSITION, vec4d);
		//set the spotlight parameters
		*((vect *) &vec4d) = light->spotDirection;
		glLightfv(n, GL_SPOT_DIRECTION, vec4d);
		glLightfv(n, GL_SPOT_EXPONENT, &light->spotExponent);
		glLightfv(n, GL_SPOT_CUTOFF, &light->spotCutoff);
		//set the attenuation parameters
		glLightfv(n, GL_CONSTANT_ATTENUATION, &light->attConstant);
		glLightfv(n, GL_LINEAR_ATTENUATION, &light->attLinear);
		glLightfv(n, GL_QUADRATIC_ATTENUATION, &light->attQuadratic);
	} else
		glDisable(index);
}

//enables sphere mapping
void EnableSphereMapping() {
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

//enables fog
void EnableFog(colour col, float density = 1.0f, float start = 0.0f, float end =
		1.0f, GLuint fogType = GL_EXP) {
	float colour[4] = { col.r, col.g, col.b, 0 };
	glFogi(GL_FOG_MODE, fogType);
	glFogf(GL_FOG_DENSITY, density);
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
	glFogfv(GL_FOG_COLOR, colour);
	glEnable(GL_FOG);
}

//disables fog
void DisableFog() {
	glDisable(GL_FOG);
}

//draw a box with sizes given by the x/y/z components of the vector
void DrawBox(vect s) {
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);		// -x face
	glTexCoord2f(0, 0);
	glVertex3f(-s.x, -s.y, -s.z);
	glTexCoord2f(1, 0);
	glVertex3f(-s.x, -s.y, s.z);
	glTexCoord2f(1, 1);
	glVertex3f(-s.x, s.y, s.z);
	glTexCoord2f(0, 1);
	glVertex3f(-s.x, s.y, -s.z);
	glNormal3f(1, 0, 0);		//+x face
	glTexCoord2f(0, 0);
	glVertex3f(s.x, -s.y, -s.z);
	glTexCoord2f(1, 0);
	glVertex3f(s.x, -s.y, s.z);
	glTexCoord2f(1, 1);
	glVertex3f(s.x, s.y, s.z);
	glTexCoord2f(0, 1);
	glVertex3f(s.x, s.y, -s.z);
	glNormal3f(0, -1, 0);		//-y face
	glTexCoord2f(0, 0);
	glVertex3f(-s.x, -s.y, -s.z);
	glTexCoord2f(1, 0);
	glVertex3f(-s.x, -s.y, s.z);
	glTexCoord2f(1, 1);
	glVertex3f(s.x, -s.y, s.z);
	glTexCoord2f(0, 1);
	glVertex3f(s.x, -s.y, -s.z);
	glNormal3f(0, 1, 0);		//+y face
	glTexCoord2f(0, 0);
	glVertex3f(-s.x, s.y, -s.z);
	glTexCoord2f(1, 0);
	glVertex3f(-s.x, s.y, s.z);
	glTexCoord2f(1, 1);
	glVertex3f(s.x, s.y, s.z);
	glTexCoord2f(0, 1);
	glVertex3f(s.x, s.y, -s.z);
	glNormal3f(0, 0, -1);		// -z face
	glTexCoord2f(0, 0);
	glVertex3f(-s.x, -s.y, -s.z);
	glTexCoord2f(1, 0);
	glVertex3f(s.x, -s.y, -s.z);
	glTexCoord2f(1, 1);
	glVertex3f(s.x, s.y, -s.z);
	glTexCoord2f(0, 1);
	glVertex3f(-s.x, s.y, -s.z);
	glNormal3f(0, 0, 1);		// -z face
	glTexCoord2f(0, 0);
	glVertex3f(-s.x, -s.y, s.z);
	glTexCoord2f(1, 0);
	glVertex3f(s.x, -s.y, s.z);
	glTexCoord2f(1, 1);
	glVertex3f(s.x, s.y, s.z);
	glTexCoord2f(0, 1);
	glVertex3f(-s.x, s.y, s.z);
	glEnd();
}

//draws the given text, starting at the given time.
void DrawText(const char *text, float starttime, float curtime, float x,
		float y, float scale, bool linger = false, colour col = 1.0f) {
	const float CHAR_FADE_TIME = 3.0f;
	const float CHAR_INTERVAL = 0.05f;

	float etime = curtime - starttime;
	if (etime < 0.0f)
		return;

	glPushAttrib(
			GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT
					| GL_LIGHTING_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glScalef(scale, scale, 1.0f);

	for (int i = 0; text[i] && etime > i * CHAR_INTERVAL; i++) {
		float charetime = etime - i * CHAR_INTERVAL;
		if (linger)
			charetime *= 0.5f;
		float blend = (CHAR_FADE_TIME - charetime) / CHAR_FADE_TIME;
		if (blend < 0.0f)
			blend = 0.0f;
		if (blend > 1.0f)
			blend = (charetime - CHAR_INTERVAL) / CHAR_INTERVAL;
		glColor4f(col.r, col.g, col.b, blend);
		glCallList(g_font + text[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}

//returns a point which starts at the given start pos, ends at the given end pos,
//and travels linearly from startpos to endpos between starttime and endtime.
vect MovePoint(const float starttime, const vect& startvect,
		const float endtime, const vect& endvect) {
	float pos = (g_status.songtime - starttime) / (endtime - starttime);
	if (pos < 0.0f)
		pos = 0.0f;
	if (pos > 1.0f)
		pos = 1.0f;
	return startvect * (1.0f - pos) + endvect * pos;
}

//returns a point on the unit sphere represented by lat/long theta/phi
vect& GetSpherePos(float theta, float phi) {
	static vect result;
	phi = (phi - 0.5f);
	theta *= 2;
	result.x = (float) sin(phi * PI);
	result.y = (float) cos(phi * PI) * (float) sin(theta * PI);
	result.z = (float) cos(phi * PI) * (float) cos(theta * PI);
	return result;
}

//sets the texture to the given-filenamed texture
void SetTexture(const char *filename) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_texman.LoadTexture(filename));
}

//returns a random float between minval and maxval
inline float random(float minval, float maxval) {
	return (maxval - minval) * rand() / RAND_MAX + minval;
}

//----------------------------------------------------------------------------//
//functions for testing whether various samples are playing

bool DetectVoice1() {
	float r = g_status.row;
	if (0 <= r && r <= 2)
		return true;
	if (15 <= r && r <= 18)
		return true;
	if (31 <= r && r <= 35)
		return true;
	if (47 <= r && r <= 53)
		return true;
	return false;
}

bool DetectHighHat() {
	switch (g_status.row) {
	case 4:
	case 7:
	case 15:
	case 19:
	case 34:
	case 36:
	case 41:
	case 47:
	case 49:
	case 55:
	case 59:
		return true;

	default:
		return false;
	}
	return false;
}

bool DetectCroak() {
	switch (g_status.row) {
	case 4:
	case 12:
	case 20:
	case 28:
	case 36:
	case 44:
	case 52:
	case 60:
		return true;

	default:
		return false;
	}
}

bool DetectKnock() {
	if (g_status.pos == 39) {
		switch (g_status.row) {
		case 1:
		case 14:
		case 32:
		case 44:
		case 63:
			return true;
		}
		return false;
	} else if (g_status.pos == 40)
		return (g_status.row == 12 || g_status.row == 31);
	else
		return false;
}

//----------------------------------------------------------------------------//
//drawing functions for parts of the demo

//draws the white-purple-black fade in time with the gong at the start
void DrawPulse(float speed = 1.0f) {
	//save attributes
	glPushAttrib(
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT
					| GL_TRANSFORM_BIT | GL_LIGHTING_BIT);
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//set the colour
	float phase = (float) fmod(g_status.songtime * speed, 4);
	colour col;
	col.g = 1.0f - 0.35f * phase;
	col.r = col.b = 1.0f - 0.25f * phase;
	if (col.r < 0.0f)
		col.r = 0.0f;
	glColor4f(col.r, col.g, col.b, col.b);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//draw the quad
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();
	//restore state
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

//draws some pseudofunky horizontal interference
void DrawInterference(colour stripcol, float speed = 1.0f,
		bool vertical = false) {
	const int NUM_STRIPS = 100;
	const float AVGWIDTH = 0.01f;
	const float WIDTHRANGE = 0.01f;
	const float BORDERWIDTH = 0.01f;
	const float AVGSPEED = 0.4f;
	const float SPEEDRANGE = 0.2f;
	const float BRIGHTNESS = 0.2f;
	static float s_brightness[NUM_STRIPS];
	static float s_stripwidth[NUM_STRIPS];
	static float s_stripspeed[NUM_STRIPS];
	static bool s_initialized = false;
	if (!s_initialized) {
		//set up the speed/width/colour arrays
		for (int i = 0; i < NUM_STRIPS; i++) {
			s_brightness[i] = random(0, BRIGHTNESS);
			s_stripwidth[i] = random(AVGWIDTH - WIDTHRANGE,
					AVGWIDTH + WIDTHRANGE);
			s_stripspeed[i] = random(AVGSPEED - SPEEDRANGE,
					AVGSPEED + SPEEDRANGE);
		}
		//and only do it once
		s_initialized = true;
	}
	//save attributes
	glPushAttrib(
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT
					| GL_TRANSFORM_BIT | GL_LIGHTING_BIT);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	if (vertical)
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	//set up blending
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//draw the quads
	for (int i = 0; i < NUM_STRIPS; i++) {
		colour col = stripcol * s_brightness[i];
		float h = s_stripwidth[i];
		float y = (float) sin(speed * s_stripspeed[i] * g_status.songtime);
		if (y > 1.0f)
			y = 1.0f - y;
		glBegin(GL_QUAD_STRIP);
		//draw the lower fade-in rect
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, y - h, 0.0f);
		glVertex3f(1.0f, y - h, 0.0f);
		glColor3f(col.r, col.g, col.b);
		glVertex3f(-1.0f, y - h + BORDERWIDTH, 0.0f);
		glVertex3f(1.0f, y - h + BORDERWIDTH, 0.0f);
		//draw the middle, solid rect
		glVertex3f(-1.0f, y + h - BORDERWIDTH, 0.0f);
		glVertex3f(1.0f, y + h - BORDERWIDTH, 0.0f);
		//draw the fade-out rect
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, y + h, 0.0f);
		glVertex3f(1.0f, y + h, 0.0f);
		glEnd();
	}
	//restore state
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

//draws one of the stylised 'people' from the concert scene
void RenderPerson(float armpitch, float stretch, float lean,
		bool shadow = false) {
	static MESH s_head;
	static MESH s_arms;
	static MESH s_legs;
	static bool s_initialized = false;
	const int HEAD_RES = 8;
	const int LIMB_ROWS = 8;
	const int LIMB_COLS = 8;
	const float HEADHEIGHT = 6.4f;
	const float ARMHEIGHT = 4.4f;
	const float LEGHEIGHT = 3.0f;
	const float LIMB_WIDTH = 3.0f;
	const float LIMB_HEIGHT = 0.8f;
	const float ARM_HEIGHT = 2.0f;
	const float LEG_HEIGHT = 3.0f;
	//set up vertex data needed
	if (!s_initialized) {
		//initialize the person's head
		s_head.vertices.resize(HEAD_RES * HEAD_RES);
		for (int theta = 0; theta < HEAD_RES; theta++) {
			for (int phi = 0; phi < HEAD_RES; phi++) {
				VERTEX& vert = s_head.vertices[phi + theta * HEAD_RES];
				vect& pos = GetSpherePos((float) phi / (HEAD_RES - 1),
						(float) theta / (HEAD_RES - 1));
				vert.nx = vert.x = pos.x;
				vert.ny = vert.y = pos.y;
				vert.nz = vert.z = pos.z;
				vert.u = 0;
				vert.v = 0;
			}
		}
		//initialize the person's limbs
		s_arms.vertices.resize(LIMB_ROWS * LIMB_COLS);
		s_legs.vertices.resize(LIMB_ROWS * LIMB_COLS);
		for (int theta = 0; theta < LIMB_ROWS; theta++) {
			for (int phi = 0; phi < LIMB_COLS; phi++) {
				float fphi = (float) phi / (LIMB_COLS - 1);
				float ftheta = (float) theta / (LIMB_ROWS - 1);
				VERTEX& verta = s_arms.vertices[phi + theta * LIMB_COLS];
				VERTEX& vertl = s_legs.vertices[phi + theta * LIMB_COLS];
				vect& pos = GetSpherePos(fphi, ftheta);
				verta.nx = pos.x;
				verta.x = pos.x * LIMB_WIDTH;
				verta.ny = pos.y;
				verta.y = pos.y * LIMB_HEIGHT;
				verta.nz = verta.z = pos.z;
				verta.u = 0;
				verta.v = 0;
				vertl = verta;
				verta.y *= LIMB_HEIGHT;
				verta.y += ARM_HEIGHT * pos.x * pos.x;
				vertl.y -= LEG_HEIGHT * pos.x * pos.x;
			}
		}
		//initialize vertices for all bits
		s_head.FillIndicesSquareMesh(HEAD_RES, HEAD_RES);
		s_arms.FillIndicesSquareMesh(LIMB_ROWS, LIMB_COLS);
		s_legs.FillIndicesSquareMesh(LIMB_ROWS, LIMB_COLS);
		//make sure all this is only done once
		s_initialized = true;
	}
	//initialize transform
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//apply scale/shear matrix
	float ssm[16] = { 1.0f, 0.0f, 0.0f, 0.0f, lean, 1.0f + stretch, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	glMultMatrixf(ssm);
	//render the head
	glTranslatef(0.0f, HEADHEIGHT, 0.0f);
	s_head.Render();
	//render the arms
	glPushMatrix();
	glTranslatef(0.0f, ARMHEIGHT - HEADHEIGHT, 0.0f);
	glRotatef(armpitch, 1, 0, 0);
	s_arms.Render();
	glPopMatrix();
	//render the legs
	glTranslatef(0.0f, LEGHEIGHT - HEADHEIGHT, 0.0f);
	s_legs.Render();

	glPopMatrix();

	if (shadow) {
		glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glColor3f(0.0f, 0.0f, 0.0f);
		//render the head's shadow
		glPushMatrix();
		glScalef(1.0f, 0.0f, 1.0f);
		s_head.Render();
		glPopMatrix();
		//render the arms' shadow
		glPushMatrix();
		glScalef(1.0f, 0.0f, 1.0f);
		glRotatef(armpitch, 1, 0, 0);
		s_arms.Render();
		glPopMatrix();
		//render the legs' shadow
		glPushMatrix();
		glScalef(1.0f, 0.0f, 1.0f);
		s_legs.Render();
		glPopMatrix();
		glPopAttrib();
	}
}

//draw a square unit cell, centered around the origin, with a person inside it
void DrawCell(float x, float y, float z, float armpitch, float stretch,
		float lean) {
	//draw the person within
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	SetTexture("shine.bmp");
	EnableSphereMapping();
	glPushMatrix();
	glTranslatef(x, y, z);
	glPushMatrix();
	glTranslatef(0.0f, -1.0f, 0.0f);
	glScalef(2.0f / PERSONHEIGHT, 2.0f / PERSONHEIGHT, 2.0f / PERSONHEIGHT);
	RenderPerson(armpitch, stretch, lean);
	glPopMatrix();
	glPopAttrib();
	//save attributes
	glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_LIGHTING_BIT);
	//draw the cube
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glMatrixMode(GL_MODELVIEW);
	glDepthMask(false);
	SetTexture("greenbox.bmp");
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	DrawBox(vect(1.0f, 1.0f, 1.0f));
	glDepthMask(true);
	//restore settings
	glPopMatrix();
	glPopAttrib();
}

//draw a cell grid, of the given 'radius' w.r.t. each axis,
//with the center cell at the origin
const int CELLMODE_SWAY = 1; //cell occupant will sway side to side
const int CELLMODE_BOP1 = 2; //cell occupant will bop up and down on the beat
const int CELLMODE_BOP2 = 3; //cell occupant will bop up and down off the beat
void DrawCellGrid(float time, int xs, int ys, int zs, int evenMode = 0,
		int oddMode = 0) {
	glPushAttrib(GL_ENABLE_BIT);
	for (int x = 0; x < xs; x++) {
		for (int y = 0; y < ys; y++) {
			for (int z = 0; z < zs; z++) {
				float stretch = 0, lean = 0;
				float px = CELLSPACING * x;
				float py = CELLSPACING * y;
				float pz = CELLSPACING * z;
				int mode = ((x + y + z) & 1) ? oddMode : evenMode;
				if (mode == 1)
					lean = 0.15f * (float) sin(time * PI);
				else if (mode == 2)
					stretch = 0.15f * (float) pow(sin((time + 0.25f) * PI), 12);
				else if (mode == 3)
					stretch = 0.15f * (float) pow(sin((time + 0.75f) * PI), 12);
				DrawCell(px, py, pz, 0, stretch, lean);
				if (x > 0)
					DrawCell(-px, py, pz, 0, stretch, lean);
				if (y > 0)
					DrawCell(px, -py, pz, 0, stretch, lean);
				if (z > 0)
					DrawCell(px, py, -pz, 0, stretch, lean);
				if (x > 0 && y > 0)
					DrawCell(-px, -py, pz, 0, stretch, lean);
				if (x > 0 && z > 0)
					DrawCell(-px, py, -pz, 0, stretch, lean);
				if (y > 0 && z > 0)
					DrawCell(px, -py, -pz, 0, stretch, lean);
				if (x > 0 && y > 0 && z > 0)
					DrawCell(-px, -py, -pz, 0, stretch, lean);
			}
		}
	}
	glPopAttrib();
}

//draw a flower-looking thing for the worshipping people to stand on
void DrawFlower(float petalcurl) {
	static MESH s_middle;
	static MESH s_petal;
	static MESH s_fringe;

	const int MIDDLE_ROWS = 16;
	const int MIDDLE_COLS = 16;
	const float MIDDLE_RADIUS = 12;
	const float MIDDLE_POWER = 0.1f;
	const float MIDDLE_DROPOFF = 6.0f;
	const float MIDDLE_RAND = 2.0f;

	const int FRINGE_PETALS = 24;
	const float FRINGE_HEIGHT = -2.0f;
	const float FRINGE_SIZE = 3.0f;
	const float FRINGE_RAND = 1.0f;
	const float FRINGE_SLOPE = 1.0f;
	const float FRINGE_RADIUS = MIDDLE_RADIUS - 1.5f;

	const int PETAL_ROWS = 8;
	const int PETAL_COLS = 8;
	const int NUM_PETALS = 12;
	const float PETAL_LENGTH = 24;
	const float PETAL_WIDTH = 12;
	const float PETAL_HEIGHT = 8.0f;
	const float PETAL_RADIUS = MIDDLE_RADIUS - 4.0f;

	static bool initialized = false;
	if (!initialized) {
		//set up the mesh for the middle bit
		s_middle.vertices.resize(MIDDLE_ROWS * MIDDLE_COLS);
		for (int i = 0; i < MIDDLE_ROWS; i++)
			for (int j = 0; j < MIDDLE_COLS; j++) {
				VERTEX& vert = s_middle.vertices[j + i * MIDDLE_COLS];
				float x = (2.0f * i / (MIDDLE_ROWS - 1) - 1.0f);
				float z = (2.0f * j / (MIDDLE_COLS - 1) - 1.0f);
				float rad = sqrt(x * x + z * z);
				vert.y = (float) pow((rad >= 1.0f) ? 0.0f : 1.0f - rad,
						MIDDLE_POWER) * MIDDLE_DROPOFF - MIDDLE_DROPOFF
						+ (float) pow(rad, 2.5)
								* random(-MIDDLE_RAND, MIDDLE_RAND);
				vert.x = x * MIDDLE_RADIUS;
				vert.z = z * MIDDLE_RADIUS;
				vert.u = x;
				vert.v = z;
			}
		s_middle.FillIndicesSquareMesh(MIDDLE_ROWS, MIDDLE_COLS);
		s_middle.CalculateNormals();
		//set up the fringe around the outside
		s_fringe.triangles.resize(FRINGE_PETALS);
		s_fringe.vertices.reserve(FRINGE_PETALS * 2);
		float delta = PI / FRINGE_PETALS;
		for (int i = 0; i < FRINGE_PETALS; i++) {
			//set up the vertices for this petal
			float ang = 2 * PI * i / FRINGE_PETALS;
			VERTEX& v1 = s_fringe.vertices[i * 2];
			v1.x = FRINGE_RADIUS * (float) sin(ang - delta);
			v1.y = FRINGE_HEIGHT;
			v1.z = FRINGE_RADIUS * (float) cos(ang - delta);
			v1.u = (i & 1) ? 1.0f : 0.0f;
			v1.v = 0.0f;
			v1.nx = 0.0f;
			v1.ny = 0.2f;
			v1.nz = 0.0f;
			VERTEX& v2 = s_fringe.vertices[i * 2 + 1];
			v2.x = (FRINGE_RADIUS + FRINGE_SLOPE) * (float) sin(ang);
			v2.x += random(-FRINGE_RAND, FRINGE_RAND);
			v2.y = FRINGE_SIZE + random(-FRINGE_RAND, FRINGE_RAND);
			v2.z = (FRINGE_RADIUS + FRINGE_SLOPE) * (float) cos(ang);
			v2.z += random(-FRINGE_RAND, FRINGE_RAND);
			v2.u = 0.5f;
			v2.v = 1.0f;
			v2.nx = 0.0f;
			v2.ny = 0.2f;
			v2.nz = 0.0f;
			//set up the indices for this petal
			TRIANGLE& tri = s_fringe.triangles[i];
			tri.vertex1 = i * 2;
			tri.vertex2 = i * 2 + 1;
			tri.vertex3 = (i * 2 + 2) % (FRINGE_PETALS * 2);
		}
		//set up the indices for the petal mesh
		s_petal.vertices.resize(PETAL_ROWS * PETAL_COLS);
		for (int i = 0; i < PETAL_ROWS; i++)
			for (int j = 0; j < PETAL_COLS; j++) {
				VERTEX& vert = s_petal.vertices[j + i * PETAL_COLS];
				float x = (float) i / (PETAL_ROWS - 1);
				float z = (float) j / (PETAL_COLS - 1) - 0.5f;
				vert.y = PETAL_HEIGHT * (float) pow(x, 3) + FRINGE_HEIGHT;
				vert.x = PETAL_LENGTH * x + PETAL_RADIUS;
				vert.z = PETAL_WIDTH * z
						* (float) (sin((1.0f - x) * PI) + 0.01f);
				vert.u = x;
				vert.v = (z + 0.5f);
			}
		s_petal.FillIndicesSquareMesh(PETAL_ROWS, PETAL_COLS);
		s_petal.CalculateNormals();
		initialized = true;
	}
	glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	//render the middle bit
	SetTexture("flower1.bmp");
	s_middle.Render();
	//render the fringe
	s_fringe.Render();
	//render the petals
	SetTexture("flower2.bmp");
	glMatrixMode(GL_MODELVIEW);
	for (int i = 0; i < NUM_PETALS; i++) {
		glPushMatrix();
		glRotatef(360.0f * (i + 0.25f) / NUM_PETALS, 0, 1, 0);
		glRotatef(petalcurl, 0, 0, 1);
		s_petal.Render();
		glPopMatrix();
	}
	glPopAttrib();
}

//draw a 3D grid of lines, centered on the nearest integer space division to the
//given point so you never fly out of them
void DrawSpaceGrid(const vect& pos, float gridSize, float gridSpacing) {
	//find the center of the grid
	float cx = (float) floor(pos.x / gridSpacing) * gridSpacing;
	float cy = (float) floor(pos.y / gridSpacing) * gridSpacing;
	float cz = (float) floor(pos.z / gridSpacing) * gridSpacing;
	glPushMatrix();
	glTranslatef(cx, cy, cz);
	//draw from the outside in so as to avoid depth conflicts
	glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
	glLineWidth(3.0f);
	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_LIGHTING);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for (int div = gridSize; div >= 0; div -= gridSpacing) {
		for (float x = -gridSize; x <= gridSize; x += gridSpacing) {
			//top horizontal plane
			glVertex3f(-gridSize, div, x);
			glVertex3f(gridSize, div, x);
			glVertex3f(x, div, gridSize);
			glVertex3f(x, div, -gridSize);
			//right vertical bars
			glVertex3f(div, -gridSize, x);
			glVertex3f(div, gridSize, x);
			if (div > 0) {
				//bottom horizontal plane
				glVertex3f(-gridSize, -div, x);
				glVertex3f(gridSize, -div, x);
				glVertex3f(x, -div, gridSize);
				glVertex3f(x, -div, -gridSize);
				//left vertical bars
				glVertex3f(-div, -gridSize, x);
				glVertex3f(-div, gridSize, x);
			}
		}
	}
	glEnd();
	glPopMatrix();
	glPopAttrib();
}

//draws the plasma lake used in the scenes with flowers
void DrawPlasmaLake() {
	const float LAKE_WIDTH = 196.0f;
	const float LAKE_HEIGHT = -4.0f;
	const float LAKE_LAYER_DIFF = -0.5f;
	const int LAKE_LAYERS = 4;
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetTexture("plasma.bmp");
	glBegin(GL_QUADS);
	for (int i = 0; i < LAKE_LAYERS; i++) {
		float h = LAKE_HEIGHT - i * LAKE_LAYER_DIFF;
		glColor4f(1, 1, 1, 0.5f);
		glTexCoord2f(0, 0);
		glVertex3f(-LAKE_WIDTH, h, -LAKE_WIDTH);
		glTexCoord2f(1, 0);
		glVertex3f(LAKE_WIDTH, h, -LAKE_WIDTH);
		glTexCoord2f(1, 1);
		glVertex3f(LAKE_WIDTH, h, LAKE_WIDTH);
		glTexCoord2f(0, 1);
		glVertex3f(-LAKE_WIDTH, h, LAKE_WIDTH);
	}
	glEnd();
	glPopAttrib();
}

//----------------------------------------------------------------------------//
//main control functions

//Initialize() - sets up everything needed to run
void Initialize() {
	//load the music
//	if (!BASS_Init(-1, 44100, BASS_DEVICE_NOTHREAD, GetHwnd(iface)))
//	{
//		MessageBox(GetHwnd(iface), "Demo will now exit.",
//			"Error: Couldn't initialize bass.dll. ", NULL);
//		iface->Deactivate();
//	}
//	g_music = BASS_MusicLoad(false, "legend.mod", 0, 0, BASS_MUSIC_RAMP);
//	if (!g_music)
//	{
//		MessageBox(GetHwnd(iface), "Demo will now exit.",
//			"Error: Couldn't load legend.mod. ", NULL);
//		iface->Deactivate();
//	}
//	BASS_Start();
//	BASS_ChannelSetPosition(g_music, (QWORD)MAKELONG(START_POS, 0));
//	BASS_MusicSetPositionScaler(g_music, 0xFF);
	//create a font
//	g_font = iface->CreateOutlineFont("Verdana", 1, 0.1f);
	//set up basic OpenGL stuff
	StandardInitGL();
	LIGHT light;
	light.position = vect(1, 4, 1.5f).normalize();
	ActivateLight(GL_LIGHT0, &light);
	glEnable(GL_LIGHTING);
	//make the material brighter
	glMaterialf(GL_FRONT_AND_BACK, GL_AMBIENT, 1.0f);
	glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, 1.0f);
	//set up path in patterns 3 & 4, with camera flying away from/towards cube
	g_camForPatterns34.AddPoint(24.0f, vect(0, 0, -2));
	g_camForPatterns34.AddPoint(39.0f, vect(10, 10, -10));
	g_camForPatterns34.AddPoint(40.0f, vect(0, 0, -2));
	//set up path in patterns 5 to 8, with camera flying around cube train
	g_camForPatterns56.AddPoint(40.0f, vect(0, 0, -2));
	g_camForPatterns56.AddPoint(41.0f, vect(-3, -3, -3));
	g_camForPatterns56.AddPoint(42.0f, vect(-9, -3, 3));
	g_camForPatterns56.AddPoint(43.0f, vect(-9, 3, 3));
	g_camForPatterns56.AddPoint(44.0f, vect(-9, 3, -3));
	g_camForPatterns56.AddPoint(45.0f, vect(-9, 3, 3));
	g_camForPatterns56.AddPoint(46.0f, vect(-9, -3, 3));
	g_camForPatterns56.AddPoint(48.0f, vect(9, 3, 3));
	//set up the path for patterns 9 to 16, camera backs slowly away then zooms in
	g_camForPatterns9to16.AddPoint(72.0f, vect(0, 0, -2));
	g_camForPatterns9to16.AddPoint(134.5f, vect(0, 0, -20));
	g_camForPatterns9to16.AddPoint(136.0f, vect(0, 0, -2));
	//set up the path for patterns 17 to 22, camera flies around origin
	const float RADIUS = 15;
	for (float t = 136; t < 160.0f; t += 0.1f) {
		float ang = t * PI / 24 + PI / 3;
		float x = (float) sin(ang) * RADIUS;
		float z = (float) cos(ang) * RADIUS;
		float y = (t - 136) * 0.5f;
		float y2 = (t - 136) * 8.0f + 13;
		g_camForPatterns17to22.AddPoint((t - 136) * 4 / 3 + 136, vect(x, y, z));
		if (t > 158.5f)
			y2 = 192.0f - (t - 158.5f) * 110.0f;
		g_camForPatterns17to22.AddPoint((t - 136) * 2 / 3 + 32 + 136,
				vect(-x, y2, -z));
	}
	//set up path for patterns 23 to 26 (184 to 216)
	g_camForPatterns23to26_from.AddPoint(184.0f, vect(10, 20, 20));
	g_camForPatterns23to26_targ.AddPoint(184.0f, vect(0, 0, CELLSPACING * 3));
	g_camForPatterns23to26_from.AddPoint(186.0f, vect(15, 0, 0));
	g_camForPatterns23to26_targ.AddPoint(186.0f, vect(0, 0, CELLSPACING * 3));
	g_camForPatterns23to26_from.AddPoint(187.0f,
			vect(8, CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(188.0f,
			vect(0, CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(189.0f,
			vect(8, CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(190.0f,
			vect(0, CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(191.0f,
			vect(8, -CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(192.0f,
			vect(0, -CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(193.0f,
			vect(8, -CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(194.0f,
			vect(0, -CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(195.0f,
			vect(8, -CELLSPACING, -CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(196.0f,
			vect(0, -CELLSPACING, -CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(197.0f,
			vect(8, -CELLSPACING, -CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(198.0f,
			vect(0, -CELLSPACING, -CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(199.0f,
			vect(8, CELLSPACING, -CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(200.0f,
			vect(0, CELLSPACING, -CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(201.0f,
			vect(8, CELLSPACING, -CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(202.0f,
			vect(0, CELLSPACING, -CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(203.0f,
			vect(8, CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(204.0f,
			vect(0, CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(205.0f,
			vect(8, CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(206.0f,
			vect(0, CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(207.0f,
			vect(8, -CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(208.0f,
			vect(0, -CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(209.0f,
			vect(8, -CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_targ.AddPoint(210.0f,
			vect(0, -CELLSPACING, CELLSPACING));
	g_camForPatterns23to26_from.AddPoint(212.0f,
			vect(CELLSPACING, -CELLSPACING * 0, -8));
	g_camForPatterns23to26_targ.AddPoint(212.0f,
			vect(-CELLSPACING, -CELLSPACING * 0, 0));
	g_camForPatterns23to26_from.AddPoint(212.0f,
			vect(CELLSPACING, -CELLSPACING * 0, -8));
	g_camForPatterns23to26_targ.AddPoint(212.0f,
			vect(-CELLSPACING, -CELLSPACING * 0, 0));
	g_camForPatterns23to26_from.AddPoint(215.5f, vect(50, 0, 0));

	//set up the starting positions of the flying people

	const int NUMFLYINGPEOPLE = 24;
	const float FLYINGPEOPLERAD = 2.0f;
	for (int i = 0; i < NUMFLYINGPEOPLE; i++) {
		float angle = 2 * PI * i / NUMFLYINGPEOPLE;
		g_particles.AddParticle(
				vect((float) sin(angle) * FLYINGPEOPLERAD, -3.0f,
						(float) cos(angle) * FLYINGPEOPLERAD),
				vect(random(-0.01f, 0.01f), random(0.9f, 1.1f),
						random(-0.01f, 0.01f)));
	}
	g_particles.SetConstants(3.0f, 0.3f, 0.3f);

	//gogogo!
//	BASS_MusicPlay(g_music);
}

//Shutdown() - frees up anything allocated
void Shutdown() {
//	BASS_Stop();
//	BASS_MusicFree(g_music);
//	BASS_Free();
}

//DoFrame() - renders a frame, based on the current position through the demo
void DoFrame() {
	float timediff = g_status.songtime;
	//update the current music position
//	BASS_Update();
//	QWORD musicpos = BASS_ChannelGetPosition(g_music);
//	g_status.pos = LOWORD(musicpos);
//	g_status.row = HIWORD(musicpos) >> 8;
//	g_status.subrow = HIWORD(musicpos) & 0xFF;
//	g_status.songtime = (64.0f * g_status.pos
//		+ (float)HIWORD(musicpos) / 0xFF) / 8.0f;
//	if (BASS_ACTIVE_STOPPED == BASS_ChannelIsActive(g_music))
//		iface->Deactivate();
	timediff = g_status.songtime - timediff;
	if (timediff > 0.1f)
		timediff = 0.1f;
	if (timediff < 0.0f)
		timediff = 0.0f;
	//render the current frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (g_status.pos) {
	//intro sequence
	case 2:
		DrawInterference(colour(1.0f, 0.5f, 1.0f), 1.0f, true);
		DrawText("if the tao is great", 18.0f, g_status.songtime, -0.8f, 0.2f,
				0.1f);
		DrawText("then the star shall shine bright", 20.0f, g_status.songtime,
				-0.34f, -0.2f, 0.1f);
	case 1:
		DrawInterference(colour(0.5f, 1.0f, 0.5f), 1.0f, false);
		DrawText("there is no way to know what", 8.0f, g_status.songtime, -0.8f,
				0.2f, 0.1f);
		DrawText("there is no way to know how", 10.0f, g_status.songtime,
				-0.24f, -0.2f, 0.1f);
		DrawText("there is no way to know when", 12.0f, g_status.songtime,
				-0.34f, 0.2f, 0.1f);
		DrawText("there is no way to know where", 14.0f, g_status.songtime,
				-0.8f, -0.2f, 0.1f);
	case 0:
		DrawPulse();
		DrawText("welcome to the superstar experience", 1.0f, g_status.songtime,
				-0.75f, 0, 0.1f, false, colour(0.72f, 0.05f, 0.95f));
		DrawText("welcome to the superstar experience", 4.0f, g_status.songtime,
				-0.75f, 0, 0.1f, false, colour(0.72f, 0.05f, 0.95f));
		break;

		//fly the camera around a single 'person', who moves a bit,
		//inside a single additive cube, on the interference background,
		//which flashes in and out depending on the voice-ish sample
	case 3:
		if (g_status.row < 30)
			DrawPulse();
	case 4: {
		int numcells = 1;
		SetCameraPos(g_camForPatterns34.GetPos(g_status.songtime), 0);
		if (g_status.pos == 4 && g_status.row >= 56)
			numcells = (g_status.row - 55) + (g_status.subrow >> 7);
		DrawCellGrid(g_status.songtime, numcells, 1, 1, CELLMODE_SWAY,
				CELLMODE_SWAY);
		if (DetectVoice1())
			DrawInterference(colour(0.8f, 0.8f, 1.0f), 1.0f, true);
		DrawText("marvel at the innate complexity", 32.0f, g_status.songtime,
				-0.7f, -0.7f, 0.1f, false, colour(0.72f, 0.05f, 0.95f));
	}
		break;

		//fly the camera up, down, and around the line of cells
	case 5:
	case 6:
	case 7:
	case 8: {
		float time = g_status.songtime;
		if (g_status.pos == 6)
			time = 96 - time;
		else if (g_status.pos == 7)
			time -= 16;
		else if (g_status.pos == 8)
			time = 112 - time;
		SetCameraPos(g_camForPatterns56.GetPos(time), 0);
		DrawCellGrid(g_status.songtime, 9, 1, 1, CELLMODE_BOP1, CELLMODE_BOP2);
		if (DetectHighHat()) {
			DrawInterference(colour(0.8f, 1, 0.8f), 2.3f, true);
			DrawInterference(colour(0.8f, 0.8f, 1), 2.6f, false);
		}
		DrawText("bong", 68.0f, g_status.songtime, -0.1f, -0.74f, 0.5f, false,
				colour(1, 0, 0));
		break;
	}

		//spend the next eight patterns looking at rows of cells, with other stuff
		//going on in front/behind to make it less boring
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16: {
		SetCameraPos(g_camForPatterns9to16.GetPos(g_status.songtime), 0);
		DrawCellGrid(g_status.songtime / 4.0f, 5, 1, 1, CELLMODE_SWAY,
				CELLMODE_SWAY);
		glMatrixMode(GL_MODELVIEW);
		for (int i = 1; i < 5; i++) {
			int bopmode = CELLMODE_BOP1;
			float y = i * CELLSPACING;
			float offset = (float) fmod(g_status.songtime, CELLSPACING);
			if (i & 1) {
				offset = -offset;
				bopmode = CELLMODE_BOP2;
			}
			glTranslatef(offset, y, 0);
			DrawCellGrid(g_status.songtime, 6, 1, 1, bopmode, bopmode);
			glTranslatef(0, -2 * y, 0);
			DrawCellGrid(g_status.songtime, 6, 1, 1, bopmode, bopmode);
			glTranslatef(-offset, y, 0);
		}
		if (DetectHighHat())
			DrawInterference(colour(0.5f, 0.5f, 0.6f), 1.0f, false);
		DrawText("as angels sing with voices of starlight", 72.0f,
				g_status.songtime, -0.95f, 0.64f, 0.12f, true,
				colour(0.7f, 0, 1));
		DrawText("the world revolves through eternity", 76.0f,
				g_status.songtime, -0.85f, -0.64f, 0.12f, true,
				colour(0.7f, 0, 1));
		DrawText("coalescent luminance", 88.0f, g_status.songtime, -0.85f,
				0.64f, 0.13f, true, colour(0.7f, 0, 1));
		DrawText("heiroglyphic transience", 92.0f, g_status.songtime, -0.45f,
				-0.64f, 0.13f, true, colour(0.7f, 0, 1));
		DrawText("falling of moondust in silver showers", 104.0f,
				g_status.songtime, -0.95f, 0.64f, 0.12f, true,
				colour(0.7f, 0, 1));
		DrawText("dawn lightens the eastern sky", 108.0f, g_status.songtime,
				-0.65f, -0.64f, 0.12f, true, colour(0.7f, 0, 1));
		DrawText("multum in parvo", 120.0f, g_status.songtime, -0.85f, 0.64f,
				0.13f, true, colour(0.7f, 0, 1));
		DrawText("many voices sing", 124.0f, g_status.songtime, -0.05f, -0.64f,
				0.13f, true, colour(0.7f, 0, 1));
	}
		break;

		//change of scene: patterns 17, 18, 21, and 22 show a circle of little people
		//'worshipping' a big person in the middle, all inside a giant flower-looking thing
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22: {
		const int NUMFLOWERS = 6;
		const float FLOWERSPACE = 56.0f;
		const float SRAD = 5.0f;
		const int NUMPEOPLE = 16;
		SetCameraPos(g_camForPatterns17to22.GetPos(g_status.songtime),
				vect(0, -PERSONHEIGHT / 2, 0));
		EnableFog(colour(0), 0.001f);
		//draw the plasma lake under the flowers
		DrawPlasmaLake();
		//render the circle
		glPushAttrib(GL_ENABLE_BIT);
		EnableSphereMapping();
		SetTexture("shine.bmp");
		glDisable(GL_LIGHTING);
		RenderPerson(0, 0.05f * (float) sin(g_status.songtime * PI / 4), 0,
				false);
		glEnable(GL_LIGHTING);
		for (int i = 0; i < NUMPEOPLE; i++) {
			float ang = 360.0f * i / NUMPEOPLE;
			float phase = 0.0f;
			float rad = (i & 1) ? 4.75f : 5.25f;
			if (g_status.pos >= 21 && (i & 1))
				phase = 180.0f;
			glPushMatrix();
			glRotatef(ang, 0, 1, 0);
			glTranslatef(0, 0, rad);
			glScalef(0.3f, 0.3f, 0.3f);
			RenderPerson(
					-45.0f
							* (float) (sin(g_status.songtime * PI / 2 + phase)
									+ 1), 0.0f, 0.0f, true);
			glPopMatrix();
		}
		glPopAttrib();
		//draw the flower field around them
		DrawFlower((float) sin(g_status.songtime * PI / 8) + 1);
		for (int i = 0; i < NUMFLOWERS; i++) {
			float angle = 2 * PI * i / NUMFLOWERS;
			glPushMatrix();
			glTranslatef((float) sin(angle) * FLOWERSPACE, 0.1f * i,
					(float) cos(angle) * FLOWERSPACE);
			DrawFlower(
					(float) sin(g_status.songtime * PI / 8 + i * 132.2f) + 1);
			glPopMatrix();
		}
		DisableFog();
		DrawText("have you ever wondered", 136.0f, g_status.songtime, -0.85f,
				0.84f, 0.13f, true, colour(0.7f, 0, 1));
		DrawText("what life would be like", 152.0f, g_status.songtime, -0.65f,
				0.84f, 0.13f, true, colour(0.7f, 0, 1));
		DrawText("if you were big and shiny", 168.5f, g_status.songtime, -0.45f,
				0.84f, 0.13f, true, colour(0.7f, 0, 1));
	}
		break;

		//back to the cube farm, this time with a horizontal 2D grid of them
		//held by big shiny guys.
	case 23:
	case 24:
	case 25:
	case 26:
		EnableFog(colour(0), 0.08f);
		SetCameraPos(g_camForPatterns23to26_from.GetPos(g_status.songtime),
				g_camForPatterns23to26_targ.GetPos(g_status.songtime),
				vect(1, 0.1f, 0.1f));
		glRotatef(270, 0, 0, 1);
		glPushAttrib(GL_ENABLE_BIT);
		EnableSphereMapping();
		SetTexture("shine.bmp");
		glDisable(GL_LIGHTING);
		for (int i = 0; i < 4; i++) {
			glPushMatrix();
			glRotatef(90 * i, 0, 1, 0);
			glTranslatef(0, -PERSONHEIGHT / 2.0f, -CELLSPACING * 3);
			RenderPerson(90, 0, 0, false);
			glPopMatrix();
		}
		glPopAttrib();
		DrawCellGrid(g_status.songtime, 3, 1, 3, CELLMODE_SWAY, CELLMODE_SWAY);
		DisableFog();
		if (g_status.songtime < 213.0f && DetectCroak())
			DrawInterference(colour(0.8f, 1.0f, 1.0f), 1.0f, false);
		DrawText("have you ever felt like", 188.0f, g_status.songtime, -0.6f,
				-0.6f, 0.15f, true, colour(1.0f, 0.0f, 0.7f));
		DrawText("you were trapped in a cube", 196.0f, g_status.songtime, -0.8f,
				-0.8f, 0.15f, true, colour(1.0f, 0.0f, 0.7f));
		break;

		//another change of scene: a flock of people flying around, in a black fog, with
		//a 3D grid of green lines to show space a little
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34: {
		float fog27 = 0.03f;
		float fog28 = 0.05f;
		float t = (g_status.songtime - 216) / 8.0f;
		if (t < 0)
			t = 0;
		if (t > 1)
			t = 1;
		float fog = fog27 * (1 - t) + fog28 * t;
		EnableFog(colour(0), fog);
		const float CAMORBITRAD = 6.0f;
		const float CAMSTARTHEIGHT = 10.0f;
		static float shearmat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
				0, 1 };
		//update the particle swarm, and move the camera to follow them
		g_particles.Step(timediff);
		vect target = g_particles.GetCenterOfMass();
		vect campos((float) sin(312.0f + g_status.songtime * PI / 12.0f),
				0.8f * (float) cos(201.0f + g_status.songtime * PI / 11.0f),
				(float) cos(312.0f + g_status.songtime * PI / 12.0f));
		campos *= CAMORBITRAD;
		if (g_status.pos == 27)
			campos.y += CAMSTARTHEIGHT * (224 - g_status.songtime);
		else if (g_status.pos == 34)
			campos.y -= CAMSTARTHEIGHT * (g_status.songtime - 272);
		SetCameraPos(campos, 0);
		glTranslatef(-target.x, -target.y, -target.z);
		//draw the particle swarm
		glPushAttrib(GL_ENABLE_BIT);
		EnableSphereMapping();
		SetTexture("shine.bmp");
		int numparticles = g_particles.GetNumParticles();
		for (int i = 0; i < numparticles; i++) {
			glPushMatrix();
			vect v = g_particles.GetPos(i);
			glTranslatef(v.x, v.y, v.z);
			glScalef(0.1f, 0.1f, 0.1f);
			v = g_particles.GetVel(i);
			shearmat[4] = 0.3f * v.x;
			shearmat[6] = 0.3f * v.z;
			glRotatef(g_status.songtime + i * 421.0f, 0, 1, 0);
			glMultMatrixf(shearmat);
			RenderPerson(0, 0, 0);
			glPopMatrix();
		}
		glPopAttrib();
		//draw the grid
		DrawSpaceGrid(campos + target, (g_status.pos == 27) ? 100.0f : 50.0f,
				5.0f);
		DisableFog();
		//white the screen out during the last zoom upwards, and
		//black it for the first second
		if (g_status.pos == 34 || g_status.songtime < 217.0f) {
			float whiteness = (g_status.songtime - 272.0f) / 8.0f;
			if (g_status.pos == 27)
				whiteness = (217.0f - g_status.songtime);
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			if (g_status.pos == 34)
				glColor4f(1, 1, 1, whiteness);
			else
				glColor4f(0, 0, 0, whiteness);
			glBegin(GL_QUADS);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
			glPopMatrix();
			glPopAttrib();
			glMatrixMode(GL_MODELVIEW);
		}
		DrawText("break free from your bonds", 222.0f, g_status.songtime, -0.8f,
				-0.6f, 0.15f, true, colour(0, 0.8f, 0));
		DrawText("soar into the sky", 228.0f, g_status.songtime, -0.4f, -0.8f,
				0.15f, true, colour(0.0f, 1.0f, 0.0f));
		DrawText("dare to dream the dream", 248.0f, g_status.songtime, -0.7f,
				-0.6f, 0.15f, true, colour(0, 0.8f, 0));
		DrawText("then you will learn to fly", 254.0f, g_status.songtime, -0.8f,
				-0.8f, 0.15f, true, colour(0.0f, 1.0f, 0.0f));
	}
		break;

		//a field of flowers, breathing to the music
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40: {
		const int NUMFLOWERS = 6;
		const float FLOWERSPACE = 64.0f;
		const float SRAD = 80.0f;
		const float FLOWER_CURL = 4.0f;
		const float BUMPSPEED = 64.0f;
		float ang = g_status.songtime * PI / 64.0f;
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glRotatef(20.0f, 1, 0, 0);
		SetCameraPos(
				vect(SRAD * (float) sin(ang), 24.0f, SRAD * (float) cos(ang)),
				0, 0, ang * 180 / PI);
		EnableFog(colour(1), 0.02f);
		//draw the plasma lake under the flowers
		DrawPlasmaLake();
		//draw the flower field
		static float mainflowerbump = 0.0f;
		if (DetectKnock())
			mainflowerbump += BUMPSPEED * timediff;
		else
			mainflowerbump -= BUMPSPEED * timediff / 8;
		if (mainflowerbump < 0)
			mainflowerbump = 0;
		DrawFlower(
				((float) sin(g_status.songtime * PI / 8) + 1) * FLOWER_CURL
						+ mainflowerbump);
		for (int i = 0; i < NUMFLOWERS; i++) {
			float angle = 2 * PI * i / NUMFLOWERS;
			glPushMatrix();
			glTranslatef((float) sin(angle) * FLOWERSPACE, 0.1f * i,
					(float) cos(angle) * FLOWERSPACE);
			DrawFlower(
					((float) sin(g_status.songtime * PI / 2 + i * 132.2f) + 1)
							* FLOWER_CURL);
			glPopMatrix();
		}
		DisableFog();
		DrawText("flowers soak in the light", 280.0f, g_status.songtime, -0.75f,
				0.6f, 0.15f, true, colour(0.7f));
		DrawText("bask in illumination", 288.0f, g_status.songtime, -0.6f, 0.8f,
				0.15f, true, colour(0.7f));
		DrawText("live life in the real world", 296.0f, g_status.songtime,
				-0.75f, 0.6f, 0.15f, true, colour(0.7f));
		DrawText("let dreams become reality", 304.0f, g_status.songtime, -0.8f,
				0.8f, 0.15f, true, colour(0.7f));
		if (g_status.songtime >= 312.0f && g_status.songtime <= 325.5f) {
			float xoff[7] = { -0.1f, 0.05f, 0.1f, -0.05f, -0.1f, 0.1f, -0.05f };
			float yoff[7] =
					{ -0.5f, -0.55f, -0.45f, -0.6f, -0.6f, -0.5f, -0.45f };
			int ind = 2 * (int) ((g_status.songtime - 312.0f) / 4.0f);
			float offset = (float) fmod(g_status.songtime, 4.0f);
			DrawText("baf", 0.0f, offset, xoff[ind], yoff[ind], 0.1f, false,
					colour(1, 0, 0));
			DrawText("baf", 1.5f, offset, xoff[ind + 1], yoff[ind + 1], 0.1f,
					false, colour(1, 0, 0));
		}
	}
		break;

		//last scene: a large crowd of small people dancing in the fog, with the
		//camera flying slowly overhead.
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50: //yep, at 2:20am I'm starting to run out of ideas. :P
	{
		const int NUM_ROWS = 8;
		const float GRID_WIDTH = 80.0f;

		const float SRAD = 60.0f;
		float ang = g_status.songtime * PI / 64.0f;
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		SetCameraPos(
				vect(SRAD * (float) sin(ang), 9.0f, SRAD * (float) cos(ang)), 0,
				0, ang * 180 / PI);
		EnableFog(colour(1), 0.02f);
		glPushAttrib(GL_ENABLE_BIT);
		EnableSphereMapping();
		SetTexture("shine.bmp");
		for (int i = 0; i < NUM_ROWS; i++) {
			for (int j = 0; j < NUM_ROWS; j++) {
				float sway = 0.15f
						* (float) sin(
								g_status.songtime * PI / 4
										+ (((i + j) & 1) ? PI / 2 : 0));
				float bop = 0.1f
						* (float) pow(
								sin(
										g_status.songtime * PI
												+ (((i + j) & 1) ? PI / 2 : 0)),
								8);
				float dist = g_status.songtime - 360.0f;
				if (dist < 0)
					dist = -dist;
				if (dist > 1)
					dist = 1;
				sway *= dist;
				bop *= dist;
				glPushMatrix();
				glTranslatef((i - NUM_ROWS / 2.0f) * GRID_WIDTH / NUM_ROWS, 0,
						(j - NUM_ROWS / 2.0f) * GRID_WIDTH / NUM_ROWS);
				glRotatef(g_status.songtime + i * 421.0f, 0, 1, 0);
				if (g_status.pos < 45)
					RenderPerson(0, bop, 0, true);
				else
					RenderPerson(0, 0, sway, true);
				glPopMatrix();
			}
		}
		glPopAttrib();
		//draw the 'sky'... useful, this lake :)
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(0, 14, 0);
		DrawPlasmaLake();
		glEnable(GL_LIGHTING);
		glPopMatrix();
		//no more fog
		DisableFog();
		//fade out at the end
		if (g_status.pos == 49) {
			float whiteness = (g_status.songtime - 392.0f) / 8.0f;
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glColor4f(1, 1, 1, whiteness);
			glBegin(GL_QUADS);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
			glPopMatrix();
			glPopAttrib();
			glMatrixMode(GL_MODELVIEW);
		}
	}
		DrawText("there is no way to know what", 344.0f, g_status.songtime,
				-0.8f, 0.6f, 0.1f, true, colour(0.04f, 0.5f, 1.0f));
		DrawText("there is no way to know how", 348.0f, g_status.songtime,
				-0.24f, 0.2f, 0.1f, true, colour(0.04f, 0.5f, 1.0f));
		DrawText("there is no way to know when", 352.0f, g_status.songtime,
				-0.34f, 0.6f, 0.1f, true, colour(0.04f, 0.5f, 1.0f));
		DrawText("there is no way to know where", 356.0f, g_status.songtime,
				-0.8f, 0.2f, 0.1f, true, colour(0.04f, 0.5f, 1.0f));
		DrawText("goodbye", 394.0f, g_status.songtime, -0.4f, 0.0f, 0.2f, true,
				colour(0.8f));
		break;
	}

	//render the current pos/row/songtime
	RenderStatusText();
	//flip
//	iface->Flip();
}

//----------------------------------------------------------------------------//
//bootstrap functions

//HandleEvent() - handles keypress, startup, shutdown, and idle events
void HandleEvent(int event, int param1, int param2, int param3) {
//	switch (event) {
//	case EVENT_INIT :
//		Initialize();
//		break;
//	case EVENT_IDLE :
//		DoFrame();
//		break;
//	case EVENT_RESET :
//	case EVENT_SHUTDOWN :
//		Shutdown();
//		break;
//	case EVENT_KEYPRESS :
//		if (param2) {
//			if (param1 == VK_ESCAPE)
//				iface->Deactivate();
//			else if (param1 == VK_F1)
//				RenderStatusText(true);
//			else if (param1 == VK_SPACE)
//			{
//				char buffer[64];
//				sprintf(buffer, "Pos: (%02d, %02d) Time: %g\n",
//					g_status.pos, g_status.row, g_status.songtime);
//				OutputDebugString(buffer);
//			}
//			else if (param1 == VK_LEFT)
//				BASS_ChannelSetPosition(g_music,
//					(QWORD)MAKELONG(g_status.pos-1, 0));
//			else if (param1 == VK_RIGHT)
//				BASS_ChannelSetPosition(g_music,
//					(QWORD)MAKELONG(g_status.pos+1, 0));
//		}
//		break;
//	}
}

//main() - entry point
int main(int argc, char **argv) {
	//initialize our interface
//	iface = CreateInterface(WIDTH, HEIGHT, BPP, true, false);

//#ifndef WINDOWED
//	iface->SetFullscreen(true);
//#endif
//
//	iface->SetTitle("Superstars");
//	iface->SetCallback(EVENT_ALL, HandleEvent);
//
//	iface->Activate();
//
//	delete iface;

	return 0;
}
