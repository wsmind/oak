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
#include <engine/graphics/_gl/GraphicDriverState.hpp>
#include <engine/graphics/_gl/ShaderProgram.hpp>
#include <engine/graphics/_gl/VertexBuffer.hpp>

#include <engine/system/Log.hpp>

#include <glm/glm.hpp>

#include <cstddef>

namespace oak {

namespace { // private section

const unsigned int maxInfoLogLength = 2048;

GLuint compileShader(GLenum type, const std::string &code)
{
	GLuint name = GL_CHECK(glCreateShader(type));
	
	std::string fullCode;
	
	// add a #define for GLES2
	#if defined(ANDROID) || defined(EMSCRIPTEN)
		fullCode = std::string("#define GLES2\n") + code;
	#else
		fullCode = code;
	#endif
	
	const char *codeString = fullCode.c_str();
	GL_CHECK(glShaderSource(name, 1, &codeString, NULL));
	GL_CHECK(glCompileShader(name));
	
	// check compilation status
	GLint compileStatus;
	GL_CHECK(glGetShaderiv(name, GL_COMPILE_STATUS, &compileStatus));
	if (compileStatus != GL_TRUE)
	{
		char errorLog[maxInfoLogLength];
		GLsizei length;
		GL_CHECK(glGetShaderInfoLog(name, maxInfoLogLength, &length, errorLog));
		
		switch (type)
		{
			case GL_VERTEX_SHADER: Log::error("Failed to compile vertex shader:"); break;
			case GL_FRAGMENT_SHADER: Log::error("Failed to compile fragment shader:"); break;
		}
		
		Log::error("%s", errorLog);
	}
	
	return name;
}

} // end of private section

GraphicDriver::GraphicDriver()
{
	#if !defined(ANDROID) && !defined(EMSCRIPTEN)
		glewInit();
	#endif
	
	GL_CHECK(glDisable(GL_CULL_FACE));
	GL_CHECK(glEnable(GL_DEPTH_TEST));
	
	this->state = new GraphicDriverState;
}

GraphicDriver::~GraphicDriver()
{
	delete this->state;
}

void GraphicDriver::setClearColor(const glm::vec3 &color)
{
	GL_CHECK(glClearColor(color.x, color.y, color.z, 0.0f));
}

void GraphicDriver::setClearDepth(float depth)
{
	#if defined(ANDROID) || defined(EMSCRIPTEN)
		GL_CHECK(glClearDepthf(depth));
	#else
		GL_CHECK(glClearDepth(depth));
	#endif
}

void GraphicDriver::clear(bool colorBuffer, bool depthBuffer)
{
	GLuint clearFlags = 0;
	clearFlags |= colorBuffer ? GL_COLOR_BUFFER_BIT : 0;
	clearFlags |= depthBuffer ? GL_DEPTH_BUFFER_BIT : 0;
	
	GL_CHECK(glClear(clearFlags));
}

VertexBuffer *GraphicDriver::createVertexBuffer(void *data, unsigned int size, VertexFormat format, unsigned int elementCount)
{
	OAK_ASSERT((size % elementCount) == 0, "Vertex buffer size is not aligned on a vertex boundary");
	
	VertexBuffer *buffer = new VertexBuffer;
	buffer->format = format;
	buffer->elementCount = elementCount;
	
	GL_CHECK(glGenBuffers(1, &buffer->name));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer->name));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	
	return buffer;
}

void GraphicDriver::destroyVertexBuffer(VertexBuffer *buffer)
{
	GL_CHECK(glDeleteBuffers(1, &buffer->name));
	delete buffer;
}

void GraphicDriver::bindVertexBuffer(VertexBuffer *buffer)
{
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer->name));
	
	// TODO: bind to vertex attibutes
	ShaderProgram *currentShader = this->state->currentShader;
	if (currentShader)
	{
		switch (buffer->format)
		{
			case Simple2DVertexFormat:
			{
				GLint positionAttribute = GL_CHECK(glGetAttribLocation(currentShader->programName, "position"));
				GL_CHECK(glEnableVertexAttribArray(positionAttribute));
				GL_CHECK(glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0));
				break;
			}
			
			case Standard3DVertexFormat:
			{
				GLint positionAttribute = GL_CHECK(glGetAttribLocation(currentShader->programName, "position"));
				if (positionAttribute != -1)
				{
					GL_CHECK(glEnableVertexAttribArray(positionAttribute));
					GL_CHECK(glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Standard3DVertex), (const GLvoid *)offsetof(Standard3DVertex, position)));
				}
				
				GLint normalAttribute = GL_CHECK(glGetAttribLocation(currentShader->programName, "normal"));
				if (normalAttribute != -1)
				{
					GL_CHECK(glEnableVertexAttribArray(normalAttribute));
					GL_CHECK(glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Standard3DVertex), (const GLvoid *)offsetof(Standard3DVertex, normal)));
				}
				
				GLint uvAttribute = GL_CHECK(glGetAttribLocation(currentShader->programName, "uv"));
				if (uvAttribute != -1)
				{
					GL_CHECK(glEnableVertexAttribArray(uvAttribute));
					GL_CHECK(glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Standard3DVertex), (const GLvoid *)offsetof(Standard3DVertex, uv)));
				}
				
				break;
			}
		}
	}
}

