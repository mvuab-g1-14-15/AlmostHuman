class "CBoss"

function CBoss:__init()
	--Basic properties
	self.Name = "Boss"
	
	self.Radius = 2.0
	self.Height = 8.0
	
	self.Delta = 1.0
	self.DeltaRot = 0.01
	
	self.Speed = 5.0
	self.TimeToRot = 1.0
	
	self.Lerp = CLerpAnimator1D()
	self.LerpInited = false
	
	self.NearPos = Vect3f(-92.0, 42.0, 62.0)
	self.MediumPos = Vect3f(-110.0, 42.0, 62.0)
	self.FarPos = Vect3f(-139.0, 42.0, 62.0)
	
	self.DoorPos = Vect3f(-137.0, 33.0, 87.0)
	
	self.TargetPos = self.NearPos
	
	self.HeightOffsetRO = 0.0
	
	--Renderable object
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, "boss")
		self.RenderableObject:SetRoomName( "sala4" )
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	local lROPos = self.TargetPos
	lROPos.y = lROPos.y - self.HeightOffsetRO
	self.RenderableObject:SetPosition( lROPos )
	self.RenderableObject:SetYaw(g_HalfPi)
	self.RenderableObject:SetScale( Vect3f(0.4) )
	self.RenderableObject:MakeTransform()
	
	self.RenderableObject:SetVelocity( 0.6 )
	self.RenderableObject:ChangeAnimation("walk", 0.2, 1)
	
	--Character controller
	if physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, self.TargetPos, CollisionGroup.ECG_ENEMY.value, -10.0) == false then 
		physic_manager:ReleasePhysicController(physic_manager:GetController(self.Name))
		physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, self.TargetPos, CollisionGroup.ECG_ENEMY.value, -10.0)
	end
	self.CharacterController = physic_manager:GetController(self.Name)
	self.CharacterController:SetYaw(0.0)
	self.CharacterController:SetScale( Vect3f(1.0) )
	
end

function CBoss:Destroy()
end

function CBoss:Update()
	local dt = timer:GetElapsedTime()
	
	if self:MoveToPos( self.TargetPos ) then
		if self.TargetPos == self.NearPos then
			self.TargetPos = self.FarPos
		elseif self.TargetPos == self.MediumPos then
			self.TargetPos = self.NearPos
		elseif self.TargetPos == self.FarPos then
			self.TargetPos = self.MediumPos
		end
	end
	
	local lROPos = self:GetPosition()
	lROPos.y = lROPos.y - self.HeightOffsetRO
	self.RenderableObject:SetPosition( lROPos )
	self.RenderableObject:MakeTransform()
end

function CBoss:GetName()
	return self.Name
end

function CBoss:SetName( aName )
	self.Name = aName
end

function CBoss:GetPosition()
	local lPos = self.CharacterController:GetPosition()
	lPos.y = lPos.y - self.Height/2
	return lPos
end

function CBoss:SetPosition( aPos )
	self.CharacterController:SetPosition( aPos )
	self.RenderableObject:SetPosition( aPos )
	self.RenderableObject:MakeTransform()
end

function CBoss:SetYaw( aYaw )
	self.CharacterController:SetYaw( aYaw )
	self.RenderableObject:SetYaw( aYaw + g_HalfPi )
	self.RenderableObject:MakeTransform()
end

function CBoss:MoveToPos( aPos )	
	local dt = timer:GetElapsedTime()
	if not self:IsInPos( aPos ) then
		if self:RotateToPos( aPos ) then
			local DirVector = self:GetDirVectorNormalized2D( aPos )
			self.CharacterController:Move(DirVector * self.Speed, dt)
		else
			self.CharacterController:Move(Vect3f(0.0), dt)
		end
		return false
	end
	self.CharacterController:Move(Vect3f(0.0), dt)
	return true
end

function CBoss:GetDirVector( aPos )
	local DirVector = aPos - self:GetPosition()
	
	return DirVector
end

function CBoss:GetDirVectorNormalized( aPos )
	local DirVector = self:GetDirVector( aPos )
	DirVector:Normalize()
	
	return DirVector
end

function CBoss:GetDirVectorNormalized2D( aPos )
	local DirVector = self:GetDirVector( aPos )
	DirVector.y = 0.0
	DirVector:Normalize()
	
	return DirVector
end

function CBoss:RotateToPos( aPos )
	local ActualPos = self:GetPosition()
	local FinalPos = aPos

	local DirVector = self:GetDirVectorNormalized2D( aPos )
	
	local ActualYaw = self.CharacterController:GetYaw()
	local ActualYawPlus = ActualYaw + g_DoublePi
	local ActualYawMinus = ActualYaw - g_DoublePi
	local DirYaw = math.atan2( DirVector.z, DirVector.x )
	local DiffYaw = math.abs(DirYaw - ActualYaw)
	local DiffYawPlus = math.abs(DirYaw - ActualYawPlus) 
	local DiffYawMinus = math.abs(DirYaw - ActualYawMinus)
	
	if DiffYawPlus < DiffYaw then
		ActualYaw = ActualYawPlus
		DiffYaw = DiffYawPlus
		if DiffYawMinus < DiffYawPlus then
			ActualYaw = ActualYawMinus
			DiffYaw = DiffYawMinus
		end
		self.LerpInited = false
	else
		if DiffYawMinus < DiffYaw then
			ActualYaw = ActualYawMinus
			DiffYaw = DiffYawMinus
			if DiffYawPlus < DiffYawMinus then
				ActualYaw = ActualYawPlus
				DiffYaw = DiffYawPlus
			end
			self.LerpInited = false
		end
	end
	
	if DiffYaw > self.DeltaRot then
		if not self.LerpInited then
			self.Lerp:SetValues(ActualYaw, DirYaw, self.TimeToRot, 0)
			self.LerpInited = true
		end
		local dt = timer:GetElapsedTime()
		
		if self.Lerp:IsFinish() then
			self.LerpInited = false
		end
		
		local TickYaw = self.Lerp:Value(dt)
		
		self:SetYaw( TickYaw )
		return false
	end

	self.LerpInited = false
	return true
end

function CBoss:IsInPos( aPos )
	local DirVector = self:GetDirVector( aPos )
	DirVector.y = 0.0
	local Distance = DirVector:Length()
	return Distance < self.Delta
end

function CBoss:PlayFootstep()
	--if countdowntimer_manager:isTimerFinish("FootstepBoss") then
	--	sound_manager:PlayEvent( "Play_Footstep_boss", "Logan" )
	--	countdowntimer_manager:Reset("FootstepBoss", true)
	--end
end