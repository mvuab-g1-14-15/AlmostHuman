class "CBoss"

function CBoss:__init()
	--Basic properties
	self.Name = "Boss"
	
	self.Radius = 2.0
	self.Height = 6.0
	
	self.Delta = 1.0
	self.DeltaRot = 0.01
	
	self.Speed = 5.0
	self.TimeToRot = 1.0
	
	self.Lerp = CLerpAnimator1D()
	self.LerpInited = false
	
	self.NearPos = Vect3f(-142.0,25.0, 58.0)
	self.MediumPos = Vect3f(-177.0,25.0, 58.0)
	self.FarPos = Vect3f(-200.0,25.0, 58.0)
	
	self.InitPos = Vect3f(-177.0, 25.0, 120.0)
	self.InitPos = self.MediumPos
	
	self.TargetPos = self.MediumPos
	
	self.HeightOffsetRO = 0.0
	
	--Renderable object
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, "boss")
		self.RenderableObject:SetRoomName( "sala4" )
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	local lROPos = self.InitPos
	lROPos.y = lROPos.y - self.HeightOffsetRO
	self.RenderableObject:SetPosition( lROPos )
	self.RenderableObject:SetYaw(g_HalfPi)
	self.RenderableObject:SetScale( Vect3f(0.3) )
	self.RenderableObject:MakeTransform()
	
	self.RenderableObject:SetVelocity( 0.6 )
	self.RenderableObject:ChangeAnimation("walk", 0.2, 1)
	
	--Character controller
	if not physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 1.0, 0.01, 1.0, self.InitPos, CollisionGroup.ECG_ENEMY.value, -10.0) then 
		physic_manager:ReleasePhysicController(physic_manager:GetController(self.Name))
		physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 1.0, 0.01, 1.0, self.InitPos, CollisionGroup.ECG_ENEMY.value, -10.0)
	end
	self.CharacterController = physic_manager:GetController(self.Name)
	self.CharacterController:SetYaw(0.0)
	self.CharacterController:SetScale( Vect3f(1.0) )
	
	self.FootstepTime = 1.0
	
	self.AttackingNear = false
	self.AttackingMedium = false
	self.AttackingFar = false
	
	self.Counter = 0
	
	self.BlashRight = CBlash( self.Name.."BlashRight")
	self.BlashLeft  = CBlash( self.Name.."BlashLeft")
end

function CBoss:Destroy()
end

function CBoss:Update()
	local dt = timer:GetElapsedTime()
	
	if self.AttackingNear then
		self:NearAttack()
	elseif self.AttackingMedium then
		self:MediumAttack()
	elseif self.AttackingFar then
		self:FarAttack()
	else
		if self:MoveToPos( self.TargetPos ) then
			if self.TargetPos == self.NearPos then
				self.TargetPos = self.FarPos
				self.AttackingNear = true
			elseif self.TargetPos == self.MediumPos then
				self.TargetPos = self.NearPos
				self.AttackingMedium = true
			elseif self.TargetPos == self.FarPos then
				self.TargetPos = self.MediumPos
				self.AttackingFar = true
			end
		end
	end
	
	self.BlashRight:Tick()
	self.BlashLeft:Tick()
	
	local lROPos = self:GetPosition()
	lROPos.y = lROPos.y - self.HeightOffsetRO
	self.RenderableObject:SetPosition( lROPos )
	self.RenderableObject:MakeTransform()
end

function CBoss:NearAttack()
	local lTimerName = "NearShootTimer"
	if not countdowntimer_manager:ExistTimer(lTimerName) then
		countdowntimer_manager:AddTimer(lTimerName, 1.0, false)
	else
		countdowntimer_manager:SetActive(lTimerName, true)
	end
	if countdowntimer_manager:isTimerFinish(lTimerName) then
		self:MakeShootNear()
		self.Counter = self.Counter + 1
		if self.Counter > 5 then
			self.AttackingNear = false
			self.Counter = 0
		end
		countdowntimer_manager:Reset(lTimerName, true)
	else
		self:RotateToPos( g_Player:GetPosition() )
	end
end

