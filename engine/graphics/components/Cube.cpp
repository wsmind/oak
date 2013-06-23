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

#include <engine/graphics/components/Cube.hpp>

#include <engine/graphics/GraphicDriver.hpp>
#include <engine/graphics/GraphicWorld.hpp>

#include <engine/graphics/shaders/cube.vs.h>
#include <engine/graphics/shaders/cube.fs.h>

#include <engine/sg/Entity.hpp>

#include <engine/system/Time.hpp>

namespace oak {

Cube::Cube(GraphicWorld *graphicWorld, GraphicDriver *driver)
{
	this->driver = driver;
	this->graphicWorld = graphicWorld;
	
	// test buffer
	GraphicDriver::Standard3DVertex vertices[] = {
		// -X
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		
		// +X
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		
		// -Y
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		
		// +Y
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		
		// -Z
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		
		// +Z
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
	};
	this->vertexBuffer = this->driver->createVertexBuffer(vertices, 36);
	
	// test shader
	this->shader = this->driver->createShaderProgram(cubeVSString, cubeFSString);
	
	this->setColor(glm::vec3(0.0f, 0.0f, 0.0f));
}

Cube::~Cube()
{
	this->driver->destroyVertexBuffer(this->vertexBuffer);
	this->driver->destroyShaderProgram(this->shader);
}

glm::vec3 Cube::getColor() const
{
	return this->color;
}

void Cube::setColor(const glm::vec3 &color)
{
	this->color = color;
	this->driver->bindShaderProgram(this->shader);
	
	this->driver->setShaderConstant("time", (float)Time::getTime());
	this->driver->setShaderConstant("color", color);
}

void Cube::activateComponent(Entity *entity)
{
	GraphicWorld::Renderable renderable;
	renderable.transform = &entity->getLocalTransform();
	renderable.buffer = this->vertexBuffer;
	renderable.shader = this->shader;
	renderable.primitiveType = GraphicWorld::TRIANGLES;
	renderable.startElement = 0;
	renderable.elementCount = 36;
	
	this->graphicWorld->registerRenderable(renderable);
}

void Cube::deactivateComponent(Entity *entity)
{
	//this->graphicWorld->unregisterXXX();
}

} // oak namespace
