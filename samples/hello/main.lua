--[[function loadfile(filename)
	oak.script.plop("calling loadfile() with filename: " .. filename)
	--return function() end
end]]--

--[[table.insert(package.searchers, function(filename)
	oak.script.plop("calling special test loader for file: " .. filename)
	return function(arg)
		oak.script.plop("loader! " .. arg)
		return dofile(arg)
	end
end)]]--

--dofile("main")
--require("yop require test")

function initialize()
	oak.system.logInfo("=== SCRIPT START ===")
	
	oak.graphics.setBackgroundColor(0.0, 0.0, 0.0)
	
	world = oak.sg.createWorld()
	scene1 = World.createScene(world)
	scene2 = World.createScene(world)
	
	entity = Scene.createEntity(scene1)
	quad = Entity.createComponent(entity, "DemoQuad")
end

local t = 0
function update(dt)
	local r, g, b = oak.graphics.getBackgroundColor()
	r = r + 0.001
	oak.graphics.setBackgroundColor(r, g, b)
	DemoQuad.setColor(quad, oak.graphics.getBackgroundColor())
	
	--t = t + dt
	--oak.graphics.setBackgroundColor(math.sin(t) * 0.5 + 0.5, 0.8, 1.0)
end

function shutdown()
	-- will destroy every referenced scene and entity recursively
	oak.sg.destroyWorld(world)
	
	oak.system.logInfo("=== SCRIPT END ===")
end
