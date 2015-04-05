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
g_HalfPi = 1.57079632679

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
	
	--core:trace("Player distance: " .. l_DistanceVector:Length())
	return l_DistanceVector:Length()
end

function GetPlayerPosition()
	local l_Player = physic_manager:GetController("Player")
	return l_Player:GetPosition()
end

function PlayerVisibility(enemy)
	local l_PlayerPos = GetPlayerPosition()
	local l_EnemyPos = enemy:GetPosition()
	
	-- The raycast collision with the capsule of the enemy, must check that
	l_EnemyPos.y = l_EnemyPos.y + enemy:GetHeight() + 0.1
	
	local l_Direction = l_PlayerPos - l_EnemyPos
	l_Direction:Normalize()
	
	local l_ImpactMask = BitOr(2 ^ CollisionGroup.ECG_PLAYER.value, 2 ^ CollisionGroup.ECG_ESCENE.value)
	
	local l_DistanceRaycast = physic_manager:RaycastDistance(l_EnemyPos, l_Direction, l_ImpactMask)
	local l_PlayerRealDistance = PlayerDistance(enemy)
	
	if l_DistanceRaycast < l_PlayerRealDistance then
		return false
	else
		return true
	end
end