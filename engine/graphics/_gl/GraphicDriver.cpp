/******************************************************************************
 *
 * Oak game engine
 * Copyright (c) 2013 Remi Papillie
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 *****************************************************************************/

#include <engine/graphics/GraphicDriver.hpp>

#include <engine/graphics/_gl/gl_includes.hpp>
#include <engine/graphics/_gl/VertexBuffer.hpp>

#include <glm/glm.hpp>

namespace oak {

void GraphicDriver::setClearColor(const glm::vec3 &color)
{
	glClearColor(color.x, color.y, color.z, 0.0f);
}

void GraphicDriver::setClearDepth(float depth)
{
	#ifdef ANDROID
		glClearDepthf(depth);
	#else
		glClearDepth(depth);
	#endif
}

void GraphicDriver::clear(bool colorBuffer, bool depthBuffer)
{
	GLuint clearFlags = 0;
	clearFlags |= colorBuffer ? GL_COLOR_BUFFER_BIT : 0;
	clearFlags |= depthBuffer ? GL_DEPTH_BUFFER_BIT : 0;
	
	glClear(clearFlags);
}

VertexBuffer *GraphicDriver::createVertexBuffer(VertexFormat format, unsigned int size)
{
	VertexBuffer *buffer = new VertexBuffer;
	buffer->format = format;
	
	glGenBuffers(1, &buffer->name);
	
	return buffer;
}

void GraphicDriver::destroyVertexBuffer(VertexBuffer *buffer)
{
	glDeleteBuffers(1, &buffer->name);
	delete buffer;
}

void GraphicDriver::fillVertexBuffer(VertexBuffer *buffer, void *data, unsigned int size)
{
	glBindBuffer(Gl_ARRAY_BUFFER, buffer->name);
	glBufferData(Gl_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void GraphicDriver::bindVertexBuffer(VertexBuffer *buffer)
{
	glBindBuffer(Gl_ARRAY_BUFFER, buffer->name);
	
	// TODO: bind to vertex attibutes
}

} // oak namespace
