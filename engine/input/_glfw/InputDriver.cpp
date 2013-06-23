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

#include <engine/input/InputDriver.hpp>

#include <engine/input/InputDriverListener.hpp>
#include <engine/input/_glfw/InputDriverState.hpp>

#include <engine/system/Log.hpp>

#include <GL/glfw.h>

namespace oak {

namespace { // private section

// glfw callbacks do not provide a client data,
// so we sadly need a global to access the driver state
InputDriverState *driverState = NULL;

void GLFWCALL mouseButtonCallback(int button, int state)
{
	OAK_ASSERT(driverState != NULL, "GLFW callback called but not driver was instanciated");
	
	switch (state)
	{
		case GLFW_PRESS: driverState->listener->pointerDown(0, button); break;
		case GLFW_RELEASE: driverState->listener->pointerUp(0, button); break;
	}
}

void GLFWCALL mousePosCallback(int x, int y)
{
	OAK_ASSERT(driverState != NULL, "GLFW callback called but not driver was instanciated");
	
	driverState->listener->pointerMove(0, glm::vec2((float)x, (float)y));
}

} // end of private section

InputDriver::InputDriver(InputDriverListener *listener)
{
	this->state = new InputDriverState;
	
	OAK_ASSERT(driverState == NULL, "Cannot instanciate multiple GLFW input drivers (GLFW limitation)");
	driverState = this->state;
	
	this->state->listener = listener;
	
	glfwSetMouseButtonCallback(mouseButtonCallback);
	glfwSetMousePosCallback(mousePosCallback);
	
	// in the GLFW implementation, only one pointer (with ID 0) is ever supported
	// declare it now
	int x;
	int y;
	glfwGetMousePos(&x, &y);
	this->state->listener->pointerAdded(0, glm::vec2((float)x, (float)y));
}

InputDriver::~InputDriver()
{
	// remove the only supported mouse
	this->state->listener->pointerRemoved(0);
	
	glfwSetMouseButtonCallback(NULL);
	glfwSetMousePosCallback(NULL);
	
	driverState = NULL;
	
	delete this->state;
}

void InputDriver::update()
{
	glfwPollEvents();
}

} // oak namespace