ShaderProgram *GraphicDriver::createShaderProgram(const std::string &vertexCode, const std::string &fragmentCode)
{
	ShaderProgram *program = new ShaderProgram;
	
	program->programName = GL_CHECK(glCreateProgram());
	program->vertexShaderName = compileShader(GL_VERTEX_SHADER, vertexCode);
	program->fragmentShaderName = compileShader(GL_FRAGMENT_SHADER, fragmentCode);
	
	GL_CHECK(glAttachShader(program->programName, program->vertexShaderName));
	GL_CHECK(glAttachShader(program->programName, program->fragmentShaderName));
	GL_CHECK(glLinkProgram(program->programName));
	
	// check link status
	GLint linkStatus;
	GL_CHECK(glGetProgramiv(program->programName, GL_LINK_STATUS, &linkStatus));
	if (linkStatus != GL_TRUE)
	{
		char errorLog[maxInfoLogLength];
		GLsizei length;
		GL_CHECK(glGetProgramInfoLog(program->programName, maxInfoLogLength, &length, errorLog));
		
		Log::error("Failed to link shader program:");
		Log::error("%s", errorLog);
	}
	
	return program;
}

void GraphicDriver::destroyShaderProgram(ShaderProgram *program)
{
	if (this->state->currentShader == program)
		this->state->currentShader = NULL;
	
	GL_CHECK(glDeleteShader(program->vertexShaderName));
	GL_CHECK(glDeleteShader(program->fragmentShaderName));
	GL_CHECK(glDeleteProgram(program->programName));
	delete program;
}

void GraphicDriver::bindShaderProgram(ShaderProgram *program)
{
	this->state->currentShader = program;
	GL_CHECK(glUseProgram(program->programName));
}

void GraphicDriver::setShaderConstant(const std::string &name, float value)
{
	OAK_ASSERT(this->state->currentShader != NULL, "No shader is bound, cannot set shader constant");
	
	GLint location = GL_CHECK(glGetUniformLocation(this->state->currentShader->programName, name.c_str()));
	GL_CHECK(glUniform1f(location, value));
}

void GraphicDriver::setShaderConstant(const std::string &name, const glm::vec2 &value)
{
	OAK_ASSERT(this->state->currentShader != NULL, "No shader is bound, cannot set shader constant");
	
	GLint location = GL_CHECK(glGetUniformLocation(this->state->currentShader->programName, name.c_str()));
	GL_CHECK(glUniform2f(location, value.x, value.y));
}

void GraphicDriver::setShaderConstant(const std::string &name, const glm::vec3 &value)
{
	OAK_ASSERT(this->state->currentShader != NULL, "No shader is bound, cannot set shader constant");
	
	GLint location = GL_CHECK(glGetUniformLocation(this->state->currentShader->programName, name.c_str()));
	GL_CHECK(glUniform3f(location, value.x, value.y, value.z));
}

void GraphicDriver::setShaderConstant(const std::string &name, const glm::mat3 &value)
{
	OAK_ASSERT(this->state->currentShader != NULL, "No shader is bound, cannot set shader constant");
	
	GLint location = GL_CHECK(glGetUniformLocation(this->state->currentShader->programName, name.c_str()));
	GL_CHECK(glUniformMatrix3fv(location, 1, GL_FALSE, &value[0].x));
}

void GraphicDriver::setShaderConstant(const std::string &name, const glm::mat4 &value)
{
	OAK_ASSERT(this->state->currentShader != NULL, "No shader is bound, cannot set shader constant");
	
	GLint location = GL_CHECK(glGetUniformLocation(this->state->currentShader->programName, name.c_str()));
	GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0].x));
}

void GraphicDriver::draw(PrimitiveType primitiveType, unsigned int startElement, unsigned int elementCount)
{
	GLenum glPrimitiveType = GL_TRIANGLES;
	switch (primitiveType)
	{
		case TriangleStrip: glPrimitiveType = GL_TRIANGLE_STRIP; break;
		case Triangles: glPrimitiveType = GL_TRIANGLES; break;
	}
	
	GL_CHECK(glDrawArrays(glPrimitiveType, startElement, elementCount));
}

} // oak namespace
