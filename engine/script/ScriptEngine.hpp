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

#include <string>

struct lua_State;

namespace oak {

class GraphicsEngine;
class SceneManager;
class SystemWrapper;

class ScriptEngine
{
	public:
		ScriptEngine();
		~ScriptEngine();
		
		void initialize();
		void shutdown();
		
		// script file loading
		void loadFile(const std::string &filename);
		
		// function call api
		bool startCall(const std::string &functionName);
		void appendParameter(double value);
		void endCall();
		
		// modules to bind
		void registerScene(SceneManager *scene);
		void registerGraphics(GraphicsEngine *graphics);
		
	private:
		static int luaErrorHandler(lua_State *L);
		
		lua_State *L;
		int errorHandlerStackIndex;
		
		// function call state
		bool callingFunction;
		int callParameterCount;
		
		// built-in system functions
		SystemWrapper *systemWrapper;
};

} // oak namespace
