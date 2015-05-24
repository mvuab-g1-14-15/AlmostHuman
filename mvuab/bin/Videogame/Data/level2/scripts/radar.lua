function get_player_position()
	g_HUD:UpdatePositionPlayer()
end

function get_player_orientation_on_map()
	g_HUD:UpdateYawPlayer()
end

function get_enemy_position(name)
	g_HUD:UpdatePositionEnemy(name)
end

function get_enemy_orientation(name)
	g_HUD:UpdateYawEnemy(name)
end