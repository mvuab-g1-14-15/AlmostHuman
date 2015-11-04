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

g_C4Taken = false
g_InsideTakeC4 = false

g_InsideDoor2 = false
g_BlockTextActive = false

g_InsideDoorPasillo = false

g_InsidePuntoExplosivo1 = false
g_InsidePuntoExplosivo2 = false

g_C4ColocadaText = false

g_InsidePuntoDetonacion = false

g_C4Colocada1 = false
g_C4Colocada2 = false

g_FaltanC4Text = false

g_ExplotionDone = false

g_InsideElevator = false

g_InsideUpdateDLC = false

g_InsideHackedNave = false
g_NaveToHack = ""

g_HackedText = false

g_FaltanC4Text = false

g_TakeElevatorText = false

g_InsideBaseCargaError = false

g_BaseCargaErrorText = false

g_Door2Unblocked = false

g_DetonarC4Text = false

g_BrazoOperativo = false

g_CargandoEnergia = false

g_SpeechIrisBoss = false

g_BossInAction = false

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

function Door2_enter()
	gui_manager:ShowStaticText("OpenDoor")
	g_InsideDoor2 = true
end

function Door2_exit()
	if g_BlockTextActive then
		gui_manager:ShowStaticText("Block")
		g_BlockTextActive = false
	end
	gui_manager:ShowStaticText("OpenDoor")
	g_InsideDoor2 = false
end

function DoorPasillo_enter()
	gui_manager:ShowStaticText("OpenDoor")
	g_InsideDoorPasillo = true
end

function DoorPasillo_exit()
	if g_BlockTextActive then
		gui_manager:ShowStaticText("Block")
		g_BlockTextActive = false
	end
	gui_manager:ShowStaticText("OpenDoor")
	g_InsideDoorPasillo = false
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
	--engine:Trace("Sala  "..room)
	--engine:Trace("Hay estos enemigos vivos "..enemigosVivos)
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
		enemy_manager:ActivateEnemiesSala3()
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
	----engine:Trace("La room es "..room)
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
	----engine:Trace(message)
end

function ShowTextDoor1(message, other_shape)
	if g_bPressRoom1X then
		g_bPressRoom1X = false	
		if g_bBlockShow then
			g_bBlockShow = false
			gui_manager:ShowStaticText("Block")
			sound_manager:PlayEvent("Play_GP_Puerta_Bloqueada", "Logan")
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
		g_Player:SetCheckpoint("sala2", Vect3f( 76.51, -10.60, -31.10 ), g_Player:GetLife(), g_Player:GetEnergy(), g_Player:GetYaw())
		
		scene:DesactivateRoom("sala1")
		
		g_bFootstepType = "pavimento"
		g_EnteredSala2 = true
		--engine:Trace("Setted properties of sala2")
	end
end
	
function SetPropPasillo()
	if not g_EnteredPasillo then
		--PlaySound('Play_Tunel','Iris')
		--cinematic_manager:Execute("cinematica_1")
		--gui_manager:SetCurrentMap("Radar2")
		g_HUD:ChangeCurrentMap()
		scene:SetCurrentRoomName("pasillo")
		g_Player:SetRoom("pasillo")
		g_Player:SetCheckpoint("pasillo", Vect3f( 53.72, -16.66, -17.16 ), g_Player:GetLife(), g_Player:GetEnergy(), g_Player:GetYaw())
		cinematic_manager:Execute("CloseDoor")
		--physic_manager:GetActor("sala2DoorEscenario"):SetPosition(Vect3f(0, 0, 0))
		scene:DesactivateRoom("sala2")
		
		sound_manager:PlayEvent("Play_Ambient_Pasillo", "Ambient")
		
		g_EnteredPasillo = true
		--engine:Trace("Setted properties of pasillo")
	end
end

