dofile("./data/scripts/Include.lua")

class "CPlayerController"

function CPlayerController:__init()
	self.Position = Vect3f(136.84, -7.99, -62)
	self.Height = 1.8
	self.Position.y = self.Position.y + self.Height/2.0
	self.Radius = 0.4
	self.Direction = Vect3f(0.0)
	self.Speed = 4.0
	self.JumpForce = 180.0
	
	self.Forward = Vect3f(0.0)
	self.Side = Vect3f(0.0)
	
	self.InitialYaw = 0.0
	self.Yaw = 0.0
	
	--States
	self.Run = false
	self.Crouch = false
	self.Shooting = false
	self.LeanOut = 0
	self.PrevLeanOut = 0
	self.YawMoved = false
	self.IsMoving = false
	
	--Timers
	self.TimeCrouch = 0.2
	self.ActualTimeCrouch = 0.2
	
	self.TimeLeanOut = 0.15
	self.ActualTimeLeanOut = 0.15
	
	self.TimeMoveYaw = 0.15
	self.ActualTimeMoveYaw= 0.15
	
	self.TimeFootstep = 0.5
	countdowntimer_manager:AddTimer("Footstep", self.TimeFootstep, false)
	
	self.TimeDetectionMoving = 1.0
	
	--Counters
	self.ShakeValueVertical = 0.0
	self.ShakeValueHorizontal = 0.0
	
	self.ShakeVerticalSpeed = 4.0
	self.ShakeHorizontalSpeed = 3.0
	self.ShakeVerticalAmplitude = 0.01
	self.ShakeHorizontalAmplitude = 0.01
	
	self.Weak = true
	
	physic_manager:AddController("Player", self.Radius, self.Height/2.0, math.cos(g_HalfPi/2.0), 0.01, 0.5, self.Position, CollisionGroup.ECG_PLAYER.value, -490)
	self.CharacterController = physic_manager:GetController("Player")
	--engine:Trace("Player Controller initialized")
end

