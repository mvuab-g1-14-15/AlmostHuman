dofile("./data/scripts/Blash.lua")

class "CBoss"

function CBoss:__init()
	--Basic properties
	self.Name = "Boss"
	
	self.Radius = 2.0
	self.Height = 6.0
	
	self.Delta = 1.0
	self.DeltaRot = 0.01
	
	self.Speed = 3.0
	self.TimeToRot = 1.0
	
	self.Lerp = CLerpAnimator1D()
	self.LerpInited = false
	
	self.NearPos = Vect3f(-142.0,25.0, 60.0)
	self.MediumPos = Vect3f(-177.0,25.0, 58.0)
	
	self.InitPos = Vect3f(-177.0, 25.0, 88.0)
	
	self.TargetPos = self.MediumPos
	
	self.HeightOffsetRO = 0.5
	
	--Renderable object
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, "boss")
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	self.RenderableObject:SetRoomName( "sala4" )
	local lROPos = self.InitPos
	lROPos.y = lROPos.y - self.HeightOffsetRO
	self.RenderableObject:SetPosition( lROPos )
	self.RenderableObject:SetYaw(g_HalfPi)
	self.RenderableObject:SetScale( Vect3f(0.3) )
	self.RenderableObject:MakeTransform()
	
	self.RenderableObject:ChangeVisibility(true)
	
	self.RenderableObject:SetVelocity( 0.6 )
	self:SetAnimation("walk")
	
	--Character controller
	if not physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 1.0, 0.01, 1.0, self.InitPos, CollisionGroup.ECG_ENEMY.value, -10.0) then 
		physic_manager:ReleasePhysicController(physic_manager:GetController(self.Name))
		physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 1.0, 0.01, 1.0, self.InitPos, CollisionGroup.ECG_ENEMY.value, -10.0)
	end
	self.CharacterController = physic_manager:GetController(self.Name)
	self.CharacterController:SetYaw(0.0)
	self.CharacterController:SetScale( Vect3f(1.0) )
	
	self.FootstepTime = 0.6
	
	self.SpawnPositions = { Vect3f(-110.0, 21.0, 54.0)
						  , Vect3f(-110.0, 21.0, 66.0)
						  , Vect3f(-180.0, 21.0, 54.0)
						  , Vect3f(-180.0, 21.0, 66.0)}
	
	self.AttackingNear   = true
	self.AttackingMedium = false
	self.AttackingFar    = false
	
	self.Counter = 0
	
	self.BlashRight = CBlash( self.Name.."BlashRight")
	self.BlashLeft  = CBlash( self.Name.."BlashLeft")
	
	self.Life = 100.0
	
	self.InitStunBar = 800.0
	self.StunBar = self.InitStunBar
	
	self.ArrivedNear = false
	
	self.lParticle1 = CParticle( self.Name.."particle_stun1", "stunned", "core" );
	self.lParticle2 = CParticle( self.Name.."particle_stun2", "stunned", "core" );
	self.lParticle3 = CParticle( self.Name.."particle_stun3", "stunned", "core" );
	
	self.lParticle1:Init( self.RenderableObject:GetBonePosition("CATRigHub004")              );
	self.lParticle2:Init( self.RenderableObject:GetBonePosition("CATRigHub004Bone001Bone001"));
	self.lParticle3:Init( self.RenderableObject:GetBonePosition("CATRigHub004Bone007")       );
	
	g_BossInAction = false
	
	self.EnterShock = false
end

function CBoss:Destroy()
	self.BlashRight:End()
	self.BlashLeft:End()
	renderable_objects_manager_characters:RemoveResource(self.Name)
	physic_manager:ReleasePhysicController(self.CharacterController)
	sound_manager:UnregisterGameObject(self.Name)
end

