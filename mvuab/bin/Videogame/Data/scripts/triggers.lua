g_bChargeEnergy = false
g_bChangeRoom = false
g_bPressRoom1X = false
g_bPressedRoom1X = false
g_bBlockShow = false
g_bPressedRoom1Button = false
g_bPressRoom1Button = false
g_bPressRoomPasillo = false
g_bRejillaBreak = false
g_bPressRoom2X = false
g_bPressRoom3X = false
g_bPressedRoom2X = false
g_bPressedRoom3X = false
g_bOpenDoor2 = false
g_bOpenDoor3 = false
g_bInBarrel = false
CuentaAtras = 3
engine = CEngine.GetSingletonPtr()
g_fC4Colocada = 0
g_bPressE = false
g_bPressedE = false
g_bC41 = false
g_bC42 = false
g_bDistanceC4 = false
g_sTextC4Press = ""
g_bBombaActivada = false
g_bFootstepType = "pavimento"

g_EnteredSala2 = false
g_EnteredPasillo = false
g_EnteredSala3 = false
g_EnteredSala4 = false

g_Hacked = {}
g_Hacked.nave1 = false
g_Hacked.nave2 = false
g_Hacked.nave3 = false
g_Hacked.nave4 = false

enemigosVivos = 1
function OnEnter()
	process = engine:GetProcess()
	physicUserData = process:GetNewPUD("Box6")
	physicUserData:SetColor(0,1,0,1)
	physicUserData:SetPaint(true)
	process:AddPudVector(physicUserData)
	physicActor = process:GetNewPhysicActor(physicUserData)
	physicActor:AddBoxShape(Vect3f( 1, 1, 1), Vect3f( 5, 8, 5), Vect3f( 0, 0, 0), Vect3f( 0, 0, 0))
	physicActor:CreateBody(1, 0.5, 0.5)
	process:AddPhysicActorVector(physicActor)
	physic_manager:AddPhysicActor(physicActor)
end

function OnLeave(other_shape)
	process = engine:GetProcess()
	process:SetSalir(true)
	physicActor = physic_manager:GetActor(other_shape)
	physicUserData = physicActor:GetUserData()
	physicUserData:SetColor(1,1,0,1)
end

function OnStay(other_shape)
	process = engine:GetProcess()
	physicUserData = process:GetLastPUDInserted()
	physicUserData:SetColor(1,0,0,1)
	
	physicActor2 = physic_manager:GetActor(other_shape)
	physicUserData2 = physicActor2:GetUserData()
	physicUserData2:SetColor(0,1,1,1)
end

function ShowImage(other_shape)
	--cinematic_manager:Execute("cinematica_1")
	if g_bChargeEnergy then
		g_bChargeEnergy = false
	else
		g_bChargeEnergy = true
	end
	gui_manager:ShowStaticText("ButtonEnergy")
end

function ShowImage1(other_shape)
	--cinematic_manager:Execute("cinematica_1")
	if g_bChargeEnergy then
		--enemy_manager:AlarmRoom("sala2")
		g_bChargeEnergy = false
	else
		g_bChargeEnergy = true
	end
	gui_manager:ShowStaticText("ButtonEnergy")
end

function ShowImageRoom(other_shape)
	--cinematic_manager:Execute("cinematica_1")
	if g_bChangeRoom then
		g_bChangeRoom = false
	else
		g_bChangeRoom = true
	end
	gui_manager:ShowStaticText("CambiarSala")
end

function ShowTextDoor2(text, other_shape)
	if g_bPressRoom2X then
		g_bPressRoom2X = false
		if CuentaAtras ~= 3 then
			CuentaAtras = 0
			gui_manager:ShowStaticText("Block")
		end
	else
		g_bPressRoom2X = true
	end
	gui_manager:ShowStaticText(text)
end

function ShowTextDoorPasillo(text, other_shape)
	if g_bPressRoomPasillo then
		g_bPressRoomPasillo = false
	else
		g_bPressRoomPasillo = true
	end
	gui_manager:ShowStaticText(text)
end

function OpenDoor(text, other_shape)
	if g_bPressRoom3X then
		g_bPressRoom3X = false	
	else
		g_bPressRoom3X = true
	end
	gui_manager:ShowStaticText(text)
end

function ShowText(text, num, textC4Colocada, text2, other_shape)
	if num == "1" then
		if not g_bPressedE and g_bC41 then
			g_bPressE = false
			g_bPressedE = true
			gui_manager:ShowStaticText(textC4Colocada)
			trigger_manager:GetTriggerByName("punto_explosivo_1_sala3"):SetActive(false)
		elseif g_bPressE then
			g_bPressE = false
			gui_manager:ShowStaticText(text)
		else
			g_bPressE = true
			g_bPressedE = false
			g_fC4Colocada = num
			gui_manager:ShowStaticText(text)
		end
	elseif num == "2" then
		if not g_bPressedE and g_bC42 then
			g_bPressE = false	
			g_bPressedE = true
			gui_manager:ShowStaticText(textC4Colocada)
			trigger_manager:GetTriggerByName("punto_explosivo_2_sala3"):SetActive(false)
		elseif g_bPressE then
		g_bPressE = false
		gui_manager:ShowStaticText(text)
		else
			g_bPressE = true
			g_bPressedE = false
			g_fC4Colocada = num
			gui_manager:ShowStaticText(text)
		end
	end
	
	