function SetPropSala3()
	if not g_EnteredSala3 then
		--PlaySound('Play_Tunel','Iris')
		--cinematic_manager:Execute("cinematica_1")
		--gui_manager:SetCurrentMap("Radar3")
		g_HUD:ChangeCurrentMap()
		scene:SetCurrentRoomName("sala3")
		g_Player:SetRoom("sala3")
		g_Player:SetCheckpoint("sala3", Vect3f( 53.80, -16.23, 41.77), g_Player:GetLife(), g_Player:GetEnergy(), g_Player:GetYaw())
		
		sound_manager:PlayEvent("Play_Sala3A", "Logan")
		cinematic_manager:Execute("CloseDoorPasillo")
		
		sound_manager:PlayEvent("Play_Ambient_Sala3", "Ambient")
		
		light_manager:GetResource("Luz_activacion_01"):ChangeVisibility(false)
		light_manager:GetResource("Luz_activacion_02"):ChangeVisibility(false)
		light_manager:GetResource("Luz_activacion_03"):ChangeVisibility(false)
		
		g_EnteredSala3 = true
		--engine:Trace("Setted properties of sala3")
	end
end

function SetPropSala4()
	if not g_EnteredSala4 then
		--PlaySound('Play_Tunel','Iris')
		--cinematic_manager:Execute("cinematica_1")
		--gui_manager:SetCurrentMap("Radar4")
		g_HUD:ChangeCurrentMap()
		scene:SetCurrentRoomName("sala4")
		g_Player:SetRoom("sala4")
		g_Player:SetCheckpoint("sala4", Vect3f( -70.0, 22.0, 59.0 ), g_Player:GetLife(), g_Player:GetEnergy(), g_Player:GetYaw())
		
		scene:DesactivateRoom("pasillo")
		scene:DesactivateRoom("sala3")
		
		camera_manager:GetCamera("Main"):SetZFar(600.0)
		camera_manager:GetCamera("FreeCam"):SetZFar(600.0)
		
		g_EnteredSala4 = true
		--engine:Trace("Setted properties of sala4")
	end
end

function Hacknave_enter(nave_obj)
	g_InsideHackedNave = true
	g_NaveToHack = nave_obj
	if not g_HackedText then
		lBoss = enemy_manager:GetBoss()
		if lBoss ~= nil then
			if lBoss:IsStunned() then
				g_HackedText = true
				gui_manager:ShowStaticText("Hackear")
			end
		end
	end
end

function Hacknave_exit(nave_obj)
	g_InsideHackedNave = false
	if g_HackedText then
		gui_manager:ShowStaticText("Hackear")
		g_HackedText = false
	end
end

function UpToSala4()
	cinematic_manager:Execute("elevatorUp")
	scene:ActivateRoom("sala4")
end

function ActivateLightsSala3()
	light_manager:GetResource("Luz_activacion_01"):ChangeVisibility(true)
	light_manager:GetResource("Luz_activacion_02"):ChangeVisibility(true)
	light_manager:GetResource("Luz_activacion_03"):ChangeVisibility(true)
end

function UpdateDLC(text_to_show)
	g_Player:SetEnergy(100.0)
	sound_manager:PlayEvent("Play_Sala2", "Logan")
	g_Player:SetAnimation("stealth")
	g_Door2Unblocked = true
end

function ActiveRoom(room)
	scene:ActivateRoom(room)
	
	if room == "sala2" then
		enemy_manager:CreateEnemiesSala2()
	end
	if room == "sala3" then
		enemy_manager:CreateEnemiesSala3()
		enemy_manager:CreateDesactivateEnemiesSala3()
	end
end

function take_C4_enter()
	g_InsideTakeC4 = true
	gui_manager:ShowStaticText("TakeC4")
	g_TakeC4Text = true
end

function take_C4_exit()
	g_InsideTakeC4 = false
	if g_TakeC4Text then
		gui_manager:ShowStaticText("TakeC4")
		g_TakeC4Text = false
	end
end

function ActivateBoss()
end

