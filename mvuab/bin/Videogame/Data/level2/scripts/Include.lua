dofile("./data/level2/scripts/IdManager.lua")

engine = CEngine.GetSingletonPtr()
action_manager = GetActionManager()
action_manager_lua_wrapper = CActionManagerLuaWrapper()
camera_manager = GetCameraManager()
timer = engine:GetTimer()
physic_manager = GetPhysicsManager()
--enemy_manager = GetEnemyManager()
gizmos_manager = GetGizmosManager()
scene = GetScene()
renderable_objects_manager_characters = scene:GetResource("core"):GetLayers():GetResource("characters")
sound_manager = GetSoundManager()
gui_manager = GetGUIManager()
id_manager = CIdManager()
countdowntimer_manager = GetCountDownTimerManager()
script_manager = GetScriptManager()
cinematic_manager = GetCinematicManager()
scene_renderer_commands_manager = GetSceneRendererCommandsManager()
billboard_manager = GetBillboardManager()
light_manager = GetLightManager()
texture_manager = GetTextureManager()
trigger_manager = GetTriggerManager()

-- Global Variables
g_CameraSensibility = 30.0
g_HalfPi = 1.57079632679
g_Pi = 3.141592
g_DoublePi = 2*g_Pi

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
	local DifHeight = l_EnemyPos.y - l_PlayerPos.y
	if DifHeight < -2 then
		--engine:Trace("El player está más alto")
		return 9999
	end
	local l_DistanceVector = l_PlayerPos - l_EnemyPos
	
	----engine:Trace("Player distance: " .. l_DistanceVector:Length())
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
	
	--engine:Trace("Dot product: " .. lDot)
	
	----engine:Trace(tostring(angle))
	--angle = math.acos(angle)
	----engine:Trace(tostring(angle))
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
	--lDirection = enemy:GetDirectionEnemy()
	--lDirection.y = lDirection.y - enemy:GetHeight()/2.0
	--return physic_manager:PlayerInSight(20, 0.7853, enemy:GetPosition(), lDirection)
	--if enemy:GetType() == "drone" then
	--	return PlayerInSightDrone(enemy:GetCamera())
	--end
	if g_Player:GetIsHidden() then
		return false
	else
		if g_Player:GetIsInBarrel() then
			lBarrel = g_Player:GetActualBarrel()
			lBarrel:SetIsSafe(false)
		end
	end
	return PlayerInSight(enemy:GetCamera())
end

function HearPlayer(enemy)
	local isRunning = g_Player:GetIsRunning()
	local isCrouch = g_Player:GetIsCrouch()
	local playerDistance = PlayerDistance(enemy)
	
	--[[
	local message_str = "Player is "
	if isRunning and isCrouch then
		message_str = message_str.."running and crouching."
	else
		if isRunning then
			message_str = message_str.."running."
		end
		if isCrouch then
			message_str = message_str.."crouching."
		end
	end
	message_str = message_str.." At distance "..playerDistance
	engine:Trace(message_str)
	]]
	
	
	if isRunning and isCrouch then
		return playerDistance < 2.0
	end
	if isRunning then
		return playerDistance < 10.0
	end
	if isCrouch then
		return playerDistance < 0.5
	end
	return playerDistance < 6.0
end	

function ChangeCameraCloseEnemy()
	local lEnemyName = g_EnemyManager:GetCloseEnemy(GetPlayerPosition()):GetName()
	local lActualCameraName = camera_manager:GetCurrentCameraName()
	
	if lActualCameraName == lEnemyName then
		--engine:Trace("Changing camera to Player")
		camera_manager:SetCurrentCamera("TestProcessCam")
	else
		--engine:Trace("Changing camera to close Enemy --> "..lEnemyName)
		--engine:Trace("Cantidad de enemigos: "..g_EnemyManager:GetNumEnemys())
		camera_manager:SetCurrentCamera(lEnemyName)
	end
end