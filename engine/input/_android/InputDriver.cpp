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
#include <engine/input/_android/InputDriverState.hpp>

#include <engine/system/Log.hpp>

#include <android/input.h>

namespace oak {

// Input events are routed from AndroidActivity to here, because
// the activity receives input events directly (as far as I know, it
// is not possible to register an independent input listener from here).
// So, we sadly need a global to access the driver state
InputDriverState *driverState = NULL;

int32_t handleAndroidInputEvent(AInputEvent *event)
{
	if (!driverState)
		return 0;
	
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		// handle only the action in this event
		int32_t action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
		int32_t actionPointerIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		int32_t actionPointerId = AMotionEvent_getPointerId(event, actionPointerIndex);
		
		float x = AMotionEvent_getX(event, actionPointerIndex);
		float y = AMotionEvent_getY(event, actionPointerIndex);
		
		if (action == AMOTION_EVENT_ACTION_DOWN)
		{
			driverState->listener->pointerAdded((unsigned int)actionPointerId, glm::vec2(x, y));
			driverState->listener->pointerDown((unsigned int)actionPointerId, 0);
		}
		else if ((action == AMOTION_EVENT_ACTION_UP) || (action == AMOTION_EVENT_ACTION_CANCEL)) // treat cancel as up, for now
		{
			driverState->listener->pointerUp((unsigned int)actionPointerId, 0);
			driverState->listener->pointerRemoved((unsigned int)actionPointerId);
		}
		else if (action == AMOTION_EVENT_ACTION_MOVE)
		{
			driverState->listener->pointerMove((unsigned int)actionPointerId, glm::vec2(x, y));
		}
		
		return 1;
	}
	
	return 0;
}

InputDriver::InputDriver(InputDriverListener *listener)
{
	this->state = new InputDriverState;
	this->state->listener = listener;
	
	OAK_ASSERT(driverState == NULL, "Cannot instanciate multiple Android input drivers");
	driverState = this->state;
}

InputDriver::~InputDriver()
{
	driverState = NULL;
	delete this->state;
}

void InputDriver::update()
{
	// nothing to do here; polling is done in AndroidActivity
}

} // oak namespace