function CBoss:Update()	
	if not self:IsStunned() then
		self.lParticle1:Hide();
		self.lParticle2:Hide();
		self.lParticle3:Hide();
		if not self.ArrivedNear then
			if self:MoveToPos( self.TargetPos ) then
				if self.TargetPos == self.MediumPos then
					self.TargetPos = self.NearPos
				else
					self.ArrivedNear = true
					self:SetAnimation("attack")
					sound_manager:PlayEvent("Play_Musica_Boss", "Ambient")
					g_Player:SetCanMove(true)
					g_BossInAction = true
				end
			end
		else
			if not countdowntimer_manager:ExistTimer("AttackTimer") then
				countdowntimer_manager:AddTimer("AttackTimer", 4.0, false)
			else
				countdowntimer_manager:SetActive("AttackTimer", true)
			end
			if countdowntimer_manager:isTimerFinish("AttackTimer") then
				if self.AttackingNear then
					self:NearAttack()
				elseif self.AttackingMedium then
					self:MediumAttack()
				elseif self.AttackingFar then
					self:FarAttack()
				end
			end
		end
	else
		self.lParticle1:Show();
		self.lParticle2:Show();
		self.lParticle3:Show();
		if not self.EnterShock then
			sound_manager:PlayEvent("Play_Electrocution_Boss", "Boss")
			self.EnterShock = true
		end
		sound_manager:PlayEvent("Play_Spark_Boss", "Boss")
		--self:SetAnimation("idle");
		----engine:TraceOnce("Boss stunned!")
	end
	
	self.lParticle1:ChangePosition( self.RenderableObject:GetBonePosition("CATRigHub004")              );
	self.lParticle2:ChangePosition( self.RenderableObject:GetBonePosition("CATRigHub004Bone001Bone001"));
	self.lParticle3:ChangePosition( self.RenderableObject:GetBonePosition("CATRigHub004Bone007")       );
	
	enemy_manager:SetAlarm( "sala4" )
	
	self.BlashRight:Tick()
	self.BlashLeft:Tick()
	
	local lROPos = self:GetPosition()
	lROPos.y = lROPos.y - self.HeightOffsetRO
	self.RenderableObject:SetPosition( lROPos )
	self.RenderableObject:MakeTransform()
end

function CBoss:GetLife()
	return self.Life
end

function CBoss:AddDamage(aValue)
	self.Life = self.Life - aValue
	----engine:TraceOnce("Damage to boss. Current life "..self.Life)
end

function CBoss:IsStunned()
	return self.StunBar <= 0
end

function CBoss:AddStun( aValue )
	self.StunBar = self.StunBar - aValue
	if self:IsStunned() then
		--self:SetAnimation("hurt")
	end
end

function CBoss:ClearStun()
	sound_manager:PlayEvent("Stop_Spark_Boss", "Boss")
	self.StunBar = self.InitStunBar
end

function CBoss:NearAttack()
	local lTimerName = "NearShootTimer"
	if not countdowntimer_manager:ExistTimer(lTimerName) then
		countdowntimer_manager:AddTimer(lTimerName, 1.0, false)
	else
		countdowntimer_manager:SetActive(lTimerName, true)
	end
	if countdowntimer_manager:isTimerFinish(lTimerName) then
		self:SetAnimation("attack")
		self:MakeShootNear()
		self.Counter = self.Counter + 1
		if self.Counter > 5 then
			self.Counter = 0
			countdowntimer_manager:Reset("AttackTimer", true)
			if enemy_manager:GetNumEnemy("sala4") == 0 then
				self:SetAttack("medium")
			end
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
		self:SetAnimation("attack")
		self:MakeShootMedium()
		self.Counter = self.Counter + 1
		if self.Counter > 3 then
			self.Counter = 0
			countdowntimer_manager:Reset("AttackTimer", true)
		end
		countdowntimer_manager:Reset(lTimerName, true)
	else
		self:RotateToPos( g_Player:GetPosition() )
	end
end

