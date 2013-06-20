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

#include <engine/sg/Entity.hpp>

#include <engine/sg/Component.hpp>
#include <engine/sg/ComponentFactory.hpp>
#include <engine/system/Log.hpp>

#include <algorithm>

namespace oak {

Entity::FactoryMap Entity::factories;

void Entity::registerComponentFactory(const std::string &className, ComponentFactory *factory)
{
	OAK_ASSERT(Entity::factories.find(className) == Entity::factories.end(), "Component factory already registered for class '%s'", className.c_str());
	Entity::factories[className] = factory;
	
	Log::info("Registered component '%s'", className.c_str());
}

void Entity::unregisterComponentFactory(const std::string &className)
{
	OAK_ASSERT(Entity::factories.find(className) != Entity::factories.end(), "Component factory never registered for class '%s'", className.c_str());
	
	FactoryMap::iterator it = Entity::factories.find(className);
	Entity::factories.erase(it);
	
	Log::info("Unregistered component '%s'", className.c_str());
}

Entity::Entity(Scene *scene)
	: scene(scene)
	, localPosition(0.0f, 0.0f, 0.0f)
	, localOrientation(1.0f, 0.0f, 0.0f, 0.0f)
	, localScale(1.0f, 1.0f, 1.0f)
{
	Log::info("Entity created!");
	
	this->updateLocalTransform();
}

Entity::~Entity()
{
	// destroy all attached components
	for (unsigned int i = 0; i < this->components.size(); i++)
	{
		this->components[i]->detachComponent(this);
		delete this->components[i];
	}
	
	Log::info("Entity destroyed!");
}

Component *Entity::createComponent(const std::string &className)
{
	FactoryMap::iterator it = this->factories.find(className);
	
	if (it == this->factories.end())
	{
		Log::error("No component factory registered for component type '%s'", className.c_str());
		return NULL;
	}
	
	ComponentFactory *factory = it->second;
	Component *component = factory->createComponent(this, className);
	OAK_ASSERT(component != NULL, "Component factory could not create a type it was registered for");
	
	this->components.push_back(component);
	component->attachComponent(this);
	component->activateComponent();
	
	return component;
}

void Entity::destroyComponent(Component *component)
{
	ComponentVector::iterator it = std::find(this->components.begin(), this->components.end(), component);
	OAK_ASSERT(it != this->components.end(), "Trying to detach an unexisting component");
	
	(*it)->deactivateComponent();
	(*it)->detachComponent(this);
	
	// remove the component in-place
	*it = this->components[this->components.size() - 1];
	this->components.pop_back();
}

void Entity::updateLocalTransform()
{
	glm::mat4 translation = glm::translate(this->localPosition.x, this->localPosition.y, this->localPosition.z);
	glm::mat4 rotation = glm::toMat4(this->localOrientation);
	glm::mat4 scale = glm::scale(this->localScale.x, this->localScale.y, this->localScale.z);
	
	this->localTransform = translation * rotation * scale;
}

} // oak namespace
