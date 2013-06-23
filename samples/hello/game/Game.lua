Game = {}
Game.__index = Game

function Game.new()
    local self = {}
    setmetatable(self, Game)
	
    return self
end

function Game:start()
	oak.graphics.setBackgroundColor(0.2, 0.5, 0.7)
	
	self.world = oak.sg.createWorld()
	local scene = oak.World.createScene(self.world)
	
	self.entity1 = oak.Scene.createEntity(scene)
	oak.Entity.setLocalPosition(self.entity1, 0, 0, -80)
	self.quad = oak.Entity.createComponent(self.entity1, "Cube")
	
	local entity2 = oak.Scene.createEntity(scene)
	self.camera = oak.Entity.createComponent(entity2, "Camera")
	
	self.view = oak.graphics.createView(self.world)
	oak.View.setCamera(self.view, self.camera)
end

function Game:update(dt)
	local time = oak.system.getTime()
	oak.Entity.rotate(self.entity1, 0, 1, 0, dt * 10.0)
	--oak.Entity.setLocalPosition(self.entity1, math.sin(time * 0.5) * 3.0, math.sin(time), -60)
end

function Game:stop()
	oak.graphics.destroyView(self.view)
	
	-- will destroy every referenced scene and entity recursively
	oak.sg.destroyWorld(self.world)
end

function Game:pointerDown(pointerId, button, x, y)
	self.dragging = true
end

function Game:pointerUp(pointerId, button, x, y)
	self.dragging = false
end

function Game:pointerMove(pointerId, x, y, dx, dy)
	--oak.Entity.setLocalPosition(self.entity1, 4.0 * (x / 1280) - 2.0, 1.0 - (y / 800) * 2.0, -60)
	
	if self.dragging then
		oak.Entity.rotate(self.entity1, 0, 1, 0, dx * 0.2)
		oak.Entity.rotate(self.entity1, 1, 0, 0, dy * 0.2)
	end
end
