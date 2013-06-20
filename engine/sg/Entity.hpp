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

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <map>
#include <string>

namespace oak {

class Component;
class ComponentFactory;
class Scene;

class Entity
{
	public:
		static void registerComponentFactory(const std::string &className, ComponentFactory *factory);
		static void unregisterComponentFactory(const std::string &className);
		
		Entity(Scene *scene);
		~Entity();
		
		Scene *getScene() const { return this->scene; }
		
		Component *createComponent(const std::string &className);
		void destroyComponent(Component *component);
		
		const glm::vec3 &getLocalPosition() const { return this->localPosition; }
		const glm::quat &getLocalOrientation() const { return this->localOrientation; }
		const glm::vec3 &getLocalScale() const { return this->localScale; }
		
		void setLocalPosition(const glm::vec3 &localPosition) { this->localPosition = localPosition; this->updateLocalTransform(); }
		void setLocalOrientation(const glm::quat &localOrientation) { this->localOrientation = localOrientation; this->updateLocalTransform(); }
		void setLocalScale(const glm::vec3 &localScale) { this->localScale = localScale; this->updateLocalTransform(); }
		
		const glm::mat4 &getLocalTransform() const { return this->localTransform; }
		//const glm::mat4 &getWorldTransform() const { return this->worldTransform; }
		
	private:
		// compute local transform from separate position, orientation and scale
		void updateLocalTransform();
		
		typedef std::map<std::string, ComponentFactory *> FactoryMap;
		static FactoryMap factories;
		
		Scene *scene;
		
		typedef std::vector<Component *> ComponentVector;
		ComponentVector components;
		
		// local transform
		glm::vec3 localPosition;
		glm::quat localOrientation;
		glm::vec3 localScale;
		glm::mat4 localTransform;
		
		// world transform (hierarchy applied)
		//glm::mat4 worldTransform;
};

} // oak namespace
