dofile("./data/scripts/Player.lua")
dofile("./data/scripts/EnemyManager.lua")
dofile("./data/scripts/Barrel.lua")

g_Player = nil
g_HUD = nil
enemy_manager = nil
g_ConsoleActivate = false
g_CinematicActive = false
g_PauseGame = false
g_Barrels = {}
g_bCinematicLoopMain = false

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
		enemy_manager = CEnemyManager()	
	end
	
	enemy_manager:CreateBoss()
	
	g_Barrels["Barrel001"] = CBarrel("Barrel001", Vect3f(60.1564, -17.9148,-56.3638))
	g_Barrels["Barrel002"] = CBarrel("Barrel002", Vect3f(51.7177, -17.9148, -67.583))
	g_Barrels["Barrel003"] = CBarrel("Barrel003", Vect3f(55.4367, -17.9148, -80.3449))
	g_Barrels["Barrel004"] = CBarrel("Barrel004", Vect3f(58.072, -17.388, 5.96274))
	g_Barrels["Barrel005"] = CBarrel("Barrel005", Vect3f(30.6394, -17.9324, 67.8429))
	g_Barrels["Barrel006"] = CBarrel("Barrel006", Vect3f(33.833, -17.9324, 46.2461))
	g_Barrels["Barrel007"] = CBarrel("Barrel007", Vect3f(19.5512, -17.9324, 51.9935))
	g_Barrels["Barrel008"] = CBarrel("Barrel008", Vect3f(19.0254, -17.9324, 74.0891))
	g_Barrels["Barrel009"] = CBarrel("Barrel009", Vect3f(35.0987, -17.9324, 67.464))
	g_Barrels["Barrel010"] = CBarrel("Barrel010", Vect3f(46.8367, -17.9324, 58.3305))
	
	--Load Ambient
	sound_manager:PlayEvent("Play_Main_Theme", "Ambient" )
	
	--Sounds Sala3 TODO: Poner en metodos de carga de sonidos por sala
	--Load Assembly Machine
	sound_manager:PlayEvent("Play_Machine_Assembly", "Assembly_Machine1_2")
	sound_manager:PlayEvent("Play_Machine_Assembly", "Assembly_Machine2_2")
	--Load Robot Assembly
	sound_manager:PlayEvent("Play_Robot_Arm", "Robot_assembly1")
	sound_manager:PlayEvent("Play_Robot_Arm", "Robot_assembly2")
	sound_manager:PlayEvent("Play_Robot_Arm", "Robot_assembly3")
	sound_manager:PlayEvent("Play_Robot_Arm", "Robot_assembly4")
	sound_manager:PlayEvent("Play_Robot_Arm", "Robot_assembly5")
	
	engine:Trace("Finish the load_gameplay()")
	--cinematic_manager:PlayCinematic("cinematica_inicial")
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
	g_Player:SetPosition(Vect3f(124.989,-7.31726,-51.9642))
	g_bCinematicLoopMain = true
	g_Player:SetAnimation("cinematica_inicial_sala1_pos00", 0.2, 0.0)
	g_Player:SetAnimation("cinematica_inicial_sala1_pos00", 0.2, 0.0)
	scene:GetResource("sala3"):GetLayer("solid"):GetResource("robot_assembly007_sala3"):ChangeAnimation("move", 0.2, 0)
end

function update_gameplay()
	if not initialized1 then
		load_basics()
		load_gameplay()
		initialized1 = true
	end
	
	debug_helper:Update()

	enemy_manager:Update()
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
		
		if g_bChargeEnergy then
			if action_manager:DoAction("Action") then
				ChargeEnergy()
			end
		end
		
		if g_bChangeRoom then
			if action_manager:DoAction("Action") then
				g_Player:SetPosition(ChangeRoom())
			end
		end
		
		if g_bPressRoomPasillo then
			if action_manager:DoAction("Action") then
					cinematic_manager:Execute("OpenDoorPasillo")
					trigger_manager:GetTriggerByName("pasillo_sala3"):SetActive(false)
					gui_manager:ShowStaticText("OpenDoor")
					physic_manager:ReleasePhysicActor(physic_manager:GetActor("pasilloDoorEscenario"))				
					g_bPressRoomPasillo = false
			end
		end
		
		if g_bPressRoom2X then
			if action_manager:DoAction("Action") then
				if enemigosVivos >= 1 then
					
					trigger_manager:GetTriggerByName("puerta_sala2"):SetActive(false)
					
					g_bPressRoom2X = false
					physic_manager:ReleasePhysicActor(physic_manager:GetActor("sala2DoorEscenario"))				
				elseif CuentaAtras == 3 then
					gui_manager:ShowStaticText("Block")
				end
				g_bPressedRoom2X = true
			end
		end
		-- if g_bOpenDoor2 then
			-- if action_manager:DoAction("Action") then						
				-- cinematic_manager:Execute("OpenDoor")
				-- trigger_manager:GetTriggerByName("puerta_sala2"):SetActive(false)
				-- gui_manager:ShowStaticText("OpenDoor")
				-- physic_manager:ReleasePhysicActor("puerta_fisica_sala2")
				-- --Code para abrir puerta
			-- end
		-- end	
		if g_bPressRoom1X then		
			if action_manager:DoAction("Action") then
				g_bPressedRoom1X = true
			end	
		end
		
		if g_bPressRoom1Button then
			if action_manager:DoAction("Action") then								
				g_bPressedRoom1Button = true
				cinematic_manager:Execute("rejilla")
				physic_manager:ReleasePhysicActor(physic_manager:GetActor("sala1DoorEscenario"))	
			end
		end	
		
		if g_bPressRoom3X then
			if action_manager:DoAction("Action") then		
				--gui_manager:ShowStaticText("Alarm")
				g_bAlarmRoom3 = true
				--enemy_manager:AlarmRoom("room3")				
				g_bPressedRoom3X = true
			end
		end
		if g_bOpenDoor3 then
			if action_manager:DoAction("Action") then
				--Code para abrir puerta
			end
		end	
		
		if g_bPressE then
			if action_manager:DoAction("Action") then			
				g_bPressedE = true
				if g_fC4Colocada == "1" then
					g_bC41 = true
				elseif g_fC4Colocada == "2" then
					g_bC42 = true
				end
			end
		end	
		
		if g_bDistanceC4 and g_bC41 and g_bC42 then
			if action_manager:DoAction("Action") then
				g_bC41 = false
				g_bC42 = false
				g_bBombaActivada = true
				gui_manager:ShowStaticText(g_sTextC4Press)
				cinematic_manager:Execute("explotion")
				g_Player:SetPosition(Vect3f(-4.58296, -14.0589, 60.0993))
				g_Player:ChangeAnimation("logan_cinem_sala3", 0.2, 0)
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