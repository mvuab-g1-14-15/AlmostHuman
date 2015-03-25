function UpdateFree()
	dt = Singleton_Core.get_singleton():GetTimer():GetElapsedTime()
	local current_camera = camera_manager:GetCurrentCamera()
	flag_speed = 0
    forward = 0
    strafe = 0
	
	if action_manager:DoAction("Run") then
		flag_speed = 1
	end
	
	if action_manager:DoAction("MoveLeft") then
		strafe = strafe + 1;
		moveFree( flag_speed, forward, strafe, dt )
	elseif action_manager:DoAction("MoveRight") then
		strafe = strafe - 1;
		moveFree( flag_speed, forward, strafe, dt )
	else
		moveFree( flag_speed, 0, 0, dt )	 
	end
	
	if action_manager:DoAction("MoveBackward") then
		forward = forward - 1
		moveFree( flag_speed, forward, strafe, dt )	
	elseif action_manager:DoAction("MoveForward") then
		forward = forward + 1
		moveFree( flag_speed, forward, strafe, dt )	
	else
		moveFree( flag_speed, 0, 0, dt )	
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

function moveFree( flag_speed, forward, strafe, dt )
	local l_ForwardSpeed = 7

	local l_Speed = 6
	local current_camera = camera_manager:GetCurrentCamera();
	local Yaw = current_camera:GetYaw()
	local Pitch = current_camera:GetPitch()
	local cam_pos = current_camera:GetPos()
	
	local addPos = Vect3f(0, 0, 0)
	addPos.x =  forward * ( math.cos(Yaw) ) + strafe * (  math.cos(Yaw + g_HalfPi) )
	addPos.y =  forward * ( math.sin(Pitch) )
	addPos.z =  forward * ( math.sin(Yaw) ) + strafe  * ( math.sin(Yaw + g_HalfPi) )
	if (not addPos.x == 0 or not addPos.z == 0) then
		addPos:Normalize()
	end
	
    constant = dt * l_ForwardSpeed;
	
	if flag_speed == 1 and  g_bFlag_agacharse == 0 then
        constant = constant * l_Speed;
	end
	
    addPos = addPos * constant;
	current_camera:SetPos((cam_pos + addPos))
end