function set_initial_waypoint()
	--TODO RAUL
	enemy = Enemy_Manager:GetActualEnemy()
	enemy:setTargetPosition(Vect3f(10,0,10))
end

function change_state()
	enemy = Enemy_Manager:GetActualEnemy()
	core = Singleton_Core.get_singleton()
	physicManager = core:GetPhysicManager()
	local character_controller_UserData = physicManager:GetUserData("CharacterController")
	local character_controller = character_controller_UserData:GetController()
end
