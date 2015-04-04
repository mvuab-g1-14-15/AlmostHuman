core = Singleton_Core.get_singleton()
action_manager = core:GetActionManager()
action_manager_lua_wrapper = CActionManagerLuaWrapper()
graphics_manager = core:GetGraphicsManager()
camera_manager = core:GetCameraManager()
--light_manager = core:GetLightManager()
timer = core:GetTimer()
physic_manager = core:GetPhysicsManager()
enemy_manager = core:GetEnemyManager()
process = Singleton_Engine.get_singleton():GetProcess()

-- Global Variables
g_CameraSensibility = 30.0

-- Global Functions
function OverlapSphere(radius, position)
	local l_PUD = physic_manager:OverlapSphere(radius, position)
	return l_PUD:size() > 0
end

function CheckVector(vector)
	return not (vector.x == 0 and vector.y == 0 and vector.z == 0)
end