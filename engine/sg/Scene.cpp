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

#include <engine/sg/Scene.hpp>

#include <engine/sg/Entity.hpp>
#include <engine/system/Log.hpp>

#include <algorithm>

namespace oak {

Scene::Scene(World *world)
	: world(world)
{
	Log::info("Scene created!");
}

Scene::~Scene()
{
	for (unsigned int i = 0; i < this->entities.size(); i++)
	{
		delete this->entities[i];
	}
	
	Log::info("Scene destroyed!");
}

Entity *Scene::createEntity()
{
	Entity *entity = new Entity(this);
	this->entities.push_back(entity);
	
	return entity;
}

void Scene::destroyEntity(Entity *entity)
{
	EntityVector::iterator it = std::find(this->entities.begin(), this->entities.end(), entity);
	OAK_ASSERT(it != this->entities.end(), "Trying to destroy an unexisting entity");
	
	// remove the entity in-place
	*it = this->entities[this->entities.size() - 1];
	this->entities.pop_back();
	
	delete entity;
}

} // oak namespace
