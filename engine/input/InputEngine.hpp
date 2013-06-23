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

#include <engine/input/InputDriverListener.hpp>

#include <vector>
#include <map>

namespace oak {

class InputDriver;
class InputListener;

class InputEngine: public InputDriverListener
{
	public:
		InputEngine();
		~InputEngine();
		
		void update();
		
		void addListener(InputListener *listener);
		void removeListener(InputListener *listener);
		
		// InputDriverListener
		virtual void pointerAdded(unsigned int pointerId, glm::vec2 position);
		virtual void pointerRemoved(unsigned int pointerId);
		
		virtual void pointerDown(unsigned int pointerId, unsigned int button);
		virtual void pointerUp(unsigned int pointerId, unsigned int button);
		virtual void pointerMove(unsigned int pointerId, glm::vec2 position);
	
	private:
		InputDriver *driver;
		
		typedef std::vector<InputListener *> ListenerVector;
		ListenerVector listeners;
		
		struct PointerState
		{
			glm::vec2 position;
		};
		
		typedef std::map<unsigned int, PointerState> PointerMap;
		PointerMap pointers;
};

} // oak namespace
