function set_initial_waypoint()
	core = Singleton_Core.get_singleton()
	Enemy_Manager = core:GetEnemyManager()
	enemy = Enemy_Manager:GetActualEnemy()
	enemy:setTargetPosition(Vect3f(10,0,10))
end

function change_state()
	core = Singleton_Core.get_singleton()
	Enemy_Manager = core:GetEnemyManager()
	enemy = Enemy_Manager:GetActualEnemy()
	physicManager = core:GetPhysicsManager()
	local character_controller_UserData = physicManager:GetUserData("CharacterController")
	local character_controller = character_controller_UserData:GetController()
	local character_pos = character_controller:GetPosition()
	
	local vector_dist = character_pos - enemy:GetPosition()
	
	if (vector_dist.x*vector_dist.x+vector_dist.y*vector_dist.y+vector_dist.z*vector_dist.z) < 10 then
		enemy:setTargetPosition(character_pos)
	end
end
