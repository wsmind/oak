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

#include <engine/scene/SceneManager.hpp>

#include <engine/scene/Component.hpp>
#include <engine/scene/ComponentFactory.hpp>
#include <engine/scene/Entity.hpp>
#include <engine/scene/Scene.hpp>
#include <engine/system/Log.hpp>

#include <algorithm>

namespace oak {

void SceneManager::registerComponentFactory(const std::string &className, ComponentFactory *factory)
{
	OAK_ASSERT(this->factories.find(className) == this->factories.end(), "Component factory already registered for class '%s'", className.c_str());
	this->factories[className] = factory;
	
	Log::info("Registered component '%s'", className.c_str());
}

void SceneManager::unregisterComponentFactory(const std::string &className)
{
	OAK_ASSERT(this->factories.find(className) != this->factories.end(), "Component factory never registered for class '%s'", className.c_str());
	
	FactoryMap::iterator it = this->factories.find(className);
	this->factories.erase(it);
	
	Log::info("Unregistered component '%s'", className.c_str());
}

Scene *SceneManager::createScene()
{
	Scene *scene = new Scene;
	this->scenes.push_back(scene);
	
	return scene;
}

void SceneManager::destroyScene(Scene *scene)
{
	SceneVector::iterator it = std::find(this->scenes.begin(), this->scenes.end(), scene);
	OAK_ASSERT(it != this->scenes.end(), "Trying to destroy an unexisting scene");
	
	// remove the scene in-place
	*it = this->scenes[this->scenes.size() - 1];
	this->scenes.pop_back();
	
	delete scene;
}

Entity *SceneManager::createEntity(Scene *scene)
{
	return scene->createEntity();
}

void SceneManager::destroyEntity(Scene *scene, Entity *entity)
{
	scene->destroyEntity(entity);
}

Component *SceneManager::createComponent(Entity *entity, const std::string &className)
{
	FactoryMap::iterator it = this->factories.find(className);
	
	if (it == this->factories.end())
	{
		Log::error("No component factory registered for component type '%s'", className.c_str());
		return NULL;
	}
	
	ComponentFactory *factory = it->second;
	Component *component = factory->createComponent(className);
	OAK_ASSERT(component != NULL, "Component factory could not create a type it was registered for");
	
	entity->attachComponent(component);
	
	return component;
}

void SceneManager::destroyComponent(Entity *entity, Component *component)
{
	entity->detachComponent(component);
	
	delete component;
}

} // oak namespace