function CreateBoss_enter()
	g_Hacked.nave1 = false
	g_Hacked.nave2 = false
	g_Hacked.nave3 = false
	g_Hacked.nave4 = false
	enemy_manager:CreateBoss()
	g_Player:SetCheckpoint("sala4", Vect3f( -70.0, 22.0, 59.0 ), g_Player:GetLife(), g_Player:GetEnergy(), g_Player:GetYaw())
end

function CreateBoss_exit()
	if not g_SpeechIrisBoss then
		trigger_manager:GetTriggerByName("final"):SetActive(false)
		sound_manager:PlayEvent("Play_Sala4A", "Logan")
		g_SpeechIrisBoss = true
	end
	if not g_BossInAction then
		g_Player:SetCanMove(false)
	end
end

function PuntoExplosivo1_enter()
	g_InsidePuntoExplosivo1 = true
	gui_manager:ShowStaticText("ColocarC4")
end

function PuntoExplosivo1_exit()
	g_InsidePuntoExplosivo1 = false
	gui_manager:ShowStaticText("ColocarC4")
	if g_C4ColocadaText then
		gui_manager:ShowStaticText("C4Colocada")
		g_C4ColocadaText = false
	end
	if g_C4Colocada1 then
		trigger_manager:GetTriggerByName("punto_explosivo_1_sala3"):SetActive(false)
	end
end

function PuntoExplosivo2_enter()
	g_InsidePuntoExplosivo2 = true
	gui_manager:ShowStaticText("ColocarC4")
end

function PuntoExplosivo2_exit()
	g_InsidePuntoExplosivo2 = false
	gui_manager:ShowStaticText("ColocarC4")
	if g_C4ColocadaText then
		gui_manager:ShowStaticText("C4Colocada")
		g_C4ColocadaText = false
	end
	if g_C4Colocada2 then
		trigger_manager:GetTriggerByName("punto_explosivo_2_sala3"):SetActive(false)
	end
end

function PuntoDetonacion_enter()
	gui_manager:ShowStaticText("DetonarC4")
	g_InsidePuntoDetonacion = true
end

function PuntoDetonacion_exit()
	gui_manager:ShowStaticText("DetonarC4")
	g_InsidePuntoDetonacion = false
	if g_FaltanC4Text then
		gui_manager:ShowStaticText("FaltanC4")
		g_FaltanC4Text = false
	end
end

function Elevator_enter()
	g_InsideElevator = true
	gui_manager:ShowStaticText("TakeElevator")
	g_TakeElevatorText = true
	if not g_ExplotionDone then
		gui_manager:ShowStaticText("C4NoDetonado")
		g_DetonarC4Text = true
	end
end

function Elevator_exit()
	g_InsideElevator = false
	if g_TakeElevatorText then
		gui_manager:ShowStaticText("TakeElevator")
		g_TakeElevatorText = false
	end
	if g_DetonarC4Text then
		g_DetonarC4Text = false
		gui_manager:ShowStaticText("C4NoDetonado") -- si no se ha detonado el C4
	end
end

function UpdateDLC_enter()
	g_InsideUpdateDLC = true
	gui_manager:ShowStaticText("UpdateDLC")
	g_UpdateDLCText = true
end

function UpdateDLC_exit()
	g_InsideUpdateDLC = false
	if g_UpdateDLCText then
		gui_manager:ShowStaticText("UpdateDLC")
		g_UpdateDLCText = false
	end
end

function Final_enter()
	scene:ActivateRoom("space")
    scene:SetCurrentRoomName("space")
	cinematic_manager:Execute("FinalGame")
	sound_manager:PlayEvent("Stop_Musica_Boss", "Ambient")
	sound_manager:PlayEvent("Play_Sala4B", "Logan")
	trigger_manager:GetTriggerByName("final"):SetActive(false)
end

function CargarEnergia_enter()
	if g_BrazoOperativo then
		gui_manager:ShowStaticText("CargarEnergia")
		g_CargarEnergiaText = true
		g_InsideCargarEnergia = true
	end
