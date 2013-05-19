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

#include <engine/graphics/GraphicsEngine.hpp>
#include <engine/graphics/GraphicDriver.hpp>

#include <engine/graphics/shaders/test.vs.h>
#include <engine/graphics/shaders/test.fs.h>

namespace oak {

GraphicsEngine::GraphicsEngine()
{
	this->driver = new GraphicDriver;
	
	// default color
	this->backgroundColor = glm::vec3(0.4f, 0.6f, 0.7f);
	
	// test buffer
	GraphicDriver::Simple2DVertex vertices[] = {
		{ glm::vec2(-1.0f, -1.0f) },
		{ glm::vec2(-1.0f, 1.0f) },
		{ glm::vec2(1.0f, -1.0f) },
		{ glm::vec2(1.0f, 1.0f) }
	};
	this->vertexBuffer = this->driver->createVertexBuffer(vertices, 4);
	
	// test shader
	this->shader = this->driver->createShaderProgram(testVSString, testFSString);
}

GraphicsEngine::~GraphicsEngine()
{
	this->driver->destroyVertexBuffer(this->vertexBuffer);
	this->driver->destroyShaderProgram(this->shader);
	
	delete this->driver;
}

void GraphicsEngine::renderFrame()
{
	this->driver->setClearColor(this->backgroundColor);
	this->driver->setClearDepth(1.0f);
	this->driver->clear(true, true);
	
	/*this->driver->bindShaderProgram(this->shader);
	this->driver->bindVertexBuffer(this->vertexBuffer);
	this->driver->drawTriangleStrip(0, 4);*/
}

} // oak namespace
