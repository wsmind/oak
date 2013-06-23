Game = {}
Game.__index = Game

function Game.new()
    local self = {}
    setmetatable(self, Game)
	
    return self
end

function Game:start()
	oak.graphics.setBackgroundColor(0.0, 0.0, 0.0)
	
	self.world = oak.sg.createWorld()
	local scene = oak.World.createScene(self.world)
	
	self.entity1 = oak.Scene.createEntity(scene)
	self.quad = oak.Entity.createComponent(self.entity1, "Cube")
	
	local entity2 = oak.Scene.createEntity(scene)
	self.camera = oak.Entity.createComponent(entity2, "Camera")
	
	self.view = oak.graphics.createView(self.world)
	oak.View.setCamera(self.view, self.camera)
end

function Game:update(dt)
	local time = oak.system.getTime();
	local r, g, b = oak.graphics.getBackgroundColor()
	r = math.sin(time) * 0.5 + 0.5
	oak.graphics.setBackgroundColor(r, g, b)
	oak.Cube.setColor(self.quad, oak.graphics.getBackgroundColor())
	oak.Entity.setLocalPosition(self.entity1, math.sin(time * 0.5) * 3.0, math.sin(time), -60)
end

function Game:stop()
	oak.graphics.destroyView(self.view)
	
	-- will destroy every referenced scene and entity recursively
	oak.sg.destroyWorld(self.world)
end
