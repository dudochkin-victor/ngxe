#include "buffer.h"

GLint Buffer::m_max_viewport_dims;
GLint Buffer::m_reference_count;
GLubyte *Buffer::m_viewport_buffer;

Buffer::Buffer() :
		m_x(0), m_y(0), m_width(0), m_height(0) {
	if (!m_max_viewport_dims)
		glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &m_max_viewport_dims);

	// Allocate the maximum amount of memory needed for a viewport.
	if (!(m_reference_count++))
		m_viewport_buffer = new GLubyte[m_max_viewport_dims
				* m_max_viewport_dims];
}

Buffer::~Buffer() {
	if (!(--m_reference_count))
		delete[] m_viewport_buffer;
}

void Buffer::read(GLenum format) {
	// Adjust the row length when packing pixel data to memory.
	GLint row_length;
	glGetIntegerv(GL_PACK_ROW_LENGTH, &row_length);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_max_viewport_dims);

	// Read all pixels from the given buffer to the viewport buffer.
	glReadPixels(m_x, m_y, m_width, m_height, format, GL_UNSIGNED_BYTE,
			m_viewport_buffer);
	ASSERT_GL_ERROR

	// Restore the row length.
	glPixelStorei(GL_PACK_ROW_LENGTH, row_length);
}

void Buffer::draw(GLsizei width, GLsizei height, GLint x, GLint y) const {
	// Adjust the row length when unpacking pixel data from memory.
	GLint row_length;
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &row_length);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, m_max_viewport_dims);

	// Set the proper pixel zoom.
	GLfloat pixel_zoom[2];
	glGetFloatv(GL_ZOOM_X, &pixel_zoom[0]);
	glGetFloatv(GL_ZOOM_Y, &pixel_zoom[1]);
	glPixelZoom((GLfloat) width / m_width, (GLfloat) height / m_height);

	// Apply the desired raster position.
	GLdouble raster_pos[4];
	glGetDoublev(GL_CURRENT_RASTER_POSITION, raster_pos);
	SetWindowRasterPos(x, y);

	// Draw the occlusion map (OpenGL draws bottom to top).
	glDrawPixels(m_width, m_height, GL_LUMINANCE, GL_UNSIGNED_BYTE,
			m_viewport_buffer);
	ASSERT_GL_ERROR

	// Restore the raster position.
	glRasterPos4dv(raster_pos);

	// Restore the pixel zoom.
	glPixelZoom(pixel_zoom[0], pixel_zoom[1]);

	// Restore the row length.
	glPixelStorei(GL_UNPACK_ROW_LENGTH, row_length);
}

void Buffer::setReadDims(GLint x, GLint y, GLint width, GLint height) {
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
}
