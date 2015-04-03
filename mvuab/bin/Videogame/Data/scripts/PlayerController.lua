dofile("./data/scripts/Include.lua")

class "CPlayerController"

function CPlayerController:__init()
	self.Position = Vect3f(0.0)
	self.Height = 2.0
	self.Position.y = self.Position.y + self.Height/2.0
	self.Radius = 0.4
	self.Direction = Vect3f(0.0)
	self.Speed = 0.2
	
	self.Forward = Vect3f(0.0)
	self.Side = Vect3f(0.0)
	
	--States
	self.Run = false
	self.Crouch = false
	self.Shooting = false
	self.LeanOut = 0
	self.PrevLeanOut = 0
	
	--Timers
	self.TimeCrouch = 0.2
	self.ActualTimeCrouch = 0.2
	
	--Counters
	self.ShakeValueVertical = 0.0
	self.ShakeValueHorizontal = 0.0
	
	self.ShakeVerticalSpeed = 4.0
	self.ShakeHorizontalSpeed = 3.0
	self.ShakeVerticalAmplitude = 0.01
	self.ShakeHorizontalAmplitude = 0.01
	
	self.TimeLeanOut = 0.15
	self.ActualTimeLeanOut = 0.15
	
	physic_manager:AddController("Player", self.Radius, self.Height, 0.2, 0.5, 0.5, self.Position, CollisionGroup.ECG_PLAYER.value, -9.8)
	self.CharacterController = physic_manager:GetController("Player")
	core:trace("Player Controller initialized")
end

function CPlayerController:Update()
	local dt = timer:GetElapsedTime()

	--local l_PlayerCamera = camera_manager:GetCamera("TestProcessCam")
	local l_PlayerCamera = camera_manager:GetCurrentCamera()
	self:CalculateDirectionVectors(l_PlayerCamera)
	
	self:UpdateCamera(l_PlayerCamera, dt)
	
	self:UpdateInput()
	
	self:UpdateTimers(dt)
	
	local l_Speed = self.Speed
	if self.Crouch then
		l_Speed = l_Speed / 3.0
	end
	if self.Run then
		l_Speed = l_Speed * 2.0
	end
	
	local l_Velocity = self.Direction * l_Speed
	
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
	
	local l_Up = l_PlayerCamera:GetVecUp()
	
	local l_ShakeVerticalSpeed = self.ShakeHorizontalSpeed
	local l_ShakeHorizontalSpeed = self.ShakeHorizontalSpeed
	local l_ShakeVerticalAmplitude = self.ShakeVerticalAmplitude
	local l_ShakeHorizontalAmplitude = self.ShakeHorizontalAmplitude	
	if CheckVector(self.Direction) then
		l_ShakeVerticalAmplitude = l_ShakeVerticalAmplitude * 4.0
		if self.Crouch then
			l_ShakeVerticalSpeed = l_ShakeVerticalSpeed / 2.0
			l_ShakeHorizontalSpeed = l_ShakeHorizontalSpeed / 2.0
			l_ShakeVerticalAmplitude = l_ShakeVerticalAmplitude / 1.5
			l_ShakeHorizontalAmplitude = l_ShakeHorizontalAmplitude / 1.5
		end
		if self.Run then
			l_ShakeVerticalSpeed = l_ShakeVerticalSpeed * 1.3
			l_ShakeHorizontalSpeed = l_ShakeHorizontalSpeed * 1.3
			l_ShakeVerticalAmplitude = l_ShakeVerticalAmplitude * 2.0
			l_ShakeHorizontalAmplitude = l_ShakeHorizontalAmplitude * 2.0
		end
	end
	
	self.ShakeValueVertical = self.ShakeValueVertical + l_ShakeVerticalSpeed * dt
	self.ShakeValueHorizontal = self.ShakeValueHorizontal + l_ShakeHorizontalSpeed * dt
	l_CameraPosition = l_CameraPosition + l_ShakeVerticalAmplitude * self.Side * math.sin(self.ShakeValueVertical) + l_ShakeHorizontalAmplitude * l_Up * math.cos(self.ShakeValueHorizontal)
	
	l_PlayerCamera:SetPosition(l_CameraPosition)
end

function CPlayerController:SetPosition(position)
	self.Position = position
	self.CharacterController:SetPosition(position)
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
	if action_manager_lua_wrapper:DoAction(action_manager, "MoveYaw") then
		camera:AddYaw( -action_manager_lua_wrapper.amount * dt * g_CameraSensibility );
	end
	if action_manager_lua_wrapper:DoAction(action_manager, "MovePitch") then
		camera:AddPitch( -action_manager_lua_wrapper.amount * dt * g_CameraSensibility );
	end
end

function CPlayerController:CalculateDirectionVectors(l_PlayerCamera)
	self.Forward = l_PlayerCamera:GetDirection()
	local l_Up = l_PlayerCamera:GetVecUp()
	self.Side = self.Forward ^ l_Up
	self.Forward.y = 0.0
	self.Side.y = 0.0
	self.Forward:Normalize()
	self.Side:Normalize()
end

function CPlayerController:UpdateInput()
	local l_AnyDirectionPress = false
	self.Direction = Vect3f(0.0)
	if action_manager:DoAction("MoveForward") then
		self.Direction = self.Direction + self.Forward
		l_AnyDirectionPress = true
	end
	if action_manager:DoAction("MoveBackward") then
		self.Direction = self.Direction - self.Forward
		l_AnyDirectionPress = true
	end
	if action_manager:DoAction("MoveLeft") then
		self.Direction = self.Direction + self.Side
		l_AnyDirectionPress = true
	end
	if action_manager:DoAction("MoveRight") then
		self.Direction = self.Direction - self.Side
		l_AnyDirectionPress = true
	end
	if l_AnyDirectionPress then
		self.Direction:Normalize()
	end
	if action_manager:DoAction("ShootDown") then
		self.Shooting = true
	end
	if action_manager:DoAction("ShootUp") then
		self.Shooting = false
	end
	if action_manager:DoAction("Run") then
		self.Run = true
	else
		self.Run = false
	end
	if action_manager:DoAction("Crouch") then
		self.Crouch = not self.Crouch
		self.ActualTimeCrouch = 0.0
	end
	
	if self.LeanOut == 0 and self.ActualTimeLeanOut == self.TimeLeanOut then
		if action_manager:DoAction("LeanOutLeft") then
			self.LeanOut = 1
			self.ActualTimeLeanOut = 0.0
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