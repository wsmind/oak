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

#include <engine/graphics/GraphicWorld.hpp>

#include <engine/graphics/GraphicDriver.hpp>
#include <engine/graphics/components/Camera.hpp>

#include <engine/sg/Entity.hpp>

#include <engine/system/Log.hpp>

#include <glm/ext.hpp>

namespace oak {

GraphicWorld::GraphicWorld(World *world)
	: world(world)
{
	Log::info("New graphic world !!");
}

GraphicWorld::~GraphicWorld()
{
	Log::info("Destroyed graphic world !!");
}

void GraphicWorld::render(GraphicDriver *driver, Camera *camera)
{
	const glm::mat4 &cameraTransform = camera->getEntity()->getLocalTransform();
	glm::mat4 viewMatrix = glm::affineInverse(cameraTransform);
	
	for (unsigned int i = 0; i < this->renderables.size(); i++)
	{
		const Renderable &renderable = this->renderables[i];
		
		driver->bindShaderProgram(renderable.shader);
		driver->bindVertexBuffer(renderable.buffer);
		
		driver->setShaderConstant("modelMatrix", *renderable.transform);
		driver->setShaderConstant("viewMatrix", viewMatrix);
		driver->setShaderConstant("projectionMatrix", camera->getProjectionMatrix());
		
		if (renderable.primitiveType == TRIANGLE_STRIP)
			driver->drawTriangleStrip(renderable.startElement, renderable.elementCount);
	}
}

void GraphicWorld::registerRenderable(const Renderable &renderable)
{
	this->renderables.push_back(renderable);
}

/*void GraphicWorld::unregisterRenderable(Renderable *renderable)
{
}*/

} // oak namespace
