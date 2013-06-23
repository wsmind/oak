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

#include <engine/sg/Component.hpp>

#include <glm/glm.hpp>

namespace oak {

class GraphicDriver;
class GraphicWorld;
struct ShaderProgram;
struct VertexBuffer;

class Cube: public Component
{
	public:
		Cube(GraphicWorld *graphicWorld, GraphicDriver *driver);
		virtual ~Cube();
		
		glm::vec3 getColor() const;
		void setColor(const glm::vec3 &color);
		
		// Component
		virtual void activateComponent(Entity *entity);
		virtual void deactivateComponent(Entity *entity);
		
	private:
		GraphicDriver *driver;
		GraphicWorld *graphicWorld;
		
		// these are shared for all cubes
		static VertexBuffer *vertexBuffer;
		static ShaderProgram *shader;
		static unsigned int instanceCount;
		
		glm::vec3 color;
};

} // oak namespace
