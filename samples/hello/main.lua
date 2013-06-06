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
	oak.graphics.setBackgroundColor(0.0, 0.0, 0.0)
	oak.system.logInfo("yeeeahhaaa")
	oak.system.logWarning("oh la la")
	oak.system.logError("ouch")
	
	scene1 = oak.scene.createScene()
	scene2 = oak.scene.createScene()
	
	--entity = oak.scene.createEntity(scene1)
	entity = Scene.createEntity(scene1)
	quad = oak.scene.createComponent(entity, "DemoQuad")
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
	oak.scene.destroyScene(scene1)
	oak.scene.destroyScene(scene2)
	oak.system.logInfo("shutting down")
end
