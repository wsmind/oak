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
#include <engine/graphics/GraphicsScene.hpp>

#include <engine/graphics/shaders/test.vs.h>
#include <engine/graphics/shaders/test.fs.h>

namespace oak {

DemoQuad::DemoQuad(GraphicsScene *scene, GraphicDriver *driver)
{
	this->driver = driver;
	this->scene = scene;
	
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

DemoQuad::~DemoQuad()
{
	this->driver->destroyVertexBuffer(this->vertexBuffer);
	this->driver->destroyShaderProgram(this->shader);
}

void DemoQuad::activateComponent()
{
	GraphicsScene::Renderable renderable;
	renderable.buffer = this->vertexBuffer;
	renderable.shader = this->shader;
	renderable.primitiveType = GraphicsScene::TRIANGLE_STRIP;
	renderable.startElement = 0;
	renderable.elementCount = 4;
	
	this->scene->registerRenderable(renderable);
}

void DemoQuad::deactivateComponent()
{
	//this->scene->unregisterXXX();
}

} // oak namespace
