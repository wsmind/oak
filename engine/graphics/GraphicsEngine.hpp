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

#include <engine/sg/ComponentFactory.hpp>
#include <engine/sg/WorldListener.hpp>

#include <glm/glm.hpp>

#include <vector>

namespace oak {

class GraphicDriver;
class GraphicWorld;
class ScriptEngine;
struct ShaderProgram;
struct VertexBuffer;
class View;
class WorldManager;

class GraphicsEngine: public ComponentFactory, public WorldListener
{
	public:
		GraphicsEngine(WorldManager *worldManager);
		~GraphicsEngine();
		
		void renderFrame();
		
		glm::vec3 getBackgroundColor() const { return this->backgroundColor; }
		void setBackgroundColor(const glm::vec3 &color) { this->backgroundColor = color; }
		
		View *createView(World *world);
		void destroyView(View *view);
		
		// ComponentFactory
		virtual Component *createComponent(Entity *entity, const std::string &className);
		
		// WorldListener
		virtual void worldCreated(World *world);
		virtual void worldDestroyed(World *world);
		
	private:
		GraphicWorld *findGraphicWorld(World *world);
		
		GraphicDriver *driver;
		
		glm::vec3 backgroundColor;
		
		WorldManager *worldManager;
		
		// Generic scene graph worlds are mirrored here for performance reasons.
		// This allows for instance to build specialized spatial indexes (e.g octrees)
		// for graphics elements, and to avoid the traversal of the whole scene
		// when rendering.
		typedef std::vector<GraphicWorld *> GraphicWorldVector;
		GraphicWorldVector graphicWorlds;
		
		typedef std::vector<View *> ViewVector;
		ViewVector views;
};

} // oak namespace
