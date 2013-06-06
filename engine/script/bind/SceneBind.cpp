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

#include <engine/script/bind/SceneBind.hpp>

#include <engine/scene/Scene.hpp>
#include <engine/scene/SceneManager.hpp>
#include <engine/script/bind/Bind.hpp>

namespace oak {

OAK_BIND_POINTER_TYPE(Component)
OAK_BIND_POINTER_TYPE(Entity)
OAK_BIND_POINTER_TYPE(Scene)

OAK_BIND_MODULE(SceneManager)
OAK_BIND_WRET_FUNCTION0(SceneManager, createScene)
OAK_BIND_VOID_FUNCTION1(SceneManager, destroyScene, Scene *)
OAK_BIND_WRET_FUNCTION1(SceneManager, createEntity, Scene *)
OAK_BIND_VOID_FUNCTION2(SceneManager, destroyEntity, Scene *, Entity *)
OAK_BIND_WRET_FUNCTION2(SceneManager, createComponent, Entity *, std::string)
OAK_BIND_VOID_FUNCTION2(SceneManager, destroyComponent, Entity *, Component *)

OAK_BIND_WRET_METHOD0(Scene, createEntity)
OAK_BIND_VOID_METHOD1(Scene, destroyEntity, Entity *)

void SceneBind::registerFunctions(lua_State *L, SceneManager *scene)
{
	OAK_REGISTER_MODULE(L, SceneManager, scene, scene)
	OAK_REGISTER_FUNCTION(L, SceneManager, scene, createScene)
	OAK_REGISTER_FUNCTION(L, SceneManager, scene, destroyScene)
	OAK_REGISTER_FUNCTION(L, SceneManager, scene, createEntity)
	OAK_REGISTER_FUNCTION(L, SceneManager, scene, destroyEntity)
	OAK_REGISTER_FUNCTION(L, SceneManager, scene, createComponent)
	OAK_REGISTER_FUNCTION(L, SceneManager, scene, destroyComponent)
	
	OAK_REGISTER_CLASS(L, Scene)
	OAK_REGISTER_METHOD(L, Scene, createEntity)
	OAK_REGISTER_METHOD(L, Scene, destroyEntity)
}

} // oak namespace