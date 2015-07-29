dofile("./data/level2/scripts/IdManager.lua")

engine = CEngine.GetSingletonPtr()
action_manager = GetActionManager()
action_manager_lua_wrapper = CActionManagerLuaWrapper()
camera_manager = GetCameraManager()
timer = engine:GetTimer()
physic_manager = GetPhysicsManager()
--enemy_manager = GetEnemyManager()
gizmos_manager = GetGizmosManager()
renderable_objects_layer_manager = GetRenderableObjectsLayersManager()
renderable_objects_manager_characters = renderable_objects_layer_manager:GetResource("characters")
sound_manager = GetSoundManager()
gui_manager = GetGUIManager()
id_manager = CIdManager()
countdowntimer_manager = GetCountDownTimerManager()
script_manager = GetScriptManager()
cinematic_manager = GetCinematicManager()
scene_renderer_commands_manager = GetSceneRendererCommandsManager()
billboard_manager = GetBillboardManager()
light_manager = GetLightManager()

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
	
	local l_EnemyDir = enemy:GetDirectionEnemy()
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
	l_Position.y = l_Position.y - (l_Player:GetHeight()/2.0)
	local lDir = l_Position - pos
	lDir:Normalize()
	
	return lDir
end

function PlayerVisibility(enemy)
	return PlayerInSight(enemy:GetCamera())
end

function ChangeCameraCloseEnemy()
	local lEnemyName = g_EnemyManager:GetCloseEnemy(GetPlayerPosition()):GetName()
	local lActualCameraName = camera_manager:GetCurrentCameraName()
	
	if lActualCameraName == lEnemyName then
		engine:Trace("Changing camera to Player")
		camera_manager:SetCurrentCamera("TestProcessCam")
	else
		engine:Trace("Changing camera to close Enemy --> "..lEnemyName)
		camera_manager:SetCurrentCamera(lEnemyName)
	end
end