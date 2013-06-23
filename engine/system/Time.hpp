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

namespace oak {

class Time
{
	public:
		// set the time reference so that application time is reset to 0 now
		static void reset();
		
		// get current application time (in seconds)
		static double getTime();
		
		// get elapsed time since last frame (in seconds)
		static double getElapsedTime();
		
		// signal a frame boundary (used to compute elapsed time)
		static void frameStart();
		
	private:
		// time at which reset() was last called (nanoseconds)
		static unsigned long long referenceTime;
		
		// time at which the last frame started (nanoseconds)
		static unsigned long long lastFrameTime;
		
		// time elapsed since the last frame (seconds)
		static double elapsedTime;
};

} // oak namespace
