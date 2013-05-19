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

#ifdef OAK_DEBUG

#include <engine/system/Log.hpp>

#include <windows.h>
#include <cassert>
#include <cstdio>
#include <string>

namespace oak {

void Log::info(const char *format, ...)
{
	// format error message
	char message[2048] = {0};
	va_list args;
	va_start(args, format);
	vsnprintf(message, 2047, format, args);
	va_end(args);
	
	// append tags
	std::string output = std::string("[INFO] ") + std::string(message) + "\n";
	OutputDebugString(output.c_str());
}

void Log::warning(const char *format, ...)
{
	// format error message
	char message[2048] = {0};
	va_list args;
	va_start(args, format);
	vsnprintf(message, 2047, format, args);
	va_end(args);
	
	// append tags
	std::string output = std::string("[WARN] ") + std::string(message) + "\n";
	OutputDebugString(output.c_str());
}

void Log::error(const char *format, ...)
{
	// format error message
	char message[2048] = {0};
	va_list args;
	va_start(args, format);
	vsnprintf(message, 2047, format, args);
	va_end(args);
	
	// append tags
	std::string output = std::string("[ERR ] ") + std::string(message) + "\n";
	OutputDebugString(output.c_str());
}

void Log::checkAssert(bool condition, const char *conditionString, const char *format, ...)
{
	if (!condition)
	{
		// print failing condition
		Log::error("Assertion failed: %s", conditionString);
		
		// format error message
		char message[2048] = {0};
		va_list args;
		va_start(args, format);
		vsnprintf(message, 2047, format, args);
		va_end(args);
		
		// append tags
		std::string output = std::string("[ERR ] ") + std::string(message) + "\n";
		OutputDebugString(output.c_str());
		
		// break in debugger here, if attached
		DebugBreak();
		
		// trigger system assert
		assert(condition);
	}
}

} // oak namespace

#endif // OAK_DEBUG
