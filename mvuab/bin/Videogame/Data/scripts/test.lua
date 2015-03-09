local cinematic=nil
local g_Pi = 3.141618
local g_HalfPi = 3.141618 * 0.5
local g_ForwardSpeed = 5
local g_StrafeSpeed = 6
local g_Speed = 5

function init()
	core = Singleton_Core.get_singleton()
	action_manager = core:GetActionManager()
	graphics_manager = core:GetGraphicsManager()
	renderable_objects_manager = core:GetRenderableObjectsManager()
	camera_manager = core:GetCameraManager()
	light_manager = core:GetLightManager()
	cinematic=renderable_objects_manager:CreateCinematic("Data/cinematic.xml")
	cinematic:Stop()
	timer = core:GetTimer()
	pos = Vect3f(0, 0, 0)
	
	initialized = true
end

function update()
	if not initialized then
		init()
	end
	
	local dt = timer:GetElapsedTime()
	local current_camera = camera_manager:GetCurrentCamera();
	local enable = current_camera:GetEnable();
	if enable == true then
		move_player( dt )
		move_light( dt )
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
	if action_manager:DoAction("MoveLeft") then
		strafe = strafe + 1;
		move( flag_speed, forward, strafe, dt )
	elseif action_manager:DoAction("MoveRight") then
		strafe = strafe - 1;
		move( flag_speed, forward, strafe, dt )
	end
	if action_manager:DoAction("MoveBackward") then
		forward = forward - 1
		move( flag_speed, forward, strafe, dt )
	elseif action_manager:DoAction("MoveForward") then
		forward = forward + 1
		move( flag_speed, forward, strafe, dt )
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
end

function move( flag_speed, forward, strafe, dt )
	local current_camera = camera_manager:GetCurrentCamera();
	local Yaw = current_camera:GetYaw()
	local Pitch = current_camera:GetPitch()
	local cam_pos = current_camera:GetPos()
	
	local addPos = Vect3f(0, 0, 0)
	addPos.x =  forward * ( math.cos(Yaw) ) + strafe * (  math.cos(Yaw + g_HalfPi) )
	addPos.z =  forward * ( math.sin(Yaw) ) + strafe  * ( math.sin(Yaw + g_HalfPi) )
	addPos:Normalize()
	
    constant = dt * g_ForwardSpeed;
	
	if flag_speed == 1 then
        constant = constant * g_Speed;
	end
	
    addPos = addPos * constant;
	current_camera:SetPos((cam_pos + addPos))
	
end

function render()
	local t = Mat44f()
	t:SetPos(pos)
	graphics_manager:SetTransform(t)
	--graphics_manager:DrawCube(1)
	t:SetIdentity()
	graphics_manager:SetTransform(t)
end