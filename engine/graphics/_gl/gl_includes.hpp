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

// platform-specific GL implementation
#if defined(ANDROID) || defined(EMSCRIPTEN)
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#else
#	include <GL/glew.h>
#	include <GL/glfw.h>
#endif

// gl error debugging
#ifdef OAK_DEBUG

#	include <engine/system/Log.hpp>

	namespace oak {
	inline void checkGLError(const char *callString, const char *filename, int line)
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::string errorConstant = "UNKNOWN ERROR";
			switch (error)
			{
				case GL_INVALID_ENUM: errorConstant = "GL_INVALID_ENUM"; break;
				case GL_INVALID_VALUE: errorConstant = "GL_INVALID_VALUE"; break;
				case GL_INVALID_OPERATION: errorConstant = "GL_INVALID_OPERATION"; break;
				case GL_INVALID_FRAMEBUFFER_OPERATION: errorConstant = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
				case GL_OUT_OF_MEMORY: errorConstant = "GL_OUT_OF_MEMORY"; break;
			}
			
			Log::error("GL call failed: %s -> %s", callString, errorConstant.c_str());
			Log::error("From %s:%d", filename, line);
		}
	}
	} // oak namespace
	
#	define GL_CHECK(glCall) glCall; oak::checkGLError(#glCall, __FILE__, __LINE__);

#else
	
#	define GL_CHECK(glCall) glCall
	
#endif // OAK_DEBUG
