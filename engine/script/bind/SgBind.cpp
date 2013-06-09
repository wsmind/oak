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

#include <engine/script/bind/SgBind.hpp>

#include <engine/script/bind/Bind.hpp>

#include <engine/sg/Entity.hpp>
#include <engine/sg/Scene.hpp>
#include <engine/sg/World.hpp>
#include <engine/sg/WorldManager.hpp>

namespace oak {

OAK_BIND_POINTER_TYPE(Component)
OAK_BIND_POINTER_TYPE(Entity)
OAK_BIND_POINTER_TYPE(Scene)
OAK_BIND_POINTER_TYPE(World)

OAK_BIND_MODULE(WorldManager)
OAK_BIND_WRET_FUNCTION0(WorldManager, createWorld)
OAK_BIND_VOID_FUNCTION1(WorldManager, destroyWorld, World *)

OAK_BIND_WRET_METHOD0(World, createScene)
OAK_BIND_VOID_METHOD1(World, destroyScene, Scene *)

OAK_BIND_WRET_METHOD0(Scene, createEntity)
OAK_BIND_VOID_METHOD1(Scene, destroyEntity, Entity *)

OAK_BIND_WRET_METHOD1(Entity, createComponent, std::string)
OAK_BIND_VOID_METHOD1(Entity, destroyComponent, Component *)

void SgBind::registerFunctions(lua_State *L, WorldManager *sg)
{
	OAK_REGISTER_MODULE(L, WorldManager, sg, sg)
	OAK_REGISTER_FUNCTION(L, WorldManager, sg, createWorld)
	OAK_REGISTER_FUNCTION(L, WorldManager, sg, destroyWorld)
	
	OAK_REGISTER_CLASS(L, World)
	OAK_REGISTER_METHOD(L, World, createScene)
	OAK_REGISTER_METHOD(L, World, destroyScene)
	
	OAK_REGISTER_CLASS(L, Scene)
	OAK_REGISTER_METHOD(L, Scene, createEntity)
	OAK_REGISTER_METHOD(L, Scene, destroyEntity)
	
	OAK_REGISTER_CLASS(L, Entity)
	OAK_REGISTER_METHOD(L, Entity, createComponent)
	OAK_REGISTER_METHOD(L, Entity, destroyComponent)
}

} // oak namespace
