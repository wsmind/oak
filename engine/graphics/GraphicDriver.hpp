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

#pragma once

#include <string>

#include <glm/glm.hpp>

namespace oak {

struct VertexBuffer;
struct ShaderProgram;

class GraphicDriver
{
	public:
		GraphicDriver();
		
		void setClearColor(const glm::vec3 &color);
		void setClearDepth(float depth);
		void clear(bool colorBuffer, bool depthBuffer);
		
		enum VertexFormat
		{
			Position2D
		};
		VertexBuffer *createVertexBuffer(VertexFormat format, unsigned int size);
		void destroyVertexBuffer(VertexBuffer *buffer);
		void fillVertexBuffer(VertexBuffer *buffer, void *data, unsigned int size);
		void bindVertexBuffer(VertexBuffer *buffer);
		
		ShaderProgram *createShaderProgram(const std::string &vertexCode, const std::string &fragmentCode);
		void destroyShaderProgram(ShaderProgram *program);
		void bindShaderProgram(ShaderProgram *program);
};

} // oak namespace