function CPlayerController:Update()
	local dt = timer:GetElapsedTime()

	--local l_PlayerCamera = camera_manager:GetCamera("TestProcessCam")
	local l_PlayerCamera = camera_manager:GetCurrentCamera()
	self:CalculateDirectionVectors(l_PlayerCamera)
	self:UpdateCamera(l_PlayerCamera, dt)
	
	--Yaw smooth movement
	local l_CameraYaw = l_PlayerCamera:GetYaw()
	self.Yaw = l_CameraYaw
	local l_Yaw = self.CharacterController:GetYaw()
	
	
	if l_CameraYaw == l_Yaw then
		self.YawMoved = false
	end
	if self.YawMoved then
		local l_Percentage = self.ActualTimeMoveYaw / self.TimeMoveYaw
		local l_Initial = l_Yaw
		local l_Final = l_CameraYaw
		local l_ActualYaw = (1.0 - l_Percentage) * l_Initial + l_Percentage * l_Final
		self.CharacterController:SetYaw(l_ActualYaw)
		self.ActualTimeMoveYaw = self.ActualTimeMoveYaw + dt
		if self.ActualTimeMoveYaw > self.TimeMoveYaw then
			self.ActualTimeMoveYaw = self.TimeMoveYaw
		end
	end
	
	self:UpdateInput()
	
	self:UpdateTimers(dt)
	
	--Is Player moving?
	if CheckVector(self.Direction) then
		if not countdowntimer_manager:ExistTimer("PlayerMoving") then
			countdowntimer_manager:AddTimer("PlayerMoving", self.TimeDetectionMoving, false)
		else
			countdowntimer_manager:SetActive("PlayerMoving", true)
		end
		if countdowntimer_manager:isTimerFinish("PlayerMoving") then
			self.IsMoving = true
		end
	else
		countdowntimer_manager:Reset("PlayerMoving", false)
		self.IsMoving = false
	end

	--Set Listenr Postion 
	sound_manager:SetListenerPosition(self:GetPosition(),l_PlayerCamera:GetDirection(),l_PlayerCamera:GetVecUp());
	local l_Speed = self.Speed
	if self.Crouch then
		l_Speed = l_Speed / 3.0
		countdowntimer_manager:AddTimer("Footstep", self.TimeFootstep * 2.0, false)
	end
	if self.Run then
		l_Speed = l_Speed * 2.0
		countdowntimer_manager:ChangeTotalTime("Footstep", self.TimeFootstep * 0.8)
	end
	if not self.Crouch and not self.Run then
		countdowntimer_manager:ChangeTotalTime("Footstep", self.TimeFootstep)
	end
	
	local l_Velocity = self.Direction * l_Speed
	
	if self.Jump then
		self.CharacterController:Jump(self.JumpForce)
		self.Jump = false
	end
	self.CharacterController:Move(l_Velocity, dt)
	self.Position = self.CharacterController:GetPosition()
	
	local l_CameraPosition = self.Position
	
	--Crouch movement
	local l_Percentage = self.ActualTimeCrouch / self.TimeCrouch
	local l_Initial = 0
	local l_Final = l_CameraPosition.y
	if self.Crouch then
		l_Initial = l_CameraPosition.y + self.Height/2.0
		l_Final = l_CameraPosition.y
	else
		l_Initial = l_CameraPosition.y
		l_Final = l_CameraPosition.y + self.Height/2.0
	end
	l_CameraPosition.y = (1.0 - l_Percentage) * l_Initial + l_Percentage * l_Final
	
	--Lean out movement
	l_Percentage = self.ActualTimeLeanOut / self.TimeLeanOut
	l_Initial = 0
	l_Final = l_CameraPosition
	if self.LeanOut ~= 0 then
		l_Initial = l_CameraPosition
		l_Final = l_CameraPosition + self.LeanOut * self.Side
	else
		l_Initial = l_CameraPosition + self.PrevLeanOut * self.Side
		l_Final = l_CameraPosition
	end
	l_LeanOutPosition = (1.0 - l_Percentage) * l_Initial + l_Percentage * l_Final

	if not OverlapSphere(0.5, l_Final) then
		l_CameraPosition = l_LeanOutPosition
	else
		self.LeanOut = 0
		self.ActualTimeLeanOut = self.TimeLeanOut
	end
	
	if not self.CharacterController:IsJumping() then
		local l_Up = l_PlayerCamera:GetVecUp()
		
		local l_ShakeVerticalSpeed = self.ShakeHorizontalSpeed
		local l_ShakeHorizontalSpeed = self.ShakeHorizontalSpeed
		local l_ShakeVerticalAmplitude = self.ShakeVerticalAmplitude
		local l_ShakeHorizontalAmplitude = self.ShakeHorizontalAmplitude	
		if CheckVector(self.Direction) then
			l_ShakeVerticalAmplitude = l_ShakeVerticalAmplitude * 5.0
			l_ShakeVerticalSpeed = l_ShakeVerticalSpeed * 3.0
			if self.Crouch then
				l_ShakeVerticalSpeed = l_ShakeVerticalSpeed / 2.0
				l_ShakeHorizontalSpeed = l_ShakeHorizontalSpeed / 2.0
				l_ShakeVerticalAmplitude = l_ShakeVerticalAmplitude / 2.0
				l_ShakeHorizontalAmplitude = l_ShakeHorizontalAmplitude / 2.0
			end
			if self.Run then
				l_ShakeVerticalSpeed = l_ShakeVerticalSpeed * 2.0
				l_ShakeHorizontalSpeed = l_ShakeHorizontalSpeed * 2.0
				l_ShakeVerticalAmplitude = l_ShakeVerticalAmplitude * 2.0
				l_ShakeHorizontalAmplitude = l_ShakeHorizontalAmplitude * 2.0
			end
		end
		
		self.ShakeValueVertical = self.ShakeValueVertical + l_ShakeVerticalSpeed * dt
		self.ShakeValueHorizontal = self.ShakeValueHorizontal + l_ShakeHorizontalSpeed * dt
		l_CameraPosition = l_CameraPosition + l_ShakeVerticalAmplitude * l_Up * math.sin(self.ShakeValueVertical) + l_ShakeHorizontalAmplitude * self.Side * math.cos(self.ShakeValueHorizontal)
	end
	
	--Move the camera outside de capsule of the controller
	l_CameraPosition = l_CameraPosition + self.Forward * self.Radius
	
	--l_CameraPosition = l_CameraPosition - self.Forward
	l_PlayerCamera:SetPosition(l_CameraPosition)
	
	self:MakeGizmo()
end

function CPlayerController:MakeGizmo()
	local Gizmo = gizmos_manager:GetResource("PlayerGizmo")
	if Gizmo == nil then
		Gizmo = gizmos_manager:CreateGizmo("PlayerGizmo", self.Position, 0.0, 0.0)
		local GizmoElement = gizmos_manager:CreateGizmoElement(1, 0.05, Vect3f(0.0), 0.0, 0.0, CColor(0.0, 0.0, 1.0, 1.0))
		Gizmo:AddElement(GizmoElement)
		gizmos_manager:AddResource("PlayerGizmo", Gizmo)
	else
		Gizmo:SetPosition(self.Position)
	end
	Gizmo:MakeTransform()
end

function CPlayerController:SetPosition(position)
	self.Position = position
	self.CharacterController:SetPosition(position)
end

function CPlayerController:GetPosition()
	return self.Position
end

function CPlayerController:GetHeight()
	return self.CharacterController:GetHeight()
end

function CPlayerController:GetYaw()
	return self.Yaw
end

function CPlayerController:UpdateTimers(dt)
	if self.ActualTimeCrouch < self.TimeCrouch then
		self.ActualTimeCrouch = self.ActualTimeCrouch + dt
	else
		self.ActualTimeCrouch = self.TimeCrouch
	end
	if self.ActualTimeLeanOut < self.TimeLeanOut then
		self.ActualTimeLeanOut = self.ActualTimeLeanOut + dt
	else
		self.ActualTimeLeanOut = self.TimeLeanOut
	end
end

