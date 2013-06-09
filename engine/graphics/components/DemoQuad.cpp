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

#include <engine/graphics/components/DemoQuad.hpp>

#include <engine/graphics/GraphicDriver.hpp>
#include <engine/graphics/GraphicWorld.hpp>

#include <engine/graphics/shaders/demo.vs.h>
#include <engine/graphics/shaders/demo.fs.h>

#include <engine/system/Time.hpp>

namespace oak {

DemoQuad::DemoQuad(GraphicWorld *graphicWorld, GraphicDriver *driver)
{
	this->driver = driver;
	this->graphicWorld = graphicWorld;
	
	// test buffer
	GraphicDriver::Simple2DVertex vertices[] = {
		{ glm::vec2(-1.0f, -1.0f) },
		{ glm::vec2(-1.0f, 1.0f) },
		{ glm::vec2(1.0f, -1.0f) },
		{ glm::vec2(1.0f, 1.0f) }
	};
	this->vertexBuffer = this->driver->createVertexBuffer(vertices, 4);
	
	// test shader
	this->shader = this->driver->createShaderProgram(demoVSString, demoFSString);
	
	this->setColor(glm::vec3(0.0f, 0.0f, 0.0f));
}

DemoQuad::~DemoQuad()
{
	this->driver->destroyVertexBuffer(this->vertexBuffer);
	this->driver->destroyShaderProgram(this->shader);
}

glm::vec3 DemoQuad::getColor() const
{
	return this->color;
}

void DemoQuad::setColor(const glm::vec3 &color)
{
	this->color = color;
	this->driver->bindShaderProgram(this->shader);
	
	float time = (float)(Time::readNanoseconds() % 100000000000LL) / 1000000000.0f;
	this->driver->setShaderConstant("time", time);
	this->driver->setShaderConstant("color", color);
}

void DemoQuad::activateComponent()
{
	GraphicWorld::Renderable renderable;
	renderable.buffer = this->vertexBuffer;
	renderable.shader = this->shader;
	renderable.primitiveType = GraphicWorld::TRIANGLE_STRIP;
	renderable.startElement = 0;
	renderable.elementCount = 4;
	
	this->graphicWorld->registerRenderable(renderable);
}

void DemoQuad::deactivateComponent()
{
	//this->graphicWorld->unregisterXXX();
}

} // oak namespace
