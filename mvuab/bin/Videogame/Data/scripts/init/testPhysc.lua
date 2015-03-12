local cinematic=nil
local g_Pi = 3.141618
local g_HalfPi = 3.141618 * 0.5
local g_ForwardSpeed = 5
local g_StrafeSpeed = 6
local g_Speed = 5
local g_Flag_agacharse = 0
local g_Levantado = 1
initialized = false

function init()
	core = Singleton_Core.get_singleton()
	action_manager = core:GetActionManager()
	graphics_manager = core:GetGraphicsManager()
	--renderable_objects_manager = core:GetRenderableObjectsManager()
	camera_manager = core:GetCameraManager()
	light_manager = core:GetLightManager()
	--cinematic=renderable_objects_manager:CreateCinematic("Data/cinematic.xml")
	--cinematic:Stop()
	timer = core:GetTimer()
	pos = Vect3f(0, 0, 0)
	physic_manager = core:GetPhysicsManager()
	--Create PlayerController
	process = Singleton_Engine.get_singleton():GetProcess()
	local PUD_Player = process:GetNewPUD("CharacterController")
	PUD_Player:SetColor(1,0,0,1)
	PUD_Player:SetPaint(true)
	--Sala 1
	--local position = Vect3f( 0, 2, 1)
	--Sala sigilo
	--local position = Vect3f( -0.66, 0, 17 )
	--Sala disparo
	--local position = Vect3f( 40, -15, -8)
	--Sala cadena montaje
	local position = Vect3f( 141, 35, -17 )
	--Sala hangar
	--local position = Vect3f( 104, 22, 198 )
	local PlayerController = process:GetNewController(0.4, 2, 0.2, 0.5, 0.5, PUD_Player, position, -10)
	physic_manager:AddPhysicController(PlayerController)
	camera_manager:GetCurrentCamera():SetPos(Vect3f(position.x, position.y + (PlayerController:GetHeight()/2), position.z))
	initialized = true
end

function update()
	if not initialized then
		init()
	end
	
	local dt = timer:GetElapsedTime()
	local current_camera = camera_manager:GetCurrentCamera()
	local enable = current_camera:GetEnable()
	if enable == true then
		move_player( dt )
		move_light( dt )
		--move_point_inicial( dt )
		--move_point_final ( dt )
	end
end

function move_light( dt )
	local current_light = light_manager:GetLight(0);
	local pos = current_light:GetPosition()	
	local addPos = Vect3f(0, 0, 0)
	
	if action_manager:DoAction("Left") then
		addPos.x = 1 * dt * g_Speed
	elseif action_manager:DoAction("Right") then
		addPos.x = - 1 * dt * g_Speed
	end
	if action_manager:DoAction("Backward") then
		addPos.z = 1 * dt * g_Speed
	elseif action_manager:DoAction("Forward") then
		addPos.z = -1 * dt * g_Speed
	end
	if action_manager:DoAction("MoveUp") then
		addPos.y = 1 * dt * g_Speed
	elseif action_manager:DoAction("MoveDown") then
		addPos.y = -1 * dt * g_Speed
	end
	
	current_light:SetPosition(pos+addPos)
end

function move_player( dt )
	flag_speed = 0
    forward = 0
    strafe = 0
	
	if action_manager:DoAction("Run") then
		flag_speed = 1
	end
	if action_manager:DoAction("Crouch") then
		if g_Flag_agacharse == 1 then
			g_Flag_agacharse = 0
			g_ForwardSpeed = g_ForwardSpeed*4
		else
			g_Flag_agacharse = 1
			g_Levantado = 0
			g_ForwardSpeed = g_ForwardSpeed/4
		end
	end
	if action_manager:DoAction("MoveLeft") then
		strafe = strafe + 1;
		move( flag_speed, forward, strafe, dt )
	elseif action_manager:DoAction("MoveRight") then
		strafe = strafe - 1;
		move( flag_speed, forward, strafe, dt )
	else
		move( flag_speed, 0, 0, dt )
	end
	
	if action_manager:DoAction("MoveBackward") then
		forward = forward - 1
		move( flag_speed, forward, strafe, dt )
	elseif action_manager:DoAction("MoveForward") then
		forward = forward + 1
		move( flag_speed, forward, strafe, dt )
	else
		move( flag_speed, 0, 0, dt )
	end
	
	local l_ActionManagerLuaWrapper=CActionManagerLuaWrapper()
	local value=""
	local current_camera = camera_manager:GetCurrentCamera();
	if l_ActionManagerLuaWrapper:DoAction(action_manager, "MoveYaw") then
		current_camera:AddYaw( -l_ActionManagerLuaWrapper.amount * dt * 100.0 );
	end
	local current_camera = camera_manager:GetCurrentCamera();
	if l_ActionManagerLuaWrapper:DoAction(action_manager, "MovePitch") then
		current_camera:AddPitch( -l_ActionManagerLuaWrapper.amount * dt * 100.0 );
	end
	
	local character_controller_UserData = physic_manager:GetUserData("CharacterController")
	local character_controller = character_controller_UserData:GetController()
	local Yaw = current_camera:GetYaw()
	character_controller:SetYaw(Yaw)
	if action_manager:DoAction("Jump") then
		character_controller:Jump(75)
	end