function CPlayerController:UpdateCamera(camera, dt)
	local delta = 0.1
	if not g_ConsoleActivate and not g_CinematicActive then
		if action_manager_lua_wrapper:DoAction(action_manager, "MoveYaw") then
			camera:AddYaw( -action_manager_lua_wrapper.amount * dt * g_CameraSensibility );
			if not self.YawMoved and math.abs(action_manager_lua_wrapper.amount) < 2 then
				self.YawMoved = true
				self.ActualTimeMoveYaw = 0.0
			else
				self.InitialYaw = camera:GetYaw()
			end
		end
		if action_manager_lua_wrapper:DoAction(action_manager, "MovePitch") then
			camera:AddPitch( -action_manager_lua_wrapper.amount * dt * g_CameraSensibility );
			if camera:GetPitch() > g_HalfPi - delta then
				camera:SetPitch(g_HalfPi - delta)
			end
			if camera:GetPitch() < -g_HalfPi + delta then
				camera:SetPitch(-g_HalfPi + delta)
			end 
		end
	end
end

function CPlayerController:CalculateDirectionVectors(l_PlayerCamera)
	self.Forward = l_PlayerCamera:GetDirection()
	self.Forward.y = 0.0
	self.Forward:Normalize()
	local l_Up = Vect3f(0.0, 1.0, 0.0)
	self.Side = self.Forward ^ l_Up
	self.Side.y = 0.0
	self.Side:Normalize()
end

function CPlayerController:PlayFootstep()
	if countdowntimer_manager:isTimerFinish("Footstep") and not self.CharacterController:IsJumping() then
		--if trigger pavimento
		if (g_bFootstepType == "metal") then
			sound_manager:PlayEvent( "Logan_Footstep_Walk_Metal", "Logan")
		end
		if (g_bFootstepType == "pavimento") then
			sound_manager:PlayEvent( "Logan_Footstep_Walk_Pavimento", "Logan" )
		end
		-- if trigger metal
		--sound_manager:PlayEvent( "Logan_Footstep_Walk_Metal", "Logan" )
		countdowntimer_manager:Reset("Footstep", true)
	end
end

function CPlayerController:UpdateInput()
	self.Direction = Vect3f(0.0)
	if not g_ConsoleActivate then
		if action_manager:DoAction("MoveForward") then
			self.Direction = self.Direction + self.Forward
			self:PlayFootstep()
		end
		if action_manager:DoAction("MoveBackward") then
			self.Direction = self.Direction - self.Forward
			self:PlayFootstep()
		end
		if action_manager:DoAction("MoveLeft") then
			self.Direction = self.Direction + self.Side
			self:PlayFootstep()
		end
		if action_manager:DoAction("MoveRight") then
			self.Direction = self.Direction - self.Side
			self:PlayFootstep()
		end
		if CheckVector(self.Direction) then
			self.Direction:Normalize()
		end
		if action_manager:DoAction("ShootDown") then
			self.Shooting = true
		end
		if action_manager:DoAction("ShootUp") then
			self.Shooting = false
		end
		if not self.CharacterController:IsJumping() then
			if action_manager:DoAction("Run") then
				self.Run = true
			else
				self.Run = false
			end
			if action_manager:DoAction("Crouch") then
				self:MakeCrouch()
			end
			if action_manager:DoAction("Jump") then
				self.Jump = true
			end
		end
		if self.LeanOut == 0 and self.ActualTimeLeanOut == self.TimeLeanOut then
			if action_manager:DoAction("LeanOutLeft") then
				self.LeanOut = 1
				self.ActualTimeLeanOut = 0.01
			end
			if action_manager:DoAction("LeanOutRight") then
				self.LeanOut = -1
				self.ActualTimeLeanOut = 0.0
			end
		end
		if action_manager:DoAction("LeanOutLeftUp") and self.LeanOut == 1 then
			self.PrevLeanOut = self.LeanOut
			self.LeanOut = 0
			self.ActualTimeLeanOut = 0.0
		end
		if action_manager:DoAction("LeanOutRightUp") and self.LeanOut == -1 then
			self.PrevLeanOut = self.LeanOut
			self.LeanOut = 0
			self.ActualTimeLeanOut = 0.0
		end
	end
end

function CPlayerController:MakeCrouch()
	local l_Can = false
	if self.Crouch then
		l_Can = self.CharacterController:UpdateCharacterExtents(true, self.Height / 2.0)
	else
		l_Can = self.CharacterController:UpdateCharacterExtents(false, self.Height / 2.0)
	end
	if l_Can then
		self.Crouch = not self.Crouch
		self.CharacterController:SetbCrouch(self.Crouch)
		self.ActualTimeCrouch = 0.0
	end
	
	return l_Can
end

function CPlayerController:GetIsMoving()
	return self.IsMoving
end

function CPlayerController:GetIsRunning()
	return self.Run
end

function CPlayerController:GetIsCrouch()
	return self.Crouch
end

function CPlayerController:GetMeshOffset( aOffset )
	local lUp = self.Side ^ self.Forward
	lUp:Normalize()
	return self.Forward * aOffset.x + self.Side * aOffset.z + lUp * aOffset.y
end