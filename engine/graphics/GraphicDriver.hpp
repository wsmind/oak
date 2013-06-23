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

struct GraphicDriverState;
struct VertexBuffer;
struct ShaderProgram;

class GraphicDriver
{
	public:
		GraphicDriver();
		~GraphicDriver();
		
		void setClearColor(const glm::vec3 &color);
		void setClearDepth(float depth);
		void clear(bool colorBuffer, bool depthBuffer);
		
		// pack vertex structure data
		#pragma pack(push)
		#pragma pack(1)
		
		struct Simple2DVertex
		{
			glm::vec2 position;
		};
		
		struct Standard3DVertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};
		
		// end of vertex structures
		#pragma pack(pop)
		
		enum VertexFormat
		{
			Simple2DVertexFormat,
			Standard3DVertexFormat
		};
		VertexBuffer *createVertexBuffer(void *data, unsigned int size, VertexFormat format, unsigned int elementCount);
		void destroyVertexBuffer(VertexBuffer *buffer);
		void bindVertexBuffer(VertexBuffer *buffer);
		
		// vertex buffer creation helpers
		inline VertexBuffer *createVertexBuffer(Simple2DVertex *vertices, unsigned int elementCount);
		inline VertexBuffer *createVertexBuffer(Standard3DVertex *vertices, unsigned int elementCount);
		
		ShaderProgram *createShaderProgram(const std::string &vertexCode, const std::string &fragmentCode);
		void destroyShaderProgram(ShaderProgram *program);
		void bindShaderProgram(ShaderProgram *program);
		
		void setShaderConstant(const std::string &name, float value);
		void setShaderConstant(const std::string &name, const glm::vec2 &value);
		void setShaderConstant(const std::string &name, const glm::vec3 &value);
		void setShaderConstant(const std::string &name, const glm::mat3 &value);
		void setShaderConstant(const std::string &name, const glm::mat4 &value);
		
		void drawTriangleStrip(unsigned int startElement, unsigned int elementCount);
		void drawTriangles(unsigned int startElement, unsigned int elementCount);
		
	private:
		GraphicDriverState *state;
};

} // oak namespace

#include <engine/graphics/GraphicDriver.inline.hpp>
