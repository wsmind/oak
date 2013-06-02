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

#include <engine/scene/ComponentFactory.hpp>

#include <glm/glm.hpp>

namespace oak {

class GraphicDriver;
class GraphicsScene;
class SceneManager;
class ScriptEngine;
struct ShaderProgram;
struct VertexBuffer;

class GraphicsEngine: public ComponentFactory
{
	public:
		GraphicsEngine();
		~GraphicsEngine();
		
		void renderFrame();
		
		glm::vec3 getBackgroundColor() const { return this->backgroundColor; }
		void setBackgroundColor(const glm::vec3 &color) { this->backgroundColor = color; }
		
		void registerComponents(SceneManager *sceneManager);
		void unregisterComponents(SceneManager *sceneManager);
		
		// ComponentFactory
		virtual Component *createComponent(const std::string &className);
		
	private:
		GraphicDriver *driver;
		
		glm::vec3 backgroundColor;
		
		VertexBuffer *vertexBuffer;
		ShaderProgram *shader;
		
		// TODO: mirror scenes here (one graphics scene per entity scene)
		GraphicsScene *scene;
};

} // oak namespace
