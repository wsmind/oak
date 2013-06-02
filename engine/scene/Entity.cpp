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

#include <engine/scene/Entity.hpp>

#include <engine/scene/Component.hpp>
#include <engine/system/Log.hpp>

#include <algorithm>

namespace oak {

Entity::Entity()
{
	Log::info("Entity created!");
}

Entity::~Entity()
{
	Log::info("Entity destroyed!");
}

void Entity::attachComponent(Component *component)
{
	this->components.push_back(component);
	component->activateComponent();
}

void Entity::detachComponent(Component *component)
{
	ComponentVector::iterator it = std::find(this->components.begin(), this->components.end(), component);
	OAK_ASSERT(it != this->components.end(), "Trying to detach an unexisting component");
	
	(*it)->deactivateComponent();
	
	// remove the scene in-place
	*it = this->components[this->components.size() - 1];
	this->components.pop_back();
}

} // oak namespace
