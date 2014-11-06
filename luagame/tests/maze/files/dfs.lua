directions = { LEFT, UP, RIGHT, DOWN }

function back_from(dir)
	if dir == LEFT then
		return RIGHT
	elseif dir == RIGHT then
		return LEFT
	elseif dir == UP then
		return DOWN
	elseif dir == DOWN then
		return UP
	else
		return STAY
	end
end

function dfs(back)
	for i, dir in pairs(directions) do
		if dir ~= back then
			if go(dir) then
				dfs(back_from(dir))
			end
		end
	end
	go(back)
end

dfs(STAY)
