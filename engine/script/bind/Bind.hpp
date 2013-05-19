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

#include <lua.hpp>

#include <string>
#include <glm/glm.hpp>

#include <engine/system/Log.hpp>

namespace oak {
namespace bind {

template <typename ArgType>
inline ArgType popArgument(lua_State *L)
{
	OAK_ASSERT(false, "Unsupported argument type in Lua binding");
	return ArgType();
}

template <>
inline float popArgument(lua_State *L)
{
	double value = lua_tonumber(L, -1);
	lua_pop(L, 1);
	
	return (float)value;
}

template <>
inline glm::vec3 popArgument(lua_State *L)
{
	lua_Number r = lua_tonumber(L, -3);
	lua_Number g = lua_tonumber(L, -2);
	lua_Number b = lua_tonumber(L, -1);
	lua_pop(L, 3);
	
	return glm::vec3((float)r, (float)g, (float)b);
}

template <>
inline std::string popArgument(lua_State *L)
{
	const char *str = lua_tostring(L, -1);
	lua_pop(L, 1);
	
	return std::string(str);
}

inline int pushReturnValue(lua_State *L, float value)
{
	lua_pushnumber(L, (lua_Number)value);
	
	return 1;
}

inline int pushReturnValue(lua_State *L, glm::vec3 value)
{
	lua_pushnumber(L, (lua_Number)value.x);
	lua_pushnumber(L, (lua_Number)value.y);
	lua_pushnumber(L, (lua_Number)value.z);
	
	return 3;
}

inline int pushReturnValue(lua_State *L, const std::string &value)
{
	lua_pushstring(L, value.c_str());
	
	return 1;
}

inline int pushReturnValue(lua_State *L, void *value)
{
	lua_pushlightuserdata(L, value);
	
	return 1;
}

} // bind namespace
} // oak namespace

#define OAK_BIND_POINTER_TYPE(PointerType) \
	namespace oak { \
	class PointerType; \
	 \
	namespace bind { \
	template <> \
	inline ::oak::PointerType *popArgument(lua_State *L) \
	{ \
		::oak::PointerType *ptr = (::oak::PointerType *)lua_touserdata(L, -1); \
		lua_pop(L, 1); \
		return ptr; \
	} \
	} \
	}

OAK_BIND_POINTER_TYPE(Scene);

#define OAK_BIND_MODULE(ModuleType) \
	ModuleType *oak_module_ptr_##ModuleType = NULL;

#define OAK_BIND_VOID_FUNCTION0(ModuleType, functionName) \
	int oak_function_##ModuleType##_##functionName(lua_State *L) \
	{ \
		ModuleType *module = oak_module_ptr_##ModuleType; \
		 \
		module->functionName(); \
		 \
		return 0; \
	}

#define OAK_BIND_VOID_FUNCTION1(ModuleType, functionName, ArgType1) \
	int oak_function_##ModuleType##_##functionName(lua_State *L) \
	{ \
		ModuleType *module = oak_module_ptr_##ModuleType; \
		 \
		ArgType1 arg1 = oak::bind::popArgument<ArgType1>(L); \
		module->functionName(arg1); \
		 \
		return 0; \
	}

#define OAK_BIND_WRET_FUNCTION0(ModuleType, functionName) \
	int oak_function_##ModuleType##_##functionName(lua_State *L) \
	{ \
		ModuleType *module = oak_module_ptr_##ModuleType; \
		 \
		return oak::bind::pushReturnValue(L, module->functionName()); \
	}

#define OAK_BIND_WRET_FUNCTION1(ModuleType, functionName, ArgType1) \
	int oak_function_##ModuleType##_##functionName(lua_State *L) \
	{ \
		ModuleType *module = oak_module_ptr_##ModuleType; \
		 \
		ArgType1 arg1 = oak::bind::popArgument<ArgType1>(L); \
		return oak::bind::pushReturnValue(L, module->functionName(arg1)); \
	}

#define OAK_REGISTER_MODULE(L, ModuleType, name, instance) \
	{ \
		OAK_ASSERT(oak_module_ptr_##ModuleType == NULL, "Script module "#ModuleType" is already registered!"); \
		 \
		oak_module_ptr_##ModuleType = instance; \
		 \
		lua_getglobal(L, "oak"); \
		lua_createtable(L, 0, 0); \
		lua_setfield(L, -2, #name); \
		lua_pop(L, 1); \
	}

#define OAK_REGISTER_FUNCTION(L, ModuleType, moduleName, functionName) \
	{ \
		lua_getglobal(L, "oak"); \
		lua_getfield(L, -1, #moduleName); \
		lua_pushcfunction(L, oak_function_##ModuleType##_##functionName); \
		lua_setfield(L, -2, #functionName); \
		lua_pop(L, 2); \
	}
