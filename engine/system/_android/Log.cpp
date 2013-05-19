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

#include <android/log.h>

namespace oak {

void Log::info(const std::string &format, ...)
{
	va_list args;
	va_start(args, format);
	__android_log_vprint(ANDROID_LOG_INFO, "oak", format.c_str(), args);
	va_end(args);
}

void Log::warning(const std::string &format, ...)
{
	va_list args;
	va_start(args, format);
	__android_log_vprint(ANDROID_LOG_WARN, "oak", format.c_str(), args);
	va_end(args);
}

void Log::error(const std::string &format, ...)
{
	va_list args;
	va_start(args, format);
	__android_log_vprint(ANDROID_LOG_ERROR, "oak", format.c_str(), args);
	va_end(args);
}

void Log::checkAssert(bool condition, const char *conditionString, const std::string &format, ...)
{
	if (!condition)
	{
		Log::error("Assertion failed: %s", conditionString);
		
		// format error message
		char errorString[2048];
		va_list args;
		va_start(args, format);
		vsnprintf(errorString, 2048, format.c_str(), args);
		va_end(args);
		
		// raise system assert
		__android_log_assert(conditionString, "oak", errorString);
	}
}

} // oak namespace

#endif // OAK_DEBUG
