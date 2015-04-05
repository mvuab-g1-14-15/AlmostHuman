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

function PlayerDistance(enemy)
	local l_PlayerPos = GetPlayerPosition()
	local l_EnemyPos = enemy:GetPosition()
	
	local l_DistanceVector = l_PlayerPos - l_EnemyPos
	
	return l_DistanceVector:Length()
end

function GetPlayerPosition()
	local l_Player = physic_manager:GetController("Player")
	return l_Player:GetPosition()
end

function PlayerVisibility(enemy)
	local l_PlayerPos = GetPlayerPosition()
	local l_EnemyPos = enemy:GetPosition()
	
	local l_Direction = l_PlayerPos - l_EnemyPos
	l_Direction:Normalize()
	
	local l_Distance = physic_manager:RaycastDistance(l_EnemyPos, l_Direction, 0xffffffff)
	
	if l_Distance < PlayerDistance(enemy) then
		return false
	else
		return true
	end
end