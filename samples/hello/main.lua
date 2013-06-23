require("game.Game")

function initialize()
	oak.system.logInfo("=== SCRIPT START ===")
	
	game = Game.new()
	game:start()
end

function update(dt)
	game:update(dt)
end

function shutdown()
	game:stop()
	
	oak.system.logInfo("=== SCRIPT END ===")
end

function pointerDown(pointerId, button, x, y)
	game:pointerDown(pointerId, button, x, y)
end

function pointerUp(pointerId, button, x, y)
	game:pointerUp(pointerId, button, x, y)
end

function pointerMove(pointerId, x, y, dx, dy)
	game:pointerMove(pointerId, x, y, dx, dy)
end
