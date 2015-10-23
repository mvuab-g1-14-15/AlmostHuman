g_bChargeEnergy = false
g_bChangeRoom = false
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
		g_Player:SetWeak(false)
		enemy_manager:AlarmRoom("room2")
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
		gui_manager:ShowStaticText(text)		
	end
end

function StayText(room, message, other_shape)
	local enemigosVivos = false --GetEnemyLive(room)
	if room == "room2" then
		if g_bPressedRoom2X and enemigosVivos then
			engine:Trace("Hay enemigos vivos todavia")
			CuentaAtras = CuentaAtras - timer:GetElapsedTime()
			engine:Trace("Cuenta atras: "..CuentaAtras)
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
	elseif room == "room3" then
		if g_bPressedRoom3X and enemigosVivos then
				gui_manager:ShowStaticText(message)
				g_sMessageAlarm = message
				g_bPressedRoom3X = false
		elseif g_bPressedRoom3X then
			
			--gui_manager:ShowStaticText(message)
			g_bPressedRoom3X = false
			g_bPressRoom3X = false
			g_bOpenDoor3 = true
			cinematic_manager:Execute("elevator")
			physic_manager:ReleasePhysicActor(physic_manager:GetActor("sala3DoorEscenario"))
			--Codigo para cambiar de sala o abrir la puerta
			
		end
	end
end

function StayText(room, other_shape)
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