function CBoss:MediumAttack()
	local lTimerName = "MediumShootTimer"
	if not countdowntimer_manager:ExistTimer(lTimerName) then
		countdowntimer_manager:AddTimer(lTimerName, 3.0, false)
	else
		countdowntimer_manager:SetActive(lTimerName, true)
	end
	if countdowntimer_manager:isTimerFinish(lTimerName) then
		self:MakeShootMedium()
		self.Counter = self.Counter + 1
		if self.Counter > 3 then
			self.AttackingMedium = false
			self.Counter = 0
		end
		countdowntimer_manager:Reset(lTimerName, true)
	else
		self:RotateToPos( g_Player:GetPosition() )
	end
end

function CBoss:FarAttack()
	local lTimerName = "FarShootTimer"
	if not countdowntimer_manager:ExistTimer(lTimerName) then
		countdowntimer_manager:AddTimer(lTimerName, 5.0, false)
	else
		countdowntimer_manager:SetActive(lTimerName, true)
	end
	if countdowntimer_manager:isTimerFinish(lTimerName) then
		self:MakeShootFar()
		self.Counter = self.Counter + 1
		if self.Counter > 2 then
			self.AttackingFar = false
			self.Counter = 0
		end
		countdowntimer_manager:Reset(lTimerName, true)
	else
		self:RotateToPos( g_Player:GetPosition() )
	end
end

function CBoss:MakeShootNear()
	local lShootSpeed = 20.0
	local lDamage = 5.0
	local lPos1 = self.RenderableObject:GetBonePosition("CATRigHub004Bone011Bone001Bone001")
	local lPos2 = self.RenderableObject:GetBonePosition("CATRigHub004Bone006Bone001")
	local lDir1 = GetPlayerDirection( lPos1 )
	local lDir2 = GetPlayerDirection( lPos2 )
	self.BlashRight:Begin(lPos1)
	self.BlashLeft:Begin(lPos2)
	for i=1,10 do
		lRandom = Vect3f(0.1*(math.random()-0.5), 0.1*(math.random()-0.5), 0.1*(math.random()-0.5))
		enemy_manager:AddShoot( lPos1, lDir1 + lRandom, lShootSpeed, lDamage/2.0 )
		lRandom = Vect3f(0.1*(math.random()-0.5), 0.1*(math.random()-0.5), 0.1*(math.random()-0.5))
		enemy_manager:AddShoot( lPos2, lDir2 + lRandom, lShootSpeed, lDamage/2.0 )
	end
	--Todo play enemy sound
end

function CBoss:MakeShootMedium()
	local lShootSpeed = 40.0
	local lDamage = 10.0
	local lPos1 = self.RenderableObject:GetBonePosition("CATRigHub004Bone011Bone001Bone001")
	local lPos2 = self.RenderableObject:GetBonePosition("CATRigHub004Bone006Bone001")
	local lDir1 = GetPlayerDirection( lPos1 )
	local lDir2 = GetPlayerDirection( lPos2 )
	self.BlashRight:Begin(lPos1)
	self.BlashLeft:Begin(lPos2)
	for i=1,6 do
		enemy_manager:AddShoot( lPos1, lDir1, lShootSpeed-i*4, lDamage/2.0 )
		enemy_manager:AddShoot( lPos2, lDir2, lShootSpeed-i*4, lDamage/2.0 )
	end
	--Todo play enemy sound
end

function CBoss:MakeShootFar()
	local lShootSpeed = 50.0
	local lDamage = 20.0
	local lPos1 = self.RenderableObject:GetBonePosition("CATRigHub004Bone011Bone001Bone001")
	local lPos2 = self.RenderableObject:GetBonePosition("CATRigHub004Bone006Bone001")
	local lDir1 = GetPlayerDirection( lPos1 )
	local lDir2 = GetPlayerDirection( lPos2 )
	self.BlashRight:Begin(lPos1)
	self.BlashLeft:Begin(lPos2)
	enemy_manager:AddShoot( lPos1, lDir1, lShootSpeed, lDamage/2.0 )
	enemy_manager:AddShoot( lPos2, lDir2, lShootSpeed, lDamage/2.0 )
	--Todo play enemy sound
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
			self:PlayFootstep()
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
	local lTimerName = "FootstepBoss"
	if not countdowntimer_manager:ExistTimer(lTimerName) then
		countdowntimer_manager:AddTimer(lTimerName, self.FootstepTime, false)
	else
		countdowntimer_manager:SetActive(lTimerName, true)
	end
	if countdowntimer_manager:isTimerFinish(lTimerName) then
		--sound_manager:PlayEvent( "Play_Footstep_boss", self.Name )
		countdowntimer_manager:Reset(lTimerName, true)
	end
end