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

#include <engine/app/Application.hpp>

#include <glm/glm.hpp>

#include <engine/graphics/GraphicsEngine.hpp>
#include <engine/scene/SceneManager.hpp>
#include <engine/script/ScriptEngine.hpp>
#include <engine/system/Log.hpp>

namespace oak {

void Application::initialize(const std::string &baseFolder)
{
	Log::info("Application::initialize");
	
	this->sceneManager = new SceneManager;
	
	this->graphics = new GraphicsEngine;
	this->graphics->registerComponents(this->sceneManager);
	
	this->script = new ScriptEngine;
	this->script->initialize();
	
	// script bindings
	this->script->registerScene(this->sceneManager);
	this->script->registerGraphics(this->graphics);
	
	this->script->loadFile(baseFolder + "/main.lua");
	
	this->script->startCall("initialize");
	this->script->endCall();
}

void Application::shutdown()
{
	Log::info("Application::shutdown");
	
	this->script->startCall("shutdown");
	this->script->endCall();
	
	this->script->shutdown();
	delete this->script;
	
	this->graphics->unregisterComponents(this->sceneManager);
	delete this->graphics;
	
	delete this->sceneManager;
}

void Application::update(float dt)
{
	this->script->startCall("update");
	this->script->appendParameter((double)dt);
	this->script->endCall();
	
	this->graphics->renderFrame();
}

} // oak namespace
