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
#include <engine/graphics/GraphicWorld.hpp>
#include <engine/graphics/components/DemoQuad.hpp>

#include <engine/sg/Entity.hpp>
#include <engine/sg/Scene.hpp>
#include <engine/sg/World.hpp>
#include <engine/sg/WorldManager.hpp>

#include <engine/system/Log.hpp>

namespace oak {

GraphicsEngine::GraphicsEngine(WorldManager *worldManager)
{
	this->driver = new GraphicDriver;
	
	// default color
	this->backgroundColor = glm::vec3(0.4f, 0.6f, 0.7f);
	
	this->worldManager = worldManager;
	this->worldManager->addWorldListener(this);
	
	Entity::registerComponentFactory("DemoQuad", this);
}

GraphicsEngine::~GraphicsEngine()
{
	OAK_ASSERT(this->graphicWorlds.size() == 0, "Some graphic worlds were not destroyed properly");
	
	Entity::unregisterComponentFactory("DemoQuad");
	
	this->worldManager->removeWorldListener(this);
	
	delete this->driver;
}

void GraphicsEngine::renderFrame()
{
	this->driver->setClearColor(this->backgroundColor);
	this->driver->setClearDepth(1.0f);
	this->driver->clear(true, true);
	
	// TODO: add viewports to choose how to render each world
	// currently: render everything
	for (unsigned int i = 0; i < this->graphicWorlds.size(); i++)
	{
		this->graphicWorlds[i]->render(this->driver);
	}
}

Component *GraphicsEngine::createComponent(Entity *entity, const std::string &className)
{
	// find the world in which the entity lives
	World *world = entity->getScene()->getWorld();
	
	// find the associated graphic world
	GraphicWorld *graphicWorld = this->findGraphicWorld(world);
	OAK_ASSERT(graphicWorld != NULL, "Creating a graphic component from an unregistered world");
	
	if (className == "DemoQuad") return new DemoQuad(graphicWorld, this->driver);
	
	return NULL;
}

void GraphicsEngine::worldCreated(World *world)
{
	GraphicWorld *graphicWorld = new GraphicWorld(world);
	this->graphicWorlds.push_back(graphicWorld);
}

void GraphicsEngine::worldDestroyed(World *world)
{
	// find the graphics world bound to the given sg world
	for (unsigned int i = 0; i < this->graphicWorlds.size(); i++)
	{
		if (this->graphicWorlds[i]->getWorld() == world)
		{
			// destroy the graphic world
			delete this->graphicWorlds[i];
			
			// then remove it in-place
			this->graphicWorlds[i] = this->graphicWorlds[this->graphicWorlds.size() - 1];
			this->graphicWorlds.pop_back();
			
			return;
		}
	}
	
	OAK_ASSERT(false, "Unregistering a world that was never registered");
}

GraphicWorld *GraphicsEngine::findGraphicWorld(World *world)
{
	for (unsigned int i = 0; i < this->graphicWorlds.size(); i++)
	{
		if (this->graphicWorlds[i]->getWorld() == world)
		{
			return this->graphicWorlds[i];
		}
	}
	
	return NULL;
}

} // oak namespace
