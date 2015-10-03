g_bChargeEnergy = false
g_bChangeRoom = false
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
		g_EnemyManager:AlarmRoom("room2")
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
	engine:Trace("He entrado en ShowText")
	if g_bPressRoom2X then
		engine:Trace("He entrado en PressRoom2X = true")
		g_bPressRoom2X = false
		if CuentaAtras ~= 3 then
			CuentaAtras = 0
			gui_manager:ShowStaticText("Block")
		end
	else
		engine:Trace("He entrado en PressRoom2X = false")
		g_bPressRoom2X = true
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
	if g_bPressedE == false and g_bC41 then
		g_bPressE = false
		g_bPressedE = true
		gui_manager:ShowStaticText(textC4Colocada)
	elseif g_bPressedE == false and g_bC42 then
		g_bPressE = false	
		g_bPressedE = true
		gui_manager:ShowStaticText(textC4Colocada)
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

function CheckC4(num, textC4Colocada, text2, other_shape)
	
	if g_bPressedE then
		gui_manager:ShowStaticText(num)
		gui_manager:ShowStaticText(textC4Colocada)
		g_bPressedE = false
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

function StayText(room, message, other_shape)
	local enemigosVivos = GetEnemyLive(room)
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
				CuentaAtras = 3
				g_bPressedRoom3X = false
		elseif g_bPressedRoom3X then
			
			gui_manager:ShowStaticText(message)
			g_bPressedRoom3X = false
			g_bPressRoom3X = false
			g_bOpenDoor3 = true
			--Codigo para cambiar de sala o abrir la puerta
			
		end
	end
end

function GetEnemyLive(room)
	lEnemy = g_EnemyManager:GetEnemys()
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