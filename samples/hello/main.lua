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
	oak.graphics.setBackgroundColor(0.2, 0.8, 1.0)
end

local t = 0
function update(dt)
	t = t + dt
	oak.graphics.setBackgroundColor(math.sin(t) * 0.5 + 0.5, 0.8, 1.0)
end

function shutdown()
	oak.script.plop("shutting down")
end
