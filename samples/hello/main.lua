function initialize()
	graphics_setBackgroundColor(0.2, 0.8, 1.0)
end

local t = 0
function update(dt)
	t = t + dt
	graphics_setBackgroundColor(math.sin(t) * 0.5 + 0.5, 0.8, 1.0)
end

function shutdown()
	oak.plop("shutting down")
end
