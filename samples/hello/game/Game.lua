Game = {}
Game.__index = Game

function Game.new()
    local self = {}
    setmetatable(self, Game)
	
    return self
end

function Game:start()
	graphics.setBackgroundColor(0.6, 0.8, 0.9)
	
	self.world = sg.createWorld()
	local scene = World.createScene(self.world)
	--Scene.load(scene, "maps/test.scene")
	--local scene2 = World.loadScene(self.world, "maps/test.scene")
	
	self.entity1 = Scene.createEntity(scene)
	Entity.setLocalPosition(self.entity1, 0, 2, 0)
	Entity.scale(self.entity1, 1.5, 1.5, 1.5)
	self.cube = Entity.createComponent(self.entity1, "Cube")
	
	-- ground
	self.cubes = {}
	for x = -10, 5 do
		for z = -10, 5 do
			local entity = Scene.createEntity(scene)
			Entity.createComponent(entity, "Cube")
			Entity.setLocalPosition(entity, x * 3, -1.2, z * 3)
			table.insert(self.cubes, entity)
		end
	end
	
	self.camera = Scene.createEntity(scene)
	local cameraComponent = Entity.createComponent(self.camera, "Camera")
	Entity.setLocalPosition(self.camera, 5, 4, 5)
	
	self.cameraAngleX = 0.78
	self.cameraAngleY = -0.5
	
	self.view = graphics.createView(self.world)
	View.setCamera(self.view, cameraComponent)
end

function Game:update(dt)
	local time = system.getTime()
	Entity.rotate(self.entity1, 0, 1, 0, dt * 0.1)
	--Entity.setLocalPosition(self.entity1, math.sin(time * 0.5) * 3.0, math.sin(time), -60)
	
	Entity.setLocalOrientation(self.camera, 1, 0, 0, 0)
	Entity.rotate(self.camera, 1, 0, 0, self.cameraAngleY)
	Entity.rotate(self.camera, 0, 1, 0, self.cameraAngleX)
	
	for i = 1, #self.cubes do
		local entity = self.cubes[i]
		local x, y, z = Entity.getLocalPosition(entity)
		Entity.rotate(entity, x, y, z, dt * 0.2)
	end
end

function Game:stop()
	graphics.destroyView(self.view)
	
	-- will destroy every referenced scene and entity recursively
	sg.destroyWorld(self.world)
end

function Game:pointerDown(pointerId, button, x, y)
	self.dragging = true
end

function Game:pointerUp(pointerId, button, x, y)
	self.dragging = false
end

function Game:pointerMove(pointerId, x, y, dx, dy)
	--Entity.setLocalPosition(self.entity1, 4.0 * (x / 1280) - 2.0, 1.0 - (y / 800) * 2.0, -60)
	
	if self.dragging then
		Entity.rotate(self.entity1, 0, 1, 0, dx * 0.004)
		Entity.rotate(self.entity1, 1, 0, -1, dy * 0.004)
		
		for i = 1, #self.cubes do
			local entity = self.cubes[i]
			Entity.rotate(entity, 0, 1, 0, dx * 0.004)
			Entity.rotate(entity, 1, 0, -1, dy * 0.004)
		end
		
		self.cameraAngleX = self.cameraAngleX + dx * 0.002
		self.cameraAngleY = self.cameraAngleY + dy * 0.002
	end
end
