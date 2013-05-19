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

#include <stdio.h>
#include <assert.h>

namespace oak {

void Log::info(const std::string &format, ...)
{
	std::string infoFormat = std::string("[INFO] ") + format + "\n";
	
	// format error message
	va_list args;
	va_start(args, format);
	vprintf(infoFormat.c_str(), args);
	va_end(args);
}

void Log::warning(const std::string &format, ...)
{
	std::string warningFormat = std::string("[WARN] ") + format + "\n";
	
	// format error message
	va_list args;
	va_start(args, format);
	vprintf(warningFormat.c_str(), args);
	va_end(args);
}

void Log::error(const std::string &format, ...)
{
	std::string errorFormat = std::string("[ERR ] ") + format + "\n";
	
	// format error message
	va_list args;
	va_start(args, format);
	vprintf(errorFormat.c_str(), args);
	va_end(args);
}

void Log::checkAssert(bool condition, const char *conditionString, const std::string &format, ...)
{
	if (!condition)
	{
		// print failing condition
		Log::error("Assertion failed: %s", conditionString);
		
		// print error message
		std::string errorFormat = std::string("[ERR ] ") + format + "\n";
		va_list args;
		va_start(args, format);
		vprintf(errorFormat.c_str(), args);
		va_end(args);
		
		// trigger system assert
		assert(condition);
	}
}

} // oak namespace

#endif // OAK_DEBUG