function CBoss:FarAttack()
	local lTimerName = "FarShootTimer"
	if not countdowntimer_manager:ExistTimer(lTimerName) then
		countdowntimer_manager:AddTimer(lTimerName, 0.5, false)
	else
		countdowntimer_manager:SetActive(lTimerName, true)
	end
	if countdowntimer_manager:isTimerFinish(lTimerName) then
		self:SetAnimation("attack")
		self:MakeShootFar()
		self.Counter = self.Counter + 1
		if self.Counter > 25 then
			self.Counter = 0
			countdowntimer_manager:Reset("AttackTimer", true)
			self:SetAttack("near")
		end
		countdowntimer_manager:Reset(lTimerName, true)
	else
		self:RotateToPos( g_Player:GetPosition() )
	end
end

function CBoss:SetAttack(aAttack)
	if aAttack == "near" then
		self.AttackingNear = true
		self.AttackingMedium = false
		self.AttackingFar = false
	end
	if aAttack == "medium" then
		self.AttackingNear = false
		self.AttackingMedium = true
		self.AttackingFar = false
	end
	if aAttack == "far" then
		self.AttackingNear = false
		self.AttackingMedium = false
		self.AttackingFar = true
	end
end

function CBoss:SpawnEnemies()
	local lIndexList = {}
	table.insert(lIndexList, math.random( #self.SpawnPositions ))
	while #lIndexList < 3 do
		lNum = math.random( #self.SpawnPositions )
		local lFound = false
		for _,lIndex in pairs(lIndexList) do
			if lIndex == lNum then
				lFound = true
			end
		end
		if not lFound then
			table.insert(lIndexList, lNum)
		end
	end
	local lNuMEnemies = enemy_manager:GetNumEnemy("sala4")
	for _,lNum in pairs(lIndexList) do
		if lNuMEnemies < 3 then
			enemy_manager:SpawnEnemy(self.SpawnPositions[lNum])
			lNuMEnemies = enemy_manager:GetNumEnemy("sala4")
		end
	end
end

function CBoss:MakeShootNear()
	local lDirToPlayer = GetPlayerDirection(self:GetPosition())
	lDirToPlayer.y = 0.0
	if CheckVector(lDirToPlayer) then
		lDirToPlayer:Normalize()
	end
	local lBossDir = self:GetDirection()
	lBossDir.y = 0.0
	if CheckVector(lBossDir) then
		lBossDir:Normalize()
	end
	
	local lDot = lDirToPlayer:DotProduct(lBossDir)
	
	local lShootSpeed = 20.0
	local lDamage = 5.0
	local lPos1 = self.RenderableObject:GetBonePosition("CATRigHub004Bone011Bone001Bone001")
	local lPos2 = self.RenderableObject:GetBonePosition("CATRigHub004Bone006Bone001")
	
	local lDir1
	local lDir2
	if lDot > 0 then
		lDir1 = GetPlayerDirection( lPos1 )
		lDir2 = GetPlayerDirection( lPos2 )
	else
		lDir1 = lBossDir;
		lDir2 = lBossDir;
	end
	
	self.BlashRight:Begin(lPos1)
	self.BlashLeft:Begin(lPos2)
	for i=1,10 do
		lRandom = Vect3f(0.02*(math.random()-0.5), 0.02*(math.random()-0.5), 0.02*(math.random()-0.5))
		enemy_manager:AddShoot( lPos1, lDir1 + lRandom, lShootSpeed, lDamage/2.0 )
		lRandom = Vect3f(0.02*(math.random()-0.5), 0.02*(math.random()-0.5), 0.02*(math.random()-0.5))
		enemy_manager:AddShoot( lPos2, lDir2 + lRandom, lShootSpeed, lDamage/2.0 )
	end
	sound_manager:PlayEvent("Play_Shoot_boss", self.Name)
end

function CBoss:MakeShootMedium()
	local lDirToPlayer = GetPlayerDirection(self:GetPosition())
	lDirToPlayer.y = 0.0
	if CheckVector(lDirToPlayer) then
		lDirToPlayer:Normalize()
	end
	local lBossDir = self:GetDirection()
	lBossDir.y = 0.0
	if CheckVector(lBossDir) then
		lBossDir:Normalize()
	end
	
	local lDot = lDirToPlayer:DotProduct(lBossDir)

	local lShootSpeed = 40.0
	local lDamage = 10.0
	local lPos1 = self.RenderableObject:GetBonePosition("CATRigHub004Bone011Bone001Bone001")
	local lPos2 = self.RenderableObject:GetBonePosition("CATRigHub004Bone006Bone001")

	local lDir1
	local lDir2
	if lDot > 0 then
		lDir1 = GetPlayerDirection( lPos1 )
		lDir2 = GetPlayerDirection( lPos2 )
	else
		lDir1 = lBossDir;
		lDir2 = lBossDir;
	end
	
	self.BlashRight:Begin(lPos1)
	self.BlashLeft:Begin(lPos2)
	for i=1,6 do
		enemy_manager:AddShoot( lPos1, lDir1, lShootSpeed-i*4, lDamage/2.0 )
		enemy_manager:AddShoot( lPos2, lDir2, lShootSpeed-i*4, lDamage/2.0 )
	end
	sound_manager:PlayEvent("Play_Shoot_boss", self.Name)
end

function CBoss:MakeShootFar()
	local lDirToPlayer = GetPlayerDirection(self:GetPosition())
	lDirToPlayer.y = 0.0
	if CheckVector(lDirToPlayer) then
		lDirToPlayer:Normalize()
	end
	local lBossDir = self:GetDirection()
	lBossDir.y = 0.0
	if CheckVector(lBossDir) then
		lBossDir:Normalize()
	end
	
	local lDot = lDirToPlayer:DotProduct(lBossDir)
	
	local lShootSpeed = 50.0
	local lDamage = 20.0
	local lPos1 = self.RenderableObject:GetBonePosition("CATRigHub004Bone011Bone001Bone001")
	local lPos2 = self.RenderableObject:GetBonePosition("CATRigHub004Bone006Bone001")

	local lDir1
	local lDir2
	if lDot > 0 then
		lDir1 = GetPlayerDirection( lPos1 )
		lDir2 = GetPlayerDirection( lPos2 )
	else
		lDir1 = lBossDir;
		lDir2 = lBossDir;
	end
	
	self.BlashRight:Begin(lPos1)
	self.BlashLeft:Begin(lPos2)
	enemy_manager:AddShoot( lPos1, lDir1, lShootSpeed, lDamage/2.0 )
	enemy_manager:AddShoot( lPos2, lDir2, lShootSpeed, lDamage/2.0 )
	sound_manager:PlayEvent("Play_Shoot_boss", self.Name)
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
	self.RenderableObject:SetYaw( -aYaw + g_HalfPi )
	self.RenderableObject:MakeTransform()
end

function CBoss:MoveToPos( aPos )	
	if not self:IsInPos( aPos ) then
		if self:RotateToPos( aPos ) then
			local DirVector = self:GetDirVectorNormalized2D( aPos )
			self:PlayFootstep()
			self.CharacterController:Move(DirVector * self.Speed, g_FrameTime)
		else
			self.CharacterController:Move(Vect3f(0.0), g_FrameTime)
		end
		return false
	end
	self.CharacterController:Move(Vect3f(0.0), g_FrameTime)
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
		if DiffYaw < 0 then
			--self:SetAnimation("giro_der")
		else
			--self:SetAnimation("giro_izq")
		end
		if not self.LerpInited then
			self.Lerp:SetValues(ActualYaw, DirYaw, self.TimeToRot, 0)
			self.LerpInited = true
		end
		
		if self.Lerp:IsFinish() then
			self.LerpInited = false
		end
		
		local TickYaw = self.Lerp:Value(g_FrameTime)
		
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
		sound_manager:PlayEvent( "Play_Footstep_boss", self.Name )
		countdowntimer_manager:Reset(lTimerName, true)
	end
end

function CBoss:GetDirection()
	return self.CharacterController:GetDirection()
end

function CBoss:SetAnimation( aName )
	self.RenderableObject:SetAnimationState(aName)
end