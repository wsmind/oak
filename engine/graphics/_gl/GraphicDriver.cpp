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

namespace oak {

namespace { // private section

const unsigned int maxInfoLogLength = 2048;

GLuint compileShader(GLenum type, const std::string &code)
{
	GLuint name = glCreateShader(type);
	
	const char *codeString = code.c_str();
	glShaderSource(name, 1, &codeString, NULL);
	glCompileShader(name);
	
	// check compilation status
	GLint compileStatus;
	glGetShaderiv(name, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		char errorLog[maxInfoLogLength];
		GLsizei length;
		glGetShaderInfoLog(name, maxInfoLogLength, &length, errorLog);
		
		switch (type)
		{
			case GL_VERTEX_SHADER: Log::error("Failed to compile vertex shader:\n"); break;
			case GL_FRAGMENT_SHADER: Log::error("Failed to compile fragment shader:\n"); break;
		}
		
		Log::error(std::string(errorLog));
		Log::error("\n");
	}
	
	return name;
}

} // end of private section

GraphicDriver::GraphicDriver()
{
	glewInit();
	
	this->state = new GraphicDriverState;
}

GraphicDriver::~GraphicDriver()
{
	delete this->state;
}

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
	glBindBuffer(GL_ARRAY_BUFFER, buffer->name);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void GraphicDriver::bindVertexBuffer(VertexBuffer *buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer->name);
	
	// TODO: bind to vertex attibutes
	ShaderProgram *currentShader = this->state->currentShader;
	if (currentShader)
	{
		switch (buffer->format)
		{
			case Position2D:
			{
				GLint positionAttribute = glGetAttribLocation(currentShader->programName, "position");
				glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
				break;
			}
		}
	}
}

ShaderProgram *GraphicDriver::createShaderProgram(const std::string &vertexCode, const std::string &fragmentCode)
{
	ShaderProgram *program = new ShaderProgram;
	
	program->programName = glCreateProgram();
	program->vertexShaderName = compileShader(GL_VERTEX_SHADER, vertexCode);
	program->fragmentShaderName = compileShader(GL_FRAGMENT_SHADER, fragmentCode);
	
	glAttachShader(program->programName, program->vertexShaderName);
	glAttachShader(program->programName, program->fragmentShaderName);
	glLinkProgram(program->programName);
	
	// check link status
	GLint linkStatus;
	glGetProgramiv(program->programName, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		char errorLog[maxInfoLogLength];
		GLsizei length;
		glGetProgramInfoLog(program->programName, maxInfoLogLength, &length, errorLog);
		
		Log::error("Failed to link shader program:\n");
		Log::error(std::string(errorLog));
		Log::error("\n");
	}
	
	return program;
}

void GraphicDriver::destroyShaderProgram(ShaderProgram *program)
{
	if (this->state->currentShader == program)
		this->state->currentShader = NULL;
	
	glDeleteShader(program->vertexShaderName);
	glDeleteShader(program->fragmentShaderName);
	glDeleteProgram(program->programName);
	delete program;
}

void GraphicDriver::bindShaderProgram(ShaderProgram *program)
{
	this->state->currentShader = program;
	glUseProgram(program->programName);
}

} // oak namespace