end

function move( flag_speed, forward, strafe, dt )

	local character_controller_UserData = physic_manager:GetUserData("CharacterController");
	local character_controller = character_controller_UserData:GetController();
	local current_camera = camera_manager:GetCurrentCamera();
	local Yaw = current_camera:GetYaw()
	local Pitch = current_camera:GetPitch()
	local cam_pos = current_camera:GetPos()
	
	local addPos = Vect3f(0, 0, 0)
	addPos.x =  forward * ( math.cos(Yaw) ) + strafe * (  math.cos(Yaw + g_HalfPi) )
	addPos.z =  forward * ( math.sin(Yaw) ) + strafe  * ( math.sin(Yaw + g_HalfPi) )
	if (not addPos.x == 0 or not addPos.z == 0) then
		addPos:Normalize()
	end
	
    constant = dt * g_ForwardSpeed;
	
	if flag_speed == 1 and  g_Flag_agacharse == 0 then
        constant = constant * g_Speed;
	end
	
    addPos = addPos * constant;
	character_controller:Move(addPos, dt)
	character_controller:SetYaw(Yaw)
	local position = Vect3f(character_controller:GetPosition())
	position.y = position.y + (character_controller:GetHeight()/2)
	local size = character_controller:GetHeight()
	if g_Flag_agacharse == 1 then
		position.y = position.y - 2
		size = size/2
	elseif g_Levantado == 0 then
		position.y = position.y + 2
		size = size*2
		g_Levantado = 1
		character_controller:SetPosition(position)
	end
	
	character_controller:SetHeight(size)
	current_camera:SetPos(position)
	
	
end

function move_point_inicial( dt )
	process = Singleton_Engine.get_singleton():GetProcess()
	local pointPos = process:GetPointInicial();
	
	local addPos = Vect3f(0, 0, 0)
	
	if action_manager:DoAction("Left") then
		addPos.z = 1 * dt * g_Speed
	elseif action_manager:DoAction("Right") then
		addPos.z = -1 * dt * g_Speed
	end
	if action_manager:DoAction("Backward") then
		addPos.x = - 1 * dt * g_Speed
	elseif action_manager:DoAction("Forward") then
		addPos.x = 1 * dt * g_Speed
	end
	
	process:SetPointInicial(pointPos+addPos)
end

function move_point_final( dt )
	process = Singleton_Engine.get_singleton():GetProcess()
	local pointPos = process:GetPointFinal();
	
	local addPos = Vect3f(0, 0, 0)
	
	if action_manager:DoAction("MoveLeft") then
		addPos.z = 1 * dt * g_Speed
	elseif action_manager:DoAction("MoveRight") then
		addPos.z = -1 * dt * g_Speed
	end
	if action_manager:DoAction("MoveBackward") then
		addPos.x = - 1 * dt * g_Speed
	elseif action_manager:DoAction("MoveForward") then
		addPos.x = 1 * dt * g_Speed
	end
	
	process:SetPointFinal(pointPos+addPos)
end

function render()
	local t = Mat44f()
	t:SetPos(pos)
	graphics_manager:SetTransform(t)
	--graphics_manager:DrawCube(1)
	t:SetIdentity()
	graphics_manager:SetTransform(t)
end

function cambiar_sala()
	--Sala 1
	local position = Vect3f( 0, 2, 1)
	--Sala sigilo
	--local position = Vect3f( -0.66, 0, 17 )
	--Sala disparo
	--local position = Vect3f( 40, -15, -8)
	--Sala cadena montaje
	--local position = Vect3f( 141, 35, -17 )
	--Sala hangar
	--local position = Vect3f( 104, 22, 198 )
	local character_controller_UserData = physic_manager:GetUserData("CharacterController")
	local character_controller = character_controller_UserData:GetController()
	character_controller:SetPosition(position)
	camera_manager:GetCurrentCamera():SetPos(Vect3f(position.x, position.y + (character_controller:GetHeight()/2), position.z))
end