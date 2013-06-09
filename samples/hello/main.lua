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
