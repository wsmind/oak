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

#include <engine/sg/Component.hpp>

#include <glm/glm.hpp>

namespace oak {

class Entity;

class Camera: public Component
{
	public:
		Camera();
		virtual ~Camera() {}
		
		// Component
		virtual void attachComponent(Entity *entity) { this->entity = entity; }
		virtual void detachComponent(Entity *entity) { this->entity = NULL; }
		
		Entity *getEntity() const { return this->entity; }
		
		float getFov() const { return this->fov; }
		float getAspect() const { return this->aspect; }
		float getNearPlane() const { return this->nearPlane; }
		float getFarPlane() const { return this->farPlane; }
		
		void setFov(float fov) { this->fov = fov; this->updateProjection(); }
		void setAspect(float aspect) { this->aspect = aspect; this->updateProjection(); }
		void setNearPlane(float nearPlane) { this->nearPlane = nearPlane; this->updateProjection(); }
		void setFarPlane(float farPlane) { this->farPlane = farPlane; this->updateProjection(); }
		
		const glm::mat4 &getProjectionMatrix() const { return this->projectionMatrix; }
		
	private:
		void updateProjection();
		
		Entity *entity;
		
		float fov;
		float aspect;
		float nearPlane;
		float farPlane;
		
		glm::mat4 projectionMatrix;
};

} // oak namespace
