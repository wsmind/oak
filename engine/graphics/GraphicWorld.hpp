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

#include <engine/graphics/GraphicDriver.hpp>

#include <glm/glm.hpp>

#include <vector>

namespace oak {

class Camera;
class GraphicDriver;
class World;

class GraphicWorld
{
	public:
		GraphicWorld(World *world);
		~GraphicWorld();
		
		World *getWorld() const { return this->world; }
		
		void render(GraphicDriver *driver, Camera *camera);
		
		enum PrimitiveType
		{
			TRIANGLE_STRIP
		};
		
		struct Renderable
		{
			const glm::mat4 *transform;
			VertexBuffer *buffer;
			ShaderProgram *shader;
			PrimitiveType primitiveType;
			unsigned int startElement;
			unsigned int elementCount;
			
			Renderable()
				: transform(NULL)
				, buffer(NULL)
				, shader(NULL)
				, primitiveType(TRIANGLE_STRIP)
				, startElement(0)
				, elementCount(0)
			{}
		};
		
		void registerRenderable(const Renderable &renderable);
		//void unregisterRenderable(const Component *owner);
		
	private:
		// the generic world this graphic world is bound to
		World *world;
		
		typedef std::vector<Renderable> RenderableVector;
		RenderableVector renderables;
};

} // oak namespace
