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
	local scene1 = oak.World.createScene(self.world)
	local scene2 = oak.World.createScene(self.world)
	
	local entity = oak.Scene.createEntity(scene1)
	self.quad = oak.Entity.createComponent(entity, "DemoQuad")
	
	self.view = oak.graphics.createView(self.world)
end

function Game:update(dt)
	local r, g, b = oak.graphics.getBackgroundColor()
	r = r + 0.001
	oak.graphics.setBackgroundColor(r, g, b)
	oak.DemoQuad.setColor(self.quad, oak.graphics.getBackgroundColor())
end

function Game:stop()
	oak.graphics.destroyView(self.view)
	
	-- will destroy every referenced scene and entity recursively
	oak.sg.destroyWorld(self.world)
end