end

function CheckC4(num, textC4Colocada, text2, other_shape)
	
	if g_bPressedE then
		gui_manager:ShowStaticText(num)
		g_bPressedE = false
		gui_manager:ShowStaticText(textC4Colocada)
		gui_manager:ShowStaticText(text2, true)
	end
		
end

function ShowDetonar(text, text2, other_shape)
	if g_bC41 and g_bC42 then
		g_bDistanceC4 = true
		g_sTextC4Press = text
		gui_manager:ShowStaticText(text)
	else
		g_bDistanceC4 = false
		g_sTextC4Press = text2
		gui_manager:ShowStaticText(text2)
	end
end

function OpenRejilla(text, other_shape)
	if g_bPressRoom1Button and not g_bRejillaBreak then
		g_bPressRoom1Button = false	
		gui_manager:ShowStaticText(text)
	elseif not g_bPressRoom1Button and not g_bRejillaBreak then
		g_bPressRoom1Button = true
		gui_manager:ShowStaticText(text)
	end
end

function StayRejilla(text, other_shape)
	if not g_bRejillaBreak and g_bPressedRoom1Button then
		g_bRejillaBreak = true
		g_bPressRoom1Button = false
		trigger_manager:GetTriggerByName("rejilla_sala1"):SetActive(false)	
		--sound_manager:PlayEvent("Play_Crash_Metal_Grille", "Rejilla" )
		gui_manager:ShowStaticText(text)		
		g_bPressRoom1Button = false	
	end
end

function StayText(room, message, other_shape)
	enemigosVivos = enemy_manager:GetNumEnemy(room)
	engine:Trace("Sala  "..room)
	engine:Trace("Hay estos enemigos vivos "..enemigosVivos)
	if room == "sala2" then
		if g_bPressedRoom2X and enemigosVivos >= 1 then
			CuentaAtras = CuentaAtras - timer:GetElapsedTime()	
			if CuentaAtras <= 0 then
				gui_manager:ShowStaticText(message)
				CuentaAtras = 3
				g_bPressedRoom2X = false
			end
			
		elseif g_bPressedRoom2X then
			
			gui_manager:ShowStaticText(message)
			CuentaAtras = 3
			g_bPressedRoom2X = false
			g_bPressRoom2X = false
			g_bOpenDoor2 = true
			--Codigo para cambiar de sala o abrir la puerta
			
		end
	elseif room == "sala3" then
		if g_bPressedRoom3X and enemigosVivos >= 1 then
				gui_manager:ShowStaticText(message)
				g_sMessageAlarm = message
				g_bPressedRoom3X = false
		elseif g_bPressedRoom3X then
			--gui_manager:ShowStaticText(message)
			g_bPressedRoom3X = false
			g_bPressRoom3X = false
			g_bOpenDoor3 = true
			cinematic_manager:Execute("elevatorDown")
			physic_manager:ReleasePhysicActor(physic_manager:GetActor("sala3DoorEscenario"))
			--Codigo para cambiar de sala o abrir la puerta
			
		end
	elseif room == "sala1" then
		if g_bPressedRoom1X and not g_bBlockShow then
				gui_manager:ShowStaticText(message)
				g_bBlockShow = true
				g_bPressedRoom1X = false		
		end
	end
	
end

function StayTextElevatorDown(text, other_shape)
	if g_bPressedRoom3X then
		gui_manager:ShowStaticText(text)
		g_bPressedRoom3X = false
		g_bPressRoom3X = false
		cinematic_manager:Execute("elevatorDown")
		trigger_manager:GetTriggerByName("elevator_sala3"):SetActive(false)			
	end
end

function StayTextPasillo(room, other_shape)
	if g_bPressedRoomPasillo and enemigosVivos then
		g_bPressedRoomPasillo = false
		g_bPressRoomPasillo = false
		g_bOpenPasillo = true
		--Codigo para cambiar de sala o abrir la puerta
	end
end

function GetEnemyLive(room)
	lEnemy = enemy_manager:GetEnemys()
	--engine:Trace("La room es "..room)
	for i in pairs (lEnemy) do
		lActualEnemy = lEnemy[i]
		if lEnemy[i] ~= nil and lActualEnemy:GetRoom() == room then
			return true
		end
	end
	return false	
end

