#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include "engine.h"

class Buffer {
  public:

	Buffer();
	~Buffer();

	void read(GLenum format);
	void draw(GLsizei width,GLsizei height,GLint x=0,GLint y=0) const;

	void setReadDims(GLint x,GLint y,GLint width,GLint height);

	private:

	static GLint m_max_viewport_dims,m_reference_count;
	static GLubyte *m_viewport_buffer;

	GLint m_x,m_y,m_width,m_height;
};

#endif // BUFFER_H_INCLUDED