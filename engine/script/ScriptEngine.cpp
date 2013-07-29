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

#include <engine/script/ScriptEngine.hpp>

#include <engine/script/bind/Bind.hpp>
#include <engine/script/bind/GraphicsBind.hpp>
#include <engine/script/bind/SgBind.hpp>
#include <engine/script/bind/SystemBind.hpp>
#include <engine/script/bind/SystemWrapper.hpp>

#include <engine/system/Log.hpp>

#include <algorithm>
#include <lua.hpp>

#ifdef ANDROID
#	include <android/asset_manager.h>

	// ugly hack
	extern AAssetManager *assetManager;
#endif

namespace oak {

ScriptEngine::ScriptEngine(const std::string &baseFolder)
{
	this->L = NULL;
	this->errorHandlerStackIndex = 0;
	
	this->baseFolder = baseFolder;
	std::replace(this->baseFolder.begin(), this->baseFolder.end(), '\\', '/');
	if (this->baseFolder[this->baseFolder.size() - 1] != '/')
		this->baseFolder += '/';
	
	this->callingFunction = false;
	this->callParameterCount = 0;
	
	// system needs a wrapper for script API
	this->systemWrapper = new SystemWrapper;
}

ScriptEngine::~ScriptEngine()
{
	delete this->systemWrapper;
	
	OAK_ASSERT(this->L == NULL, "ScriptEngine is destroyed, but was not shut down");
}

void ScriptEngine::initialize()
{
	// create the state
	this->L = luaL_newstate();
	
	// initialize standard libs
	luaL_openlibs(this->L);
	
	// error callback function
	lua_pushcfunction(this->L, ScriptEngine::luaErrorHandler);
	this->errorHandlerStackIndex = lua_gettop(this->L);
	
	// save the base loading folder to registry
	lua_pushstring(this->L, this->baseFolder.c_str());
	lua_setfield(this->L, LUA_REGISTRYINDEX, "oakBaseFolder");
	
	// global oak namespace
	lua_createtable(this->L, 0, 0);
	lua_setglobal(this->L, "oak");
	
	// override low-level file loading functions
	lua_register(this->L, "loadfile", ScriptEngine::luaLoadFile);
	lua_register(this->L, "dofile", ScriptEngine::luaDoFile);
	
	// override package.searchers tables (used for require() calls)
	lua_getglobal(this->L, "package");
	lua_createtable(this->L, 1, 0); // package.searchers = {}
	lua_pushinteger(this->L, 1);
	lua_pushcfunction(this->L, ScriptEngine::luaPackageSearcher); // package.searchers[1] = ScriptEngine::luaPackageSearcher
	lua_settable(this->L, -3);
	lua_setfield(this->L, -2, "searchers");
	lua_pop(this->L, 1); // pop the package table
	
	// built-in system functions
	SystemBind::registerFunctions(this->L, this->systemWrapper);
}

void ScriptEngine::shutdown()
{
	// pop error callback function
	lua_pop(this->L, 1);
	
	// ensure no value leaked on the lua stack
	OAK_ASSERT(lua_gettop(this->L) == 0, "Some values (%d) were not removed from the lua stack", lua_gettop(this->L));
	
	lua_close(this->L);
	this->L = NULL;
}

void ScriptEngine::loadFile(const std::string &filename)
{
	// forward the call the dofile()
	lua_getglobal(this->L, "dofile");
	lua_pushstring(this->L, filename.c_str());
	if (lua_pcall(this->L, 1, 0, this->errorHandlerStackIndex) != 0)
		lua_pop(this->L, 1);
}

bool ScriptEngine::startCall(const std::string &functionName)
{
	OAK_ASSERT(!this->callingFunction, "A function call is already in progress");
	
	// check that the symbol exists, and is actually a function
	lua_getglobal(this->L, functionName.c_str());
	if (lua_type(this->L, -1) != LUA_TFUNCTION)
	{
		// just pop the unknown value
		lua_pop(this->L, 1);
		return false;
	}
	
	this->callingFunction = true;
	return true;
}

void ScriptEngine::appendParameter(int value)
{
	if (!this->callingFunction)
		return;
	
	lua_pushinteger(this->L, value);
	this->callParameterCount++;
}

void ScriptEngine::appendParameter(double value)
{
	if (!this->callingFunction)
		return;
	
	lua_pushnumber(this->L, value);
	this->callParameterCount++;
}

void ScriptEngine::endCall()
{
	if (!this->callingFunction)
		return;
	
	// actual call
	if (lua_pcall(this->L, this->callParameterCount, 0, this->errorHandlerStackIndex) != 0)
		lua_pop(this->L, 1);
	
	this->callingFunction = false;
	this->callParameterCount = 0;
}

void ScriptEngine::registerGraphics(GraphicsEngine *graphics)
{
	GraphicsBind::registerFunctions(this->L, graphics);
}

void ScriptEngine::registerSg(WorldManager *sg)
{
	SgBind::registerFunctions(this->L, sg);
}

int ScriptEngine::luaErrorHandler(lua_State *L)
{
	const char *errorMessage = lua_tostring(L, -1);
	Log::error("Lua error: %s", errorMessage);
	
	return 0;
}

int ScriptEngine::luaLoadFile(lua_State *L)
{
	const char *filename = luaL_checkstring(L, -1);
	if (!filename)
	{
		lua_pushstring(L, "loadfile() parameter must be a string");
		lua_error(L);
	}
	
	// get base folder from registry
	lua_getfield(L, LUA_REGISTRYINDEX, "oakBaseFolder");
	const char *baseFolder = lua_tostring(L, -1);
	
	std::string fullPath = std::string(baseFolder) + std::string(filename);
	
	int result = 0;
	
	// temp way to load file from .apk on android
	#ifdef ANDROID
		AAsset *file = AAssetManager_open(assetManager, fullPath.c_str(), AASSET_MODE_UNKNOWN);
		if (file)
		{
			std::string code;
			
			char buf[256];
			int size = 0;
			while ((size = AAsset_read(file, buf, 256)) > 0)
			{
				std::string str(buf, size);
				code += str;
			}
			AAsset_close(file);
			
			result = luaL_loadstring(L, code.c_str());
		}
		else
		{
			std::string errorMessage = "failed to load script file '" + fullPath + "'";
			lua_pushstring(L, errorMessage.c_str());
			result = -1;
		}
	#else
		result = luaL_loadfile(L, fullPath.c_str());
	#endif
	
	if (result != 0)
		lua_error(L);
	
	return 1;
}

int ScriptEngine::luaDoFile(lua_State *L)
{
	const char *filename = luaL_checkstring(L, -1);
	if (!filename)
	{
		lua_pushstring(L, "dofile() parameter must be a string");
		lua_error(L);
	}
	
	// forward the call to loadfile()
	lua_getglobal(L, "loadfile");
	lua_pushstring(L, filename);
	lua_call(L, 1, 1);
	
	// then execute the returned chunk
	lua_call(L, 0, 1);
	
	return 1;
}

int ScriptEngine::luaPackageSearcher(lua_State *L)
{
	const char *packageName = luaL_checkstring(L, -1);
	if (!packageName)
	{
		lua_pushstring(L, "require() parameter must be a string");
		lua_error(L);
	}
	
	// expand package name to file name
	std::string filename = std::string(packageName);
	std::replace(filename.begin(), filename.end(), '.', '/');
	filename += ".lua";
	
	// then forward to loadfile()
	lua_getglobal(L, "loadfile");
	lua_pushstring(L, filename.c_str());
	lua_call(L, 1, 1);
	
	return 1;
}

} // oak namespace