function HiddenBarrelOnEnter(aName, other_shape)
	if not g_bInBarrel then
		gui_manager:ShowStaticText("HideInBarrel")
		g_bInBarrel = true
		g_BarrelName = aName
	end
end

function HiddenBarrelOnLeave(aName, other_shape)
	if g_bInBarrel then
		gui_manager:ShowStaticText("HideInBarrel")
		g_bInBarrel = false
	end
end

function HiddenBarrelExit(aName, other_shape)
	local lBarrel = g_Barrels[aName]
	lBarrel:RestoreBarrel()
end

function HideImage(other_shape)
	gui_manager:ShowImage(false)
end

function ChargeEnergy()
	g_Player:AddEnergy(5)
end

function TracePhysX(message, other_shape)
	--engine:Trace(message)
end

function ShowTextDoor1(message, other_shape)
	if g_bPressRoom1X then
		g_bPressRoom1X = false	
		if g_bBlockShow then
			g_bBlockShow = false
			gui_manager:ShowStaticText("Block")			
		end
	else
		g_bPressRoom1X = true
	end
	gui_manager:ShowStaticText(message)
end

function PlaySound(event,game_object)
	sound_manager:PlayEvent(event, game_object)
end

function SetPropTunel()
	--PlaySound('Play_Tunel','Iris')
	--cinematic_manager:Execute("cinematica_1")
	g_bFootstepType = "metal"
end

function SetPropSala2()
	if not g_EnteredSala2 then
		--PlaySound('Play_Tunel','Iris')
		--cinematic_manager:Execute("cinematica_1")
		
		scene:SetCurrentRoomName("sala2")
		g_Player:SetRoom("sala2")
		g_Player:SetCheckpoint("sala2", Vect3f( 76.51, -10.60, -31.10 ), g_Player:GetLife(), g_Player:GetEnergy())
		
		scene:DesactivateRoom("sala1")
		
		g_bFootstepType = "pavimento"
		g_EnteredSala2 = true
		engine:Trace("Setted properties of sala2")
	end
end
	
function SetPropPasillo()
	if not g_EnteredPasillo then
		--PlaySound('Play_Tunel','Iris')
		--cinematic_manager:Execute("cinematica_1")
		
		scene:SetCurrentRoomName("pasillo")
		g_Player:SetRoom("pasillo")
		g_Player:SetCheckpoint("pasillo", Vect3f( 53.72, -16.66, -17.16 ), g_Player:GetLife(), g_Player:GetEnergy())
		
		scene:DesactivateRoom("sala2")
		
		g_EnteredPasillo = true
		engine:Trace("Setted properties of pasillo")
	end
end

function SetPropSala3()
	if not g_EnteredSala3 then
		--PlaySound('Play_Tunel','Iris')
		--cinematic_manager:Execute("cinematica_1")
		
		scene:SetCurrentRoomName("sala3")
		g_Player:SetRoom("sala3")
		g_Player:SetCheckpoint("sala3", Vect3f( 53.80, -16.23, 41.77), g_Player:GetLife(), g_Player:GetEnergy())
		
		scene:DesactivateRoom("pasillo")
		
		g_EnteredSala3 = true
		engine:Trace("Setted properties of sala3")
	end
end

function SetPropSala4()
	if not g_EnteredSala4 then
		--PlaySound('Play_Tunel','Iris')
		--cinematic_manager:Execute("cinematica_1")
		
		scene:SetCurrentRoomName("sala4")
		g_Player:SetRoom("sala4")
		g_Player:SetCheckpoint("sala4", Vect3f( -70.0, 22.0, 59.0 ), g_Player:GetLife(), g_Player:GetEnergy())
		
		g_EnteredSala4 = true
		engine:Trace("Setted properties of sala4")
	end
end

function Hacknave(nave_obj)
	if enemy_manager:GetBoss():IsStunned() then
		if not g_Hacked[nave_obj] then
			--logica de disparo hacia el boss
			engine:TraceOnce("Hacked space ship "..nave_obj)
			g_Hacked[nave_obj] = true
			enemy_manager:GetBoss():ClearStun()
			enemy_manager:GetBoss():AddDamage(25.0)
		end
	end
end

function UpToSala4(text, other_shape)
	cinematic_manager:Execute("elevatorUp")
end

function ActivateLightsSala3()
	-- activate the lights
	engine:TraceOnce("Executing trigger enemies sala3 activation")
	enemy_manager:ActivateEnemiesSala3()
end

function UpdateDLC(text_to_show)
end

function ActiveRoom(room)
	scene:ActivateRoom(room)
end

function take_C4()
end

function ActivateBoss()
end

function CreateBoss()
	g_Hacked.nave1 = false
	g_Hacked.nave2 = false
	g_Hacked.nave3 = false
	g_Hacked.nave4 = false
	enemy_manager:CreateBoss()
end