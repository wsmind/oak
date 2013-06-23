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

#include <engine/input/InputEngine.hpp>

#include <engine/input/InputDriver.hpp>
#include <engine/input/InputListener.hpp>

#include <engine/system/Log.hpp>

#include <algorithm>

namespace oak {

InputEngine::InputEngine()
{
	this->driver = new InputDriver(this);
}

InputEngine::~InputEngine()
{
	OAK_ASSERT(this->listeners.size() == 0, "Some input listeners were not removed");
	
	delete this->driver;
}

void InputEngine::update()
{
	this->driver->update();
}

void InputEngine::addListener(InputListener *listener)
{
	this->listeners.push_back(listener);
}

void InputEngine::removeListener(InputListener *listener)
{
	ListenerVector::iterator it = std::find(this->listeners.begin(), this->listeners.end(), listener);
	OAK_ASSERT(it != this->listeners.end(), "Cannot remove input listener: it was not registered before");
	
	*it = this->listeners.back();
	this->listeners.pop_back();
}

void InputEngine::pointerAdded(unsigned int pointerId, glm::vec2 position)
{
	PointerState &pointer = this->pointers[pointerId];
	pointer.position = position;
}

void InputEngine::pointerRemoved(unsigned int pointerId)
{
	PointerMap::iterator it = this->pointers.find(pointerId);
	this->pointers.erase(it);
}

void InputEngine::pointerDown(unsigned int pointerId, unsigned int button)
{
	PointerState &pointer = this->pointers[pointerId];
	
	for (unsigned int i = 0; i < this->listeners.size(); i++)
		this->listeners[i]->pointerDown(pointerId, button, pointer.position);
}

void InputEngine::pointerUp(unsigned int pointerId, unsigned int button)
{
	PointerState &pointer = this->pointers[pointerId];
	
	for (unsigned int i = 0; i < this->listeners.size(); i++)
		this->listeners[i]->pointerUp(pointerId, button, pointer.position);
}

void InputEngine::pointerMove(unsigned int pointerId, glm::vec2 position)
{
	PointerState &pointer = this->pointers[pointerId];
	
	// compute position delta from old position
	glm::vec2 movement = position - pointer.position;
	
	// then update position
	pointer.position = position;
	
	for (unsigned int i = 0; i < this->listeners.size(); i++)
		this->listeners[i]->pointerMove(pointerId, position, movement);
}

} // oak namespace
