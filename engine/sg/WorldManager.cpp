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

#include <engine/sg/WorldManager.hpp>

#include <engine/sg/World.hpp>
#include <engine/sg/WorldListener.hpp>
#include <engine/system/Log.hpp>

#include <algorithm>

namespace oak {

World *WorldManager::createWorld()
{
	World *world = new World;
	this->worlds.push_back(world);
	
	// notify listeners
	for (unsigned int i = 0; i < this->listeners.size(); i++)
	{
		this->listeners[i]->worldCreated(world);
	}
	
	return world;
}

void WorldManager::destroyWorld(World *world)
{
	WorldVector::iterator it = std::find(this->worlds.begin(), this->worlds.end(), world);
	OAK_ASSERT(it != this->worlds.end(), "Trying to destroy an unexisting world");
	
	// notify listeners
	for (unsigned int i = 0; i < this->listeners.size(); i++)
	{
		this->listeners[i]->worldDestroyed(world);
	}
	
	// remove the world in-place
	*it = this->worlds.back();
	this->worlds.pop_back();
	
	delete world;
}

void WorldManager::addWorldListener(WorldListener *listener)
{
	// check that the listener is not already registered
	WorldListenerVector::iterator it = std::find(this->listeners.begin(), this->listeners.end(), listener);
	OAK_ASSERT(it == this->listeners.end(), "WorldListener already registered");
	
	this->listeners.push_back(listener);
}

void WorldManager::removeWorldListener(WorldListener *listener)
{
	// check that the listener was previously registered
	WorldListenerVector::iterator it = std::find(this->listeners.begin(), this->listeners.end(), listener);
	OAK_ASSERT(it != this->listeners.end(), "WorldListener never registered");
	
	// remove in-place
	*it = this->listeners.back();
	this->listeners.pop_back();
}

} // oak namespace
