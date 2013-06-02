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

#include <vector>
#include <string>
#include <map>

namespace oak {

class Component;
class ComponentFactory;
class Entity;
class Scene;

class SceneManager
{
	public:
		void registerComponentFactory(const std::string &className, ComponentFactory *factory);
		void unregisterComponentFactory(const std::string &className);
		
		Scene *createScene();
		void destroyScene(Scene *scene);
		
		Entity *createEntity(Scene *scene);
		void destroyEntity(Scene *scene, Entity *entity);
		
		Component *createComponent(Entity *entity, const std::string &className);
		void destroyComponent(Entity *entity, Component *component);
		
	private:
		typedef std::map<std::string, ComponentFactory *> FactoryMap;
		FactoryMap factories;
		
		typedef std::vector<Scene *> SceneVector;
		SceneVector scenes;
};

} // oak namespace
