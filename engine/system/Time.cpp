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

#include <engine/system/Time.hpp>

#include <engine/system/PrecisionTime.hpp>

namespace oak {

unsigned long long Time::referenceTime = 0L;
unsigned long long Time::lastFrameTime = 0L;
double Time::elapsedTime = 0.0;

void Time::reset()
{
	Time::referenceTime = PrecisionTime::readNanoseconds();
}

double Time::getTime()
{
	unsigned long long now = PrecisionTime::readNanoseconds();
	unsigned long long applicationTime = now - Time::referenceTime;
	
	return (double)applicationTime / 1000000000.0;
}

double Time::getElapsedTime()
{
	return Time::elapsedTime;
}

void Time::frameStart()
{
	unsigned long long now = PrecisionTime::readNanoseconds();
	Time::elapsedTime = (double)(now - Time::lastFrameTime) / 1000000000.0;
	Time::lastFrameTime = now;
}

} // oak namespace
