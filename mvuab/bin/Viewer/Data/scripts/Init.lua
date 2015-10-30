dofile("./data/scripts/Player.lua")
dofile("./data/scripts/PatrolEnemy.lua")

g_Player = nil
g_HUD = nil
g_ConsoleActivate = false
g_CinematicActive = false
local initialized = false

function load_basics()
	-- basic loads
end

function load_gameplay()
    g_Player = CPlayer()
	g_Player:SetEnergy(0.0)
	g_HUD = CHUD()
	
	--local pos = Vect3f( 0.0, 0.0, 0.0 )
	local waypoints = {Vect3f(-2.12, 0.4, 5.19), Vect3f(3.30, 0.4, 2.94)}
	--g_Enemy = CPatrolEnemyLUA(waypoints)
	
	--g_stateMachine = CStateMachineLUA("Data/enemies/AI-patroll.xml")
	
	sound_manager:PlayEvent("Play_Long_Ambient", "Ambient" )
end

function update_gameplay()
	if not initialized then
		load_basics()
		load_gameplay()
		initialized = true
	end
	
	g_ConsoleActivate = gui_manager:GetConsole():GetVisible()
	g_CinematicActive = false--cinematic_manager:GetCinematicActive()
	if ( CameraType.Free.value == camera_manager:GetCurrentCamera():GetCameraType() ) then
		UpdateFree()
	else
		if not g_CinematicActive then
			g_Player:Update()
			g_HUD:Update()
		end
		
	end
	
	
	--g_Enemy:Update()
	

	if not (g_ConsoleActivate or g_CinematicActive) then
		if action_manager:DoAction("ChangeRoom") then
			g_Player:SetPosition(ChangeRoom())
		end
		if action_manager:DoAction("PressR") then
			if g_bChargeEnergy then
				ChargeEnergy()
			end
			if g_bChangeRoom then
				g_Player:SetPosition(ChangeRoom())
			end
		end
		if action_manager:DoAction("PressX") then
			if g_bPressX then
				gui_manager:ShowStaticText("Block")
				g_bPressedX = true
			end
		end
	end
	
	if not (g_ConsoleActivate or g_CinematicActive) then
		if action_manager:DoAction("ChangeCamera" ) then
			if "FreeCam" == camera_manager:GetCurrentCameraName() then
				g_Player:SetPosition(camera_manager:GetCurrentCamera():GetPosition())
				l_Yaw = camera_manager:GetCurrentCamera():GetYaw()
				l_Pitch = camera_manager:GetCurrentCamera():GetPitch()
				camera_manager:SetCurrentCamera( "Main" )
				camera_manager:GetCurrentCamera():SetYaw(l_Yaw)
				camera_manager:GetCurrentCamera():SetPitch(l_Pitch)
			else
				l_Yaw = camera_manager:GetCurrentCamera():GetYaw()
				l_Pitch = camera_manager:GetCurrentCamera():GetPitch()
				camera_manager:SetCurrentCamera( "FreeCam" )
				camera_manager:GetCurrentCamera():SetYaw(l_Yaw)
				camera_manager:GetCurrentCamera():SetPitch(l_Pitch)
				l_Pos = g_Player:GetPosition()
				l_Pos.y = l_Pos.y + g_Player:GetHeight()
				camera_manager:GetCurrentCamera():SetPosition(g_Player:GetPosition())
			end
		end
		
		if action_manager:DoAction("ChangeCameraEnemy") then
			ChangeCameraCloseEnemy()
		end
	end
end