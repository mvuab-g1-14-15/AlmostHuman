dofile("./data/level2/scripts/IdManager.lua")

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
gui_manager = GetGUIManager()
id_manager = CIdManager()
countdowntimer_manager = GetCountDownTimerManager()
script_manager = GetScriptManager()
cinematic_manager = GetCinematicManager()

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

function GetAngleEnemyPlayer(enemy)
	local l_PlayerPos = GetPlayerPosition()
	local l_EnemyPos = enemy:GetPosition()
	
	l_EnemyPos.y = l_EnemyPos.y + enemy:GetHeight()
	
	local l_DistanceVector = l_PlayerPos - l_EnemyPos
	l_DistanceVector.y = 0.0
	l_DistanceVector:Normalize()
	
	local l_EnemyDir = enemy:GetDirection()
	l_EnemyDir.y = 0.0
	l_EnemyDir:Normalize()
	
	--local angle = (l_EnemyDir.x*l_DistanceVector.x + l_EnemyDir.y*l_DistanceVector.y + l_EnemyDir.z*l_DistanceVector.z)
	
	local lDot = l_DistanceVector:DotProduct(l_EnemyDir)
	
	engine:Trace("Dot product: " .. lDot)
	
	--engine:Trace(tostring(angle))
	--angle = math.acos(angle)
	--engine:Trace(tostring(angle))
	return lDot
end

function GetPlayerPosition()
	local l_Player = physic_manager:GetController("Player")
	local l_Position =  l_Player:GetPosition()
	--l_Position.y = l_Position.y - l_Player:GetHeight() / 2.0
	return l_Position
end

function GetPlayerDirection(pos)
	local l_Player = physic_manager:GetController("Player")
	local l_Position =  l_Player:GetPosition()
	
	local lDir = l_Position - pos;
	lDir:Normalize()
	
	return l_Dir
end

function PlayerVisibility(enemy)
	local l_EnemyPos = enemy:GetPosition()
	local l_EnemyDir = enemy:GetDirection()
	
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