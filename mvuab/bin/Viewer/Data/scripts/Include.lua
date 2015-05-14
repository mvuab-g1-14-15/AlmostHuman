engine = CEngine.GetSingletonPtr()
action_manager = GetActionManager()
action_manager_lua_wrapper = CActionManagerLuaWrapper()
camera_manager = GetCameraManager()
timer = engine:GetTimer()
physic_manager = GetPhysicsManager()
enemy_manager = GetEnemyManager()
gizmos_manager = GetGizmosManager()
renderable_objects_layer_manager = GetRenderableObjectsLayersManager()
renderable_objects_manager_characters = renderable_objects_layer_manager:GetResource("characters")
sound_manager = GetSoundManager()
process = CEngine.GetSingletonPtr():GetProcess()
gui_manager = GetGUIManager()

-- Global Variables
g_CameraSensibility = 30.0
g_HalfPi = 1.57079632679
g_Pi = 3.141592

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
	
	--engine:Trace("Player distance: " .. l_DistanceVector:Length())
	return l_DistanceVector:Length()
end

function GetPlayerPosition()
	local l_Player = physic_manager:GetController("Player")
	local l_Position =  l_Player:GetPosition()
	--l_Position.y = l_Position.y - l_Player:GetHeight() / 2.0
	return l_Position
end

function PlayerVisibility(enemy)
	local l_EnemyPos = enemy:GetPosition()
	local l_EnemyDir = enemy:GetRenderableObject():GetDirection()
	
	--engine:Trace("Enemy dir: " .. l_EnemyDir:Vect3f2String())
	
	l_ViewingPlayer = physic_manager:PlayerInSight(8, 45, l_EnemyPos, l_EnemyDir)

	return l_ViewingPlayer
--[[
	local l_PlayerPos = GetPlayerPosition()
	local l_EnemyPos = enemy:GetPosition()
	
	local l_Direction = l_PlayerPos - l_EnemyPos
	if CheckVector(l_Direction) then
		l_Direction:Normalize()
	end
	
	local l_ImpactMask = BitOr(2 ^ CollisionGroup.ECG_PLAYER.value, 2 ^ CollisionGroup.ECG_ESCENE.value)
	
	-- The impact mask is not used
	local l_CollisionGroup = physic_manager:RaycastType(l_EnemyPos + l_Direction * enemy:GetRadius(), l_Direction, l_ImpactMask)
	
	--engine:Trace("Group: " .. l_CollisionGroup)
	
	if l_CollisionGroup == CollisionGroup.ECG_PLAYER.value then
		return true
	else
		return false
	end
]]
end