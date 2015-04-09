core = CCore.GetSingletonPtr()
action_manager = core:GetActionManager()
action_manager_lua_wrapper = CActionManagerLuaWrapper()
camera_manager = core:GetCameraManager()
timer = core:GetTimer()
physic_manager = core:GetPhysicsManager()
enemy_manager = core:GetEnemyManager()
gizmos_manager = core:GetGizmosManager()
particle_manager = core:GetParticleManager()
renderable_objects_layer_manager = core:GetRenderableObjectsLayersManager()
renderable_objects_manager_characters = renderable_objects_layer_manager:GetResource("characters")
process = CEngine.GetSingletonPtr():GetProcess()

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
	l_EnemyPos.y = l_EnemyPos.y + enemy:GetHeight()
	
	local l_DistanceVector = l_PlayerPos - l_EnemyPos
	
	--core:Trace("Player distance: " .. l_DistanceVector:Length())
	return l_DistanceVector:Length()
end

function GetPlayerPosition()
	local l_Player = physic_manager:GetController("Player")
	local l_Position =  l_Player:GetPosition()
	--l_Position.y = l_Position.y - l_Player:GetHeight() / 2.0
	return l_Position
end

function PlayerVisibility(enemy)
	local l_PlayerPos = GetPlayerPosition()
	local l_EnemyPos = enemy:GetPosition();
	
	-- The raycast collision with the capsule of the enemy, must check that
	l_EnemyPos.y = l_EnemyPos.y + 0.1
	l_PlayerPos.y = l_PlayerPos.y - 0.1
	
	local l_Direction = l_PlayerPos - l_EnemyPos
	l_Direction:Normalize()
	
	local l_ImpactMask = BitOr(2 ^ CollisionGroup.ECG_PLAYER.value, 2 ^ CollisionGroup.ECG_ESCENE.value)
	
	-- The impact mask is not used
	local l_CollisionGroup = physic_manager:RaycastType(l_EnemyPos, l_Direction, l_ImpactMask)
	
	--core:Trace("Group: " .. l_CollisionGroup)
	
	if l_CollisionGroup == CollisionGroup.ECG_PLAYER.value then
		return true
	else
		return false
	end
end