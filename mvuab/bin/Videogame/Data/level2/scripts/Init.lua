dofile("./data/level2/scripts/Player.lua")
dofile("./data/level2/scripts/EnemyManager.lua")
dofile("./data/level2/scripts/Barrel.lua")

g_Player = nil
g_HUD = nil
enemy_manager = nil
g_ConsoleActivate = false
g_CinematicActive = false
g_PauseGame = false
g_Barrels = {}

initialized1 = false

g_bAlarmRoom3 = false
g_fOcultarMensaje = 5.0
g_sMessageAlarm = ""
g_PlayerCinematic = nil

function load_basics()
	engine:Trace("Init the load_basics()")
	-- basic loads
	scene:ActivateRoom("sala1")
	scene:ActivateRoom("pasillo")
	scene:ActivateRoom("sala4")
	scene:ActivateRoom("sala2")
	scene:ActivateRoom("sala3")
	engine:Trace("Finish the load_basics()")
end

function load_gameplay()
	engine:Trace("Init the load_gameplay()")
    g_Player = CPlayer()
	g_Player:SetEnergy(100.0)
	if g_HUD == nil then
		g_HUD = CHUD()
	end
	if enemy_manager == nil then
		enemy_manager = CEnemyManagerLUA()	
	end
	
	enemy_manager:CreateBoss()
	
	g_Barrels["Barrel001"] = CBarrel("Barrel001", Vect3f(76.50, -12.30, -42.30))

	--sound_manager:PlayEvent("Play_Main_Theme", "Ambient" )
	engine:Trace("Finish the load_gameplay()")
	g_Player:Update()
	cinematic_manager:PlayCinematic("cinematica_inicial")
	--
	-- if renderable_objects_manager_characters:AddResource("PlayerCinematic", CreateAnimatedInstanceModel("PlayerCinematic", "playercinematic1")) == false  then
		-- renderable_objects_manager_characters:RemoveResource("PlayerCinematic")
		-- renderable_objects_manager_characters:AddResource("PlayerCinematic", CreateAnimatedInstanceModel("PlayerCinematic", "playercinematic1"))
	-- end
	-- g_PlayerCinematic = renderable_objects_manager_characters:GetResource("PlayerCinematic")
	-- g_PlayerCinematic:SetPosition(Vect3f(124.989,-8.31726,-51.9642))
	-- g_PlayerCinematic:MakeTransform()
	-- g_PlayerCinematic:SetRoomName( "sala1" )
	-- g_PlayerCinematic:ChangeAnimation("anim_cinem_inicial_sala1", 0.2, 1)
	 --g_Player:SetPosition(Vect3f(124.989,-8.31726,-51.9642))
	 --g_Player:Update()
	 --g_Player:GetRenderableObject():ChangeAnimation("cinematica_inicial_sala1", 0.2, 0)
	 --g_Player:SetCurrentState("cinematica_inicial_sala1")

end

function update_gameplay()
	if not initialized1 then
		load_basics()
		load_gameplay()
		initialized1 = true
	end
	
	enemy_manager:Update()
	engine:Trace("El estado actual del player es: "..g_Player:GetCurrentState())
	--g_ConsoleActivate = gui_manager:GetConsole():GetVisible()
	g_CinematicActive = cinematic_manager:GetCinematicActive()
	g_PauseGame = engine_manager:GetbGamePause()
	if ( CameraType.Free.value == camera_manager:GetCurrentCamera():GetCameraType() ) then
		UpdateFree()
	else
		if not (g_CinematicActive or g_PauseGame) then
			g_Player:Update()
			g_HUD:Update()
		end
		
	end
	
	if g_PauseGame then
		if action_manager:DoAction("RightClick") then
			local findialog = gui_manager:NextDialog()
			if findialog then
				gui_manager:ShowDialogText("current")
			end
		end
	end
	
	if action_manager:DoAction("PlayAmbient") then	
		sound_manager:PlayEvent("Play_Long_Ambient", "Ambient" )
	end
	if action_manager:DoAction("Alarm") then	
			sound_manager:PlayEvent("Play_Alarm", "Alarma_Sala2" )	
	end
	
	if action_manager:DoAction("Ambient_salaX") then
		sound_manager:PlayEvent("Play_Ambient_Sala3", "Ambient" )	
	end

	if not (g_ConsoleActivate or g_CinematicActive or g_PauseGame) then
		if action_manager:DoAction("ChangeRoom") then
			if camera_manager:GetCurrentCamera():GetName() == "FreeCam" then				
				camera_manager:GetCurrentCamera():SetPosition(ChangeRoom())
			else
				g_Player:SetPosition(ChangeRoom())
			end
			
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
			if g_bPressRoom2X then
				if CuentaAtras == 3 then
					gui_manager:ShowStaticText("Block")
				end
				g_bPressedRoom2X = true
			elseif g_bOpenDoor2 then
				cinematic_manager:Execute("OpenDoor")
				trigger_manager:GetTriggerByName("puerta_sala2"):SetActive(false)
				gui_manager:ShowStaticText("OpenDoor")
				--Code para abrir puerta
			end
		end	
		
		if action_manager:DoAction("RomperRejilla") then			
			if g_bPressRoom1Button then				
				g_bPressedRoom1Button = true
				cinematic_manager:Execute("rejilla")
			end
		end	
		
		if action_manager:DoAction("OpenDoorRoom3") then
			if g_bPressRoom3X then
				--gui_manager:ShowStaticText("Alarm")
				g_bAlarmRoom3 = true
				enemy_manager:AlarmRoom("room3")				
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
		dt = timer:GetElapsedTime()
		UpdateVariables(dt)
	end
	if g_bInBarrel then
		--engine:Trace("Next to barrel "..g_BarrelName)
		if action_manager:DoAction("EnterBarrel") then
			--engine:Trace("Entered in barrel!!!!!  "..g_BarrelName)
			lBarrel = g_Barrels[g_BarrelName]
			lBarrel:SetStateInside()
			g_bInBarrel = false
			gui_manager:ShowStaticText("HideInBarrel")
		end
	end
	if not (g_ConsoleActivate or g_CinematicActive or g_PauseGame) then
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
	end
end

function UpdateVariables(dt)
	if g_bAlarmRoom3 then
		g_fOcultarMensaje = g_fOcultarMensaje - dt
		if g_fOcultarMensaje <= 0 then
			g_fOcultarMensaje = 5.0
			g_bAlarmRoom3 = false
			gui_manager:ShowStaticText(g_sMessageAlarm)
		end
	end
end