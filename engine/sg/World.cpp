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

#include <engine/sg/World.hpp>

#include <engine/sg/Scene.hpp>
#include <engine/system/Log.hpp>

#include <algorithm>

namespace oak {

World::World()
{
	Log::info("World created!");
}

World::~World()
{
	for (unsigned int i = 0; i < this->scenes.size(); i++)
	{
		delete this->scenes[i];
	}
	
	Log::info("World destroyed!");
}

Scene *World::createScene()
{
	Scene *scene = new Scene(this);
	this->scenes.push_back(scene);
	
	return scene;
}

void World::destroyScene(Scene *scene)
{
	SceneVector::iterator it = std::find(this->scenes.begin(), this->scenes.end(), scene);
	OAK_ASSERT(it != this->scenes.end(), "Trying to destroy an unexisting scene");
	
	// remove the scene in-place
	*it = this->scenes.back();
	this->scenes.pop_back();
	
	delete scene;
}

} // oak namespace