end

function CargarEnergia_exit()
	if g_BrazoOperativo then
		g_InsideCargarEnergia = false
		if g_CargarEnergiaText then
			gui_manager:ShowStaticText("CargarEnergia")
			g_InsideCargarEnergia = false
			countdowntimer_manager:Reset("BaseDeCargaTimer", false)
		end
		g_CargandoEnergia = false
	end
end

function BaseCargaError_enter()
	g_InsideBaseCargaError = true
	--texto carga
end

function BaseCargaError_exit()
	g_InsideBaseCargaError = false
	--quitar texto carga
	if g_BaseCargaErrorText then
		gui_manager:ShowStaticText("BaseCargaError")
		g_BaseCargaErrorText = false
	end
end

function UpdateTriggers()
	if g_CargandoEnergia then
		if not countdowntimer_manager:ExistTimer("BaseDeCargaTimer") then
			countdowntimer_manager:AddTimer("BaseDeCargaTimer", 0.25, false)
		else
			countdowntimer_manager:SetActive("BaseDeCargaTimer", true)
		end
		if countdowntimer_manager:isTimerFinish("BaseDeCargaTimer") then
			g_Player:AddEnergy(10.0)
			g_Player:AddLife(10.0)
			countdowntimer_manager:Reset("BaseDeCargaTimer", false)
		end
	end
	if g_InsideCargarEnergia then
		if action_manager:DoAction("Action") then
			if g_CargarEnergiaText then
				gui_manager:ShowStaticText("CargarEnergia")
				g_CargarEnergiaText = false
			end
			g_CargandoEnergia = true
		end
	end
	if g_InsideBaseCargaError then
		if action_manager:DoAction("Action") then
			gui_manager:ShowStaticText("BaseCargaError")
			g_BaseCargaErrorText = true
		end
	end
	if g_InsideTakeC4 then
		if action_manager:DoAction("Action") then
			g_C4Taken = true
			sound_manager:PlayEvent("Play_Pasillo", "Logan")
			enemy_manager:CreateEnemiesPasillo()
			take_C4_exit()
			trigger_manager:GetTriggerByName("take_C4"):SetActive(false)
			local lBombMesh = scene:GetResource("pasillo"):GetLayer("solid"):GetResource("bomb_pasillo")
			if lBombMesh ~= nil then
				lBombMesh:SetActive(false)
			end
		end
	end
	
	if g_InsideDoor2 then
		if action_manager:DoAction("Action") then
			if not g_BrazoOperativo then
				if not g_BlockTextActive then
					gui_manager:ShowStaticText("Block")
					g_BlockTextActive = true
				end
			else
				Door2_exit()
				scene:ActivateRoom("pasillo")
				trigger_manager:GetTriggerByName("puerta_sala2"):SetActive(false)
				cinematic_manager:Execute("OpenDoor")
				sound_manager:PlayEvent("Open_Close_Door_Event", "Door_sala2")
            end
		end
	end
	
	if g_InsideDoorPasillo then
		if action_manager:DoAction("Action") then
			if not g_C4Taken then
				if not g_BlockTextActive then
					gui_manager:ShowStaticText("Block")
					g_BlockTextActive = true
				end
			else
				DoorPasillo_exit()
				scene:ActivateRoom("sala3")
				scene:ActivateRoom("elevator")
				enemy_manager:CreateEnemiesSala3()
				enemy_manager:CreateDesactivateEnemiesSala3()
				trigger_manager:GetTriggerByName("door_pasillo_to_sala3"):SetActive(false)
				cinematic_manager:Execute("OpenDoorPasillo")
				sound_manager:PlayEvent("Open_Close_Door_Event", "Door_pasillo")
			end
		end
	end
	
	if g_InsidePuntoExplosivo1 then
		if action_manager:DoAction("Action") then
			gui_manager:ShowStaticText("C4Colocada")
			g_C4Colocada1 = true
			g_C4ColocadaText = true
			scene:GetResource("sala3"):GetLayer("glow"):GetResource("bomb_sala3"):SetActive(false)
			scene:GetResource("sala3"):GetLayer("solid"):GetResource("bomb_sala3"):SetActive(true)
			sound_manager:PlayEvent("Play_Plant_C4","C4_1")
		end
	end
	
	if g_InsidePuntoExplosivo2 then
		if action_manager:DoAction("Action") then
			gui_manager:ShowStaticText("C4Colocada")
			g_C4Colocada2 = true
			g_C4ColocadaText = true
			scene:GetResource("sala3"):GetLayer("glow"):GetResource("bomb002_sala3"):SetActive(false)
			scene:GetResource("sala3"):GetLayer("solid"):GetResource("bomb002_sala3"):SetActive(true)
			sound_manager:PlayEvent("Play_Plant_C4","C4_2")
		end
	end
	
	if g_InsidePuntoDetonacion then
		if action_manager:DoAction("Action") then
			if g_C4Colocada1 and g_C4Colocada2 then
				PuntoDetonacion_exit()
				cinematic_manager:Execute("explotion")
				sound_manager:PlayEvent("Play_Sala3B", "Logan")
				trigger_manager:GetTriggerByName("punto_detonacion"):SetActive(false)
				g_ExplotionDone = true
				g_Player:SetCheckpoint("sala3", Vect3f( -7.0, -14.14, 60.05 ), g_Player:GetLife(), g_Player:GetEnergy(), g_Player:GetYaw())
			else
				if not g_FaltanC4Text then
					g_FaltanC4Text = true
					gui_manager:ShowStaticText("FaltanC4")
				end
			end
		end
	end
	
	if g_InsideElevator then
		if g_ExplotionDone then
			if action_manager:DoAction("Action") then
				Elevator_exit()
				cinematic_manager:Execute("elevatorDown")
				trigger_manager:GetTriggerByName("elevator_sala3"):SetActive(false)	
				enemy_manager:ActivateEnemiesSala3()
				enemy_manager:SetAlarm("sala3")
			end
		end
	end
	
	if g_InsideUpdateDLC then
		if action_manager:DoAction("Action") then
			UpdateDLC_exit()
			trigger_manager:GetTriggerByName("base_DLC_sala2"):SetActive(false)
			sound_manager:PlayEvent("Play_Sala2", "Logan")
			g_Player:SetEnergy(100.0)
			g_Player:SetCheckpoint("sala2", Vect3f(75.66, -16.77, -29.67), g_Player:GetLife(), g_Player:GetEnergy(), g_Player:GetYaw())
			g_BrazoOperativo = true
		end
	end
	
	if g_InsideHackedNave then
		if action_manager:DoAction("Action") then
			lBoss = enemy_manager:GetBoss()
			if lBoss ~= nil then
				if lBoss:IsStunned() then
					if not g_Hacked[g_NaveToHack] then
						g_Hacked[g_NaveToHack] = true
						lBoss:ClearStun()
						lBoss:AddDamage(25.0)
						lBoss:SpawnEnemies()
						lBoss:SetAttack("far")
						g_Player:SetEnergy(100.0)
						Hacknave_exit(g_NaveToHack)
						if g_NaveToHack == "nave1" then
							trigger_manager:GetTriggerByName("Hack_nave1"):SetActive(false)
						end
						if g_NaveToHack == "nave2" then
							trigger_manager:GetTriggerByName("Hack_nave2"):SetActive(false)
						end
						if g_NaveToHack == "nave3" then
							trigger_manager:GetTriggerByName("Hack_nave3"):SetActive(false)
						end
						if g_NaveToHack == "nave4" then
							trigger_manager:GetTriggerByName("Hack_nave4"):SetActive(false)
						end
						g_NaveToHack = ""
					end
				end
			end
		end
	end
end