dofile("./data/level2/scripts/Player.lua")
dofile("./data/level2/scripts/EnemyManager.lua")
dofile("./data/level2/scripts/Barrel.lua")

g_Player = nil
g_HUD = nil
g_EnemyManager = nil
g_ConsoleActivate = false
g_CinematicActive = false
g_Barrels = {}

initialized1 = false

function load_basics()
	engine:Trace("Init the load_basics()")
	-- basic loads
	scene:ActivateRoom("sala1")
	--scene:ActivateRoom("sala4")
	--scene:ActivateRoom("pasillo")
	--scene:ActivateRoom("sala2")
	--scene:ActivateRoom("sala3")
	engine:Trace("Finish the load_basics()")
end

function load_gameplay()
	engine:Trace("Init the load_gameplay()")
    g_Player = CPlayer()
	g_Player:SetEnergy(0.0)
	if g_HUD == nil then
		g_HUD = CHUD()
	end
	if g_EnemyManager == nil then
		g_EnemyManager = CEnemyManagerLUA()	
	end
	
	g_Barrels["Barrel001"] = CBarrel("Barrel001", Vect3f(76.50, -12.30, -42.30))
	
	sound_manager:PlayEvent("Play_Long_Ambient", "Ambient" )
	engine:Trace("Finish the load_gameplay()")
end

function update_gameplay()
	if not initialized1 then
		load_basics()
		load_gameplay()
		initialized1 = true
	end
	
	g_EnemyManager:Update()
	
	--g_ConsoleActivate = gui_manager:GetConsole():GetVisible()
	g_CinematicActive = false--cinematic_manager:GetCinematicActive()
	if ( CameraType.Free.value == camera_manager:GetCurrentCamera():GetCameraType() ) then
		UpdateFree()
	else
		if not g_CinematicActive then
			g_Player:Update()
			g_HUD:Update()
		end
		
	end
	
	if action_manager:DoAction("Alarm") then
			sound_manager:PlayEvent("Play_Alarm", "Alarma_Sala2" )	
	end

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
			engine:Trace("He entrado en PressX")
			if g_bPressRoom2X then
				engine:Trace("He entrado en PressRoom2X")
				if CuentaAtras == 3 then
					gui_manager:ShowStaticText("Block")
				end
				g_bPressedRoom2X = true
			elseif g_bOpenDoor2 then
				--Code para abrir puerta
			end
		end	
		
		if action_manager:DoAction("OpenDoorRoom3") then
			if g_bPressRoom3X then
				--gui_manager:ShowStaticText("Alarm")
				g_EnemyManager:AlarmRoom("room3")				
				g_bPressedRoom3X = true
			elseif g_bOpenDoor3 then
				--Code para abrir puerta
			end
		end	
		
		if action_manager:DoAction("PressR") then
			if g_bPressE then
				g_bPressedE = true
				if g_fC4Colocada == "1" then
					g_bC41 = true
				elseif g_fC4Colocada == "2" then
					g_bC42 = true
				end
			end
		end	
		if action_manager:DoAction("DetonarC4") then
			if g_bDistanceC4 and g_bC41 and g_bC42 then
				g_bC41 = false
				g_bC42 = false
				g_bBombaActivada = true
				gui_manager:ShowStaticText(g_sTextC4Press)
				--Code para montar las cinematicas y matar a los drones
			end
		end	
		
	end
	if g_bInBarrel then
		--engine:Trace("Next to barrel "..g_BarrelName)
		if action_manager:DoAction("EnterBarrel") then
			--engine:Trace("Entered in barrel!!!!!  "..g_BarrelName)
			lBarrel = g_Barrels[g_BarrelName]
			lBarrel:SetStateInside()
			g_bInBarrel = false
		end
	end
	if not (g_ConsoleActivate or g_CinematicActive) then
		if action_manager:DoAction("ChangeCamera" ) then
			if "FreeCam" == camera_manager:GetCurrentCameraName() then
				g_Player:SetPosition(camera_manager:GetCurrentCamera():GetPosition())
				l_Yaw = camera_manager:GetCurrentCamera():GetYaw()
				l_Pitch = camera_manager:GetCurrentCamera():GetPitch()
				camera_manager:SetCurrentCamera( "TestProcessCam" )
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
		
		if action_manager:DoAction("LoadSala1") then
			local lRoom
			if scene:Exist("sala1") then
				engine:Trace("lRoom 1 exist")
				lRoom = scene:GetResource("sala1")
				if lRoom:GetActive() then
					engine:Trace("lRoom 1 active")
					scene:UnloadRoom("sala1")
				else
					engine:Trace("lRoom 1 non active")
					scene:ActivateRoom("sala1")
				end
			else
				engine:Trace("lRoom 1 not exist")
				scene:ActivateRoom("sala1")
			end
		end
		
		
		if action_manager:DoAction("LoadPasillo") then
			local lRoom
			if scene:Exist("pasillo") then
				engine:Trace("lRoom 1 exist")
				lRoom = scene:GetResource("pasillo")
				if lRoom:GetActive() then
					engine:Trace("lRoom 1 active")
					scene:UnloadRoom("pasillo")
				else
					engine:Trace("lRoom 1 non active")
					scene:ActivateRoom("pasillo")
				end
			else
				engine:Trace("lRoom 1 not exist")
				scene:ActivateRoom("pasillo")
			end
		end
		
		if action_manager:DoAction("LoadSala2") then
			local lRoom
			if scene:Exist("sala2") then
				lRoom = scene:			GetResource("sala2")
				if lRoom:GetActive() then
					scene:UnloadRoom("sala2")
				else
					scene:ActivateRoom("sala2")
				end
			else
				scene:ActivateRoom("sala2")
			end
		end
		
		if action_manager:DoAction("LoadSala3") then
			local lRoom
			if scene:Exist("sala3") then
				lRoom = scene:GetResource("sala3")
				if lRoom:GetActive() then
					scene:UnloadRoom("sala3")
				else
					scene:ActivateRoom("sala3")
				end
			else
				scene:ActivateRoom("sala3")
			end
		end
		
		if action_manager:DoAction("LoadSala4") then
			local lRoom
			if scene:Exist("sala4") then
				lRoom = scene:GetResource("sala4")
				if lRoom:GetActive() then
					scene:UnloadRoom("sala4")
				else
					scene:ActivateRoom("sala4")
				end
			else
				scene:ActivateRoom("sala4")
			end
		end
		
		
		
		
		
	end
end