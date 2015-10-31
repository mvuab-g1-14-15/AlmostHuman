class "CEnemy"

function CEnemy:__init( aInfo )
	self.Name = aInfo.name
	self.Life = aInfo.life
	
	self.Active = aInfo.active
	
	self.Damage = aInfo.damage
	
	self.Radius = aInfo.radius
	self.Height = aInfo.height
	
	self.Delta = 1.0
	self.InitDelta = self.Delta
	self.DeltaRot = 0.01
	
	self.Speed = aInfo.speed
	self.TimeToRot = 1.0
	self.InitTimeToRot = self.TimeToRot
	
	self.Lerp = CLerpAnimator1D()
	self.LerpInited = false
	
	self.InitPosition = aInfo.position
	self.TargetPos = self.InitPosition
	
	self.BlashRight = CBlash( self.Name.."Right")
	self.BlashLeft  = CBlash( self.Name.."Left")
	
	self.IsPatrol = aInfo.is_patrol
	if self.IsPatrol then
		self.Waypoints = aInfo.waypoints
		self.ActualWaypoint = 1
		self.TargetPos = self.Waypoints[self.ActualWaypoint]
	end
	
	if not physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, self.InitPosition, CollisionGroup.ECG_ENEMY.value, -10.0) then 
		physic_manager:ReleasePhysicController(physic_manager:GetController(self.Name))
		physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, self.InitPosition, CollisionGroup.ECG_ENEMY.value, -10.0)
	end
	self.CharacterController = physic_manager:GetController(self.Name)
	
	self.HeightOffsetRO = 0.0
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, aInfo.mesh)
		self.RenderableObject:SetRoomName( aInfo.room )
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	self.RenderableObject:SetYaw( -self.CharacterController:GetYaw() + g_HalfPi )
	self.RenderableObject:MakeTransform()
	self.RenderableObject:ChangeAnimation("idle", 0.5, 1.0)

	self.UseGizmo = aInfo.use_gizmo
	if self.UseGizmo then
		self.Gizmo = gizmos_manager:CreateGizmo( self.Name.."TargetPos", self.InitPosition, 0.0, 0.0)
		local lGizmoElement = gizmos_manager:CreateGizmoElement(1, 0.2, self.InitPosition, 0.0, 0.0, CColor(1.0, 0.0, 0.0, 1.0))
		self.Gizmo:AddElement( lGizmoElement )
		gizmos_manager:AddResource( self.Name.."TargetPos", self.Gizmo )
	end
	
	self.Room = aInfo.room
	self.AStar = nil
	
	self.PathCalculated = false
	self.Path = {}
	self.ActualPathPoint = 0
	
	self.CanUseGraph = aInfo.can_use_graph
	self.UseGraph = false
	
	self.Suspected = false
	self.SuspectedPosition = Vect3f(0.0)
	
	self.OnDead = aInfo.on_dead_code
	if self.OnDead then
		self.OnDeadCode = aInfo.on_dead_code
	end
	
	camera_manager:NewCamera(CameraType.Free.value, self.Name, Vect3f( 0.0, 1.0, 0.0), Vect3f( 0.0 ))
	self.Camera = camera_manager:GetCamera(self.Name)
	self.Camera:SetZFar(aInfo.camera_far)
	self.Camera:SetFovInRadians(aInfo.camera_fov)
	self.CameraPitch = aInfo.camera_pitch
	self:UpdateCamera()
	
	self.ShootSpeed = aInfo.shoot_speed
	
	self.TimeToShoot = aInfo.time_to_shoot
	self.TimeBurst = aInfo.time_burst
	self.NumShootBurst = aInfo.num_shoot_burst
	self.ActualShootBurst = 0
	self.CountTimeShoot = 0.0
	
	self.DontMove = false
	
	self.ChaseDistance = aInfo.chase_distance
	
	self.CanSee = aInfo.can_see
	
	self.Fly = aInfo.fly
	self.InitHeight = self.InitPosition.y
	
	if self.Fly then
		self.Laser = renderable_objects_manager_glow:GetResource(self.Name.."Laser")
		if self.Laser == nil then
			self.Laser = CreateInstanceMesh(self.Name.."Laser", "DroneLaser")
			self.Laser:SetRoomName( aInfo.room )
			renderable_objects_manager_glow:AddResource(self.Name.."Laser", self.Laser)
		end
		self.Laser:SetScale(Vect3f(3.0, 1.0, 1.0))
		self.Laser:SetPitch(self.CameraPitch + g_HalfPi + 0.3 )
		self.RenderableObject:SetPitch(self.CameraPitch)
		
		self.Laser:MakeTransform()
		self.RenderableObject:MakeTransform()
	end
	
	self.Alarm = aInfo.alarm
	if self.Alarm then
		self.AlarmTimerName = self.Name.."AlarmTimer"
		self.AlarmTime = aInfo.alarm_time
	end
	
	self.BillboardEnemy = billboard_manager:CreateInstance("blash", Vect3f(0, 0, 0), true)
	
	sound_manager:SetGameObjectMapById(self.Name)
	sound_manager:RegisterGameObject(self.Name)
	self.FootstepTime = 0.5
	if self.Fly then
		sound_manager:PlayEvent( "Play_Drone_Movement", self.Name )
	end
	
	self.LastPos = Vect3f(0.0)
	self.Death = false
end

function CEnemy:Destroy()
	if self.OnDead then
		engine:Trace(self.Name.." executing on dead code.")
		local codeToExecute = "local lPos = Vect3f"..self:GetPosition():ToString()..";"..
							  "local selfName = '"..self:GetName().."';"..
							  self.OnDeadCode
		script_manager:RunCode(codeToExecute)
	end
	
	if self.Fly then
		renderable_objects_manager_glow:RemoveResource(self.Name.."Laser")
	end
			
	self.BlashRight:End()
	self.BlashLeft:End()
	physic_manager:ReleasePhysicController(self.CharacterController)
	sound_manager:UnregisterGameObject(self.Name)
end

function CEnemy:Update()
	--profiler:AddInit("CEnemy:Update()")
	if self.Life > 0 then
		if self.Active then
			local dt = timer:GetElapsedTime()
			
			self:UpdateCamera()
			
			self.BlashRight:Tick()
			self.BlashLeft:Tick()
			
			local IsPlayerInSight = false
			if self.CanSee then
				IsPlayerInSight = self:PlayerInSight()
			end
			
			if IsPlayerInSight then
				if self.Alarm then
					if not countdowntimer_manager:ExistTimer(self.AlarmTimerName) then
						countdowntimer_manager:AddTimer(self.AlarmTimerName, self.AlarmTime, false)
					else
						countdowntimer_manager:SetActive(self.AlarmTimerName, true)
					end
					if countdowntimer_manager:isTimerFinish(self.AlarmTimerName) then
						enemy_manager:SetAlarm(self.Room)
						countdowntimer_manager:Reset(self.AlarmTimerName, false)
					end
				end
				
				self.Delta = self.InitDelta * 2.0
				self.TimeToRot = self.InitTimeToRot / 2.0
				
				if self.Fly then
					local lPlayerDir = GetPlayerDirection(self:GetPosition())
					lPlayerDir:Normalize()
					local lPitch = math.atan2( lPlayerDir.y, math.sqrt( lPlayerDir.z * lPlayerDir.z + lPlayerDir.x * lPlayerDir.x ) )
					self.RenderableObject:SetPitch(lPitch)
					self.Laser:SetPitch(lPitch + g_HalfPi)
				end
				
				local lDist = PlayerDistance(self)
				if lDist < self.ChaseDistance then
					self.DontMove = true
					if self:RotateToPos(g_Player:GetPosition()) then
						self:ChangeAnimation("shoot", 0.5, 1.0)
				
						self.CountTimeShoot = self.CountTimeShoot + dt
						local timerBurst = "Burst"..self.Name
						if self.CountTimeShoot >= self.TimeToShoot then
							if not countdowntimer_manager:ExistTimer(timerBurst) then
								countdowntimer_manager:AddTimer(timerBurst, self.TimeBurst, false)
							else
								countdowntimer_manager:SetActive(timerBurst, true)
							end
							if countdowntimer_manager:isTimerFinish(timerBurst) then
								self:MakeShoot()
								self.ActualShootBurst = self.ActualShootBurst + 1
								countdowntimer_manager:Reset(timerBurst, false)
							end
							if self.ActualShootBurst == 0 then
								self:MakeShoot()
								self.ActualShootBurst = self.ActualShootBurst + 1
							end
							if self.ActualShootBurst == self.NumShootBurst then
								self.CountTimeShoot = 0.0
								self.ActualShootBurst = 0
							end
						end
					end
				else
					self.DontMove = false
				end
				
				self.Suspected = true
				self.SuspectedPosition = g_Player:GetPosition()
			else
				self.DontMove = false
			end
			
			if self.Suspected then
				if self.CanUseGraph then
					self.UseGraph = true
				end
				self.TargetPos = self.SuspectedPosition
			end
			
			if not self.DontMove then
				if self:MoveToPos( self.TargetPos ) then
					if self.Suspected then
						self.Suspected = false
						self.TargetPos = self.InitPosition
						self.Delta = self.InitDelta
						self.TimeToRot = self.InitTimeToRot
						if self.IsPatrol then
							self.ActualWaypoint = 1
						end
						if self.UseGraph then
							self.PathCalculated = false
						end
					else
						if self.TargetPos == self.InitPosition then
							self.UseGraph = false
						end
						if self.IsPatrol then
							self.ActualWaypoint = self.ActualWaypoint + 1
							if self.ActualWaypoint > #self.Waypoints then
								self.ActualWaypoint = 1
							end
							self.TargetPos = self.Waypoints[self.ActualWaypoint]
						end
					end
				end
			else
				local dt = timer:GetElapsedTime()
				self.CharacterController:Move(Vect3f(0.0), dt)
			end
			
			if self.Fly then
				local lPos = self:GetPosition()
				lPos.y = self.InitHeight
				self:SetPosition(lPos)
				self.Laser:SetPosition(lPos)
				self.Laser:SetYaw(-self.Camera:GetYaw() + g_HalfPi)
				self.Laser:MakeTransform()
			end
		else
			local dt = timer:GetElapsedTime()
			self.CharacterController:Move(Vect3f(0.0), dt)
		end
	else
		if not self.Death then
			lActualEnemy:ChangeAnimationAction("death", 0.5, 1.0)
			if self.Fly then
				renderable_objects_manager_glow:RemoveResource(self.Name.."Laser")
			end
			self.Death = true
		end
		local dt = timer:GetElapsedTime()
		self.CharacterController:Move(Vect3f(0.0), dt)
		local lActualPos = self:GetPosition()
		engine:TraceOnce("Actual pos "..lActualPos:ToString().." last pos "..self.LastPos:ToString())
		if self.LastPos == lActualPos then
			enemy_manager:DestroyEnemy(self)
			return
		else
			self.LastPos = lActualPos
		end
	end
	
	local lROPos = self:GetPosition()
	lROPos.y = lROPos.y - self.HeightOffsetRO
	self.RenderableObject:SetPosition( lROPos )
	self.RenderableObject:MakeTransform()
	
	sound_manager:SetGameObjectPosition(self.Name, self:GetPosition(), self:GetPosition())
	--profiler:AddEnd("CEnemy:Update()")
end

function CEnemy:PlayerInSight()
	if g_Player:GetIsHidden() or g_Player:GetRoom() ~= self.Room then
		return false
	end
	return PlayerInSight(self.Camera)
end

function CEnemy:GetName()
	return self.Name
end

function CEnemy:SetName( aName )
	self.Name = aName
end

function CEnemy:GetActive()
	return self.Active
end

function CEnemy:SetActive( aBool )
engine:TraceOnce("Setting the enemy "..self.Name.." active")
	self.Active = aBool
end

function CEnemy:GetLife()
	return self.Life
end

function CEnemy:GetPosition()
	local lPos = self.CharacterController:GetPosition()
	lPos.y = lPos.y - self.Height/2
	return lPos
end

function CEnemy:SetPosition( aPos )
	self.CharacterController:SetPosition( aPos )
	self.RenderableObject:SetPosition( aPos )
	self.RenderableObject:MakeTransform()
end

function CEnemy:GetDirection()
	return self.CharacterController:GetDirection()
end

function CEnemy:GetHeight()
	return self.CharacterController:GetHeight()
end

function CEnemy:SetYaw( aYaw )
	self.CharacterController:SetYaw( aYaw )
	self.RenderableObject:SetYaw( -aYaw + g_HalfPi )
	self.RenderableObject:MakeTransform()
end

function CEnemy:GetDirVector( aPos )
	local DirVector = aPos - self:GetPosition()
	
	return DirVector
end

function CEnemy:GetDirVectorNormalized( aPos )
	local DirVector = self:GetDirVector( aPos )
	DirVector:Normalize()
	
	return DirVector
end

function CEnemy:GetDirVectorNormalized2D( aPos )
	local DirVector = self:GetDirVector( aPos )
	DirVector.y = 0.0
	DirVector:Normalize()
	
	return DirVector
end

function CEnemy:MoveToPos( aPos )
	local dt = timer:GetElapsedTime()
	
	if self.UseGraph then
		if not self.PathCalculated then  
			if self.AStar == nil then
				self.AStar = enemy_manager:GetAStar(self.Room)
			end
			self.Path = self.AStar:GetPath( self:GetPosition(), aPos )
			self.PathCalculated = true
			self.ActualPathPoint = 1
		end
		
		local lRealTargetPos = self.Path:GetResource(self.ActualPathPoint)
		if not self:IsInPos( lRealTargetPos ) then
			self:ChangeAnimation("walk", 0.5, 1.0)
			if self:RotateToPos( lRealTargetPos ) then
				local DirVector = self:GetDirVectorNormalized2D( lRealTargetPos )
				local lPos = self:GetPosition()
				lBool, lEnemy = enemy_manager:CollisionWithEnemy(self.Name, lPos, self.Radius)
				if lBool then
					local lDirToEnemy = lEnemy:GetPosition() - lPos
					lDirToEnemy.y = 0.0
					if CheckVector(lDirToEnemy) then
						lDirToEnemy:Normalize()
					end
					if DirVector:DotProduct(lDirToEnemy) > 0 then					
						local lDir = lDirToEnemy ^ Vect3f(0.0, 1.0, 0.0)
						if CheckVector(lDir) then
							lDir:Normalize()
							lDir = lDir - lDirToEnemy * 0.2
							lDir:Normalize()
						end
						self:PlayFootstep()
						self.CharacterController:Move(lDir * self.Speed*5.0, dt)
					end
				else
					self:PlayFootstep()
					self.CharacterController:Move(DirVector * self.Speed, dt)
				end
				if self.UseGizmo then
					self.Gizmo:SetPosition(lRealTargetPos)
					self.Gizmo:MakeTransform()
				end
			else
				self.CharacterController:Move(Vect3f(0.0), dt)
			end
		else
			self.ActualPathPoint = self.ActualPathPoint + 1
			if self.ActualPathPoint == self.Path:size() then
				self.ActualPathPoint = self.ActualPathPoint - 1
			end
		end
		
		if ( self.Path:GetResource(self.Path:size() - 1):Distance( aPos ) > 5.0 ) then
			self.PathCalculated = false
			self.ActualPathPoint = 1
		end
		
		local lDistVect = lRealTargetPos - aPos
		local lDist = lDistVect:Length()
		if lDist < self.Delta then
			return true
		end
		self.CharacterController:Move(Vect3f(0.0), dt)
		return false
	end
	
	if not self:IsInPos( aPos ) then
		self:ChangeAnimation("walk", 0.5, 1.0)
		if self:RotateToPos( aPos ) then
			local DirVector = self:GetDirVectorNormalized2D( aPos )
			local lPos = self:GetPosition()
			lBool, lEnemy = enemy_manager:CollisionWithEnemy(self.Name, lPos, self.Radius)
			if lBool then
				local lDirToEnemy = lEnemy:GetPosition() - lPos
				lDirToEnemy.y = 0.0
				if CheckVector(lDirToEnemy) then
					lDirToEnemy:Normalize()
				end
				if DirVector:DotProduct(lDirToEnemy) > 0 then					
					local lDir = lDirToEnemy ^ Vect3f(0.0, 1.0, 0.0)
					if CheckVector(lDir) then
						lDir:Normalize()
						lDir = lDir - lDirToEnemy * 0.2
						lDir:Normalize()
					end
					self:PlayFootstep()
					self.CharacterController:Move(lDir * self.Speed*5.0, dt)
				end
			else
				self:PlayFootstep()
				self.CharacterController:Move(DirVector * self.Speed, dt)
			end
			if self.UseGizmo then
				self.Gizmo:SetPosition(aPos)
				self.Gizmo:MakeTransform()
			end
		else
			self.CharacterController:Move(Vect3f(0.0), dt)
		end
		return false
	end
	self.CharacterController:Move(Vect3f(0.0), dt)
	return true
end

function CEnemy:RotateToPos( aPos )
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

function CEnemy:IsInPos( aPos )
	local DirVector = self:GetDirVector( aPos )
	DirVector.y = 0.0
	local Distance = DirVector:Length()
	return Distance < self.Delta
end

function CEnemy:PlayFootstep()
	if not self.Fly then
		local lTimerName = "Footstep"..self.Name
		if not countdowntimer_manager:ExistTimer(lTimerName) then
			countdowntimer_manager:AddTimer(lTimerName, self.FootstepTime, false)
		else
			countdowntimer_manager:SetActive(lTimerName, true)
		end
		if countdowntimer_manager:isTimerFinish(lTimerName) then
			sound_manager:PlayEvent( "Play_Footstep_Androide", self.Name )
			countdowntimer_manager:Reset(lTimerName, true)
		end
	end
end

function CEnemy:GetRoom()
	return self.Room
end

function CEnemy:SetSuspected( aBool )
	self.Suspected = aBool
end

function CEnemy:GetSuspected()
	return self.Suspected
end

function CEnemy:SetSuspectedPosition( aPos )
	self.SuspectedPosition = aPos
end

function CEnemy:GetSuspectedPosition()
	return self.SuspectedPosition
end

function CEnemy:AddDamage( aAmount )
	self.Life = self.Life - aAmount
end

function CEnemy:GetYaw()
	return self.CharacterController:GetYaw()
end

function CEnemy:GetPitch()
	return self.CharacterController:GetPitch()
end

function CEnemy:SetOnDead(aBool)
	self.OnDead = aBool
end

function CEnemy:UpdateCamera()
	local lPosition = self:GetPosition()
	lPosition.y = lPosition.y + self:GetHeight()
	lPosition.x = lPosition.x + self:GetDirection().x
	lPosition.z = lPosition.z + self:GetDirection().z
	self.Camera:SetPosition(lPosition)
	self.Camera:SetYaw( self:GetYaw() )
	self.Camera:SetPitch( self.CameraPitch )
	self.Camera:MakeTransform()
	self.Camera:UpdateFrustum()
end

function CEnemy:MakeShoot()
	if self.Fly then
		local lDir = self.Camera:GetDirection()
		local lSide = lDir ^ Vect3f(0.0, 1.0, 0.0)
		local lUp = lSide ^ lDir
		if CheckVector(lDir) then
			lDir:Normalize()
		end
		if CheckVector(lSide) then
			lSide:Normalize()
		end
		if CheckVector(lUp) then
			lUp:Normalize()
		end
		
		local lPos = self:GetPosition()
		
		local lPos1 = lPos + lDir * 0.4 + lSide * 0.2 + lUp * 0.3
		local lPos2 = lPos + lDir * 0.4 - lSide * 0.2 + lUp * 0.3
		
		self.BlashRight:Begin(lPos1)
		self.BlashLeft:Begin(lPos2)
		
		local lDirShoot1 = GetPlayerDirection( lPos1 )
		local lDirShoot2 = GetPlayerDirection( lPos2 )
		local lSingleShootDamage = self.Damage/2.0
		enemy_manager:AddShoot( lPos1, lDirShoot1, self.ShootSpeed, lSingleShootDamage )
		enemy_manager:AddShoot( lPos2, lDirShoot2, self.ShootSpeed, lSingleShootDamage )
	else
		local lPos = self.RenderableObject:GetBonePosition("Base HumanRPalm")
		local lDir = GetPlayerDirection( lPos )
		self.BlashRight:Begin(lPos)
		enemy_manager:AddShoot( lPos, lDir, self.ShootSpeed, self.Damage )
	end
	--Todo play enemy sound
end

function CEnemy:ChangeAnimation(animation, delayin, delayout)
	self.RenderableObject:ChangeAnimation(animation, delayin, delayout)
end

function CEnemy:ChangeAnimationAction(animation, delayin, delayout)
	self.RenderableObject:ChangeAnimationAction(animation, delayin, delayout)
end

--class "CEnemyLUA"
--
--function CEnemyLUA:__init(Node, state_machine, core_enemy)
--
--	self.Life = core_enemy:GetLife()
--	self.RespawnTime = core_enemy:GetRespawnTime()
--    self.TimeToShoot = core_enemy:GetTimeToShoot()
--	self.TimeBurst = 0.3
--	self.NumShootBurst = 3
--	self.ActualShootBurst = 0
--    self.ShootAccuracy = core_enemy:GetShootAccuracy()
--	self.Damage = core_enemy:GetDamage()
--	self.CountTimeShoot = 0.0
--	self.Name = Node:GetAttributeString("name", "no_name")
--	self.Type = Node:GetAttributeString("type", "no_enemy")
--	self.Radius = 0.4
--	self.Height = 2.0
--	self.PitchCameraMove = 0.0
--	self.Room = Node:GetAttributeString("room", "no_name")
--	self.Suspected = false
--	self.SuspectedPosition = Vect3f(0.0)
--	
--	--self.Gizmo = gizmos_manager:CreateGizmo( self.Name.."TargetPos", Vect3f(0.0), 0.0, 0.0)
--	--local lGizmoElement = gizmos_manager:CreateGizmoElement(1, 0.05, Vect3f(0.0), 0.0, 0.0, CColor(1.0, 0.0, 0.0, 1.0))
--	--self.Gizmo:AddElement( lGizmoElement )
--	--gizmos_manager:AddResource( self.Name.."TargetPos", self.Gizmo )
--	self.GizmoPosition = Vect3f(0.0)
--	
--	self.ShootSpeed = 50.0
--	self.Velocity = 2.0
--	self.Delta = 0.5
--	
--	self.IsHurting = false
--	countdowntimer_manager:AddTimer(self.Name.."HurtTime", 1.0, false)
--	
--	self.OnDeadCode = Node:GetAttributeString("on_dead_code", "")
--	self.OnDead = true
--	if self.OnDeadCode == "" then
--		self.OnDead = false
--	end
--	
--	self.Alarmado = false
--	self.PositionAlarm = Vect3f(0,0,0)
--	self.IdRouteAlarm = Node:GetAttributeInt("route_alarm", -1)
--	
--	if physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), CollisionGroup.ECG_ENEMY.value, -10.0) == false then 
--		physic_manager:ReleasePhysicController(physic_manager:GetController(self.Name))
--		physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), CollisionGroup.ECG_ENEMY.value, -10.0)
--	end
--	self.CharacterController = physic_manager:GetController(self.Name)
--	self.CharacterController:SetYaw(Node:GetAttributeFloat("yaw", 0.0))
--	self.CharacterController:SetScale(Node:GetAttributeVect3f("scale", Vect3f( 1.0, 1.0, 1.0)))
--	
--	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
--	if self.RenderableObject == nil then
--		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, Node:GetAttributeString("mesh", "default_mesh"))
--		self.RenderableObject:SetRoomName( Node:GetAttributeString("room", "") )
--		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
--	end
--	
--	local l_MeshPosition = self.CharacterController:GetPosition()
--	l_MeshPosition.y = l_MeshPosition.y - self.Height/2
--	self.RenderableObject:SetVelocity(self.Velocity)
--	self.RenderableObject:SetPosition(l_MeshPosition)
--	self.RenderableObject:SetYaw(-self.CharacterController:GetYaw() + g_HalfPi)
--	self.RenderableObject:SetPitch(self.CharacterController:GetPitch())
--	self.RenderableObject:SetRoll(self.CharacterController:GetRoll())
--	self.RenderableObject:SetScale(self.CharacterController:GetScale())
--	self.RenderableObject:MakeTransform()
--
--	self.Brain = CBrain(Node:GetAttributeString("state", "inicial"), state_machine)
--	
--	self.CurrentAnimation = Node:GetAttributeString("state", "inicial")
--	self.BackAnimation = self.CurrentAnimation
--	--self:ChangeState("perseguir")
--	--self:GetAnimationModel():ChangeAnimation("perseguir", 0.2, 1.0)
--	
--	camera_manager:NewCamera(CameraType.Free.value, self.Name, Vect3f( 0.0, 1.0, 0.0), Vect3f( 0.0 ))
--	self.Camera = camera_manager:GetCamera(self.Name)
--	self.Camera:SetZFar(20.0)
--	if self.Type == "drone" then
--		self.Camera:SetFovInRadians(20.0)
--	end
--	
--	self:UpdateCamera()
--	
--	AddEnemy(Node:GetAttributeString("texture_enemy", ""), Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), Node:GetAttributeInt("width", 50.0), Node:GetAttributeInt("height", 50.0), self.CharacterController:GetYaw(), Node:GetAttributeString("get_position_script", "no_script"), Node:GetAttributeString("orientation", "no_script"), self.Name)
--	
--	--engine:Trace("CEnemyLUA: " .. self.Name .. " initialized")
--end
--
--function CEnemyLUA:Destroy()
--	if self.OnDead then
--		local codeToExecute = "local lPos = Vect3f"..self:GetPosition():ToString().."; local selfName = '"..self:GetName().."'; "..self.OnDeadCode
--		script_manager:RunCode(codeToExecute)
--	end
--		
--	physic_manager:ReleasePhysicController(self.CharacterController)
--	renderable_objects_manager_characters:RemoveResource(self.Name)
--	
--	--engine:Trace("He muerto Vida actual: "..self.Life)
--end
--
--function CEnemyLUA:Update()
--	self:SetMeshTransform()
--	self.Brain:Update()
--	self:UpdateCamera()
--	self:CheckHurting()
--	self:MakeGizmo()
--end
--
--function CEnemyLUA:MakeGizmo()
--	local lGizmoName = self.Name.."TargetPos"
--	local Gizmo = gizmos_manager:GetResource(lGizmoName)
--	if Gizmo == nil then
--		Gizmo = gizmos_manager:CreateGizmo(lGizmoName, self.GizmoPosition, 0.0, 0.0)
--		local GizmoElement = gizmos_manager:CreateGizmoElement(1, 0.25, Vect3f(0.0), 0.0, 0.0, CColor(1.0, 0.0, 0.0, 1.0))
--		Gizmo:AddElement(GizmoElement)
--		gizmos_manager:AddResource(lGizmoName, Gizmo)
--	else
--		self.GizmoPosition.y = self.CharacterController:GetPosition().y - self.Height/2
--		Gizmo:SetPosition(self.GizmoPosition)
--	end
--	Gizmo:MakeTransform()
--end
--
--function CEnemyLUA:UpdateCamera()
--	lPosition = self.CharacterController:GetPosition()
--	lPosition.y = lPosition.y + self:GetHeight()
--	lPosition.x = lPosition.x + self:GetDirection().x
--	lPosition.z = lPosition.z + self:GetDirection().z
--	self.Camera:SetPosition(lPosition)
--	self.Camera:SetYaw(-self.RenderableObject:GetYaw() + g_HalfPi)
--	if not (self:GetActualState() == "atacar") then
--		if self.PitchCameraMove >= 360.0 then
--			self.PitchCameraMove = 0
--		end
--		self.PitchCameraMove = self.PitchCameraMove + timer:GetElapsedTime()
--	end
--	--self.Camera:SetPitch(self.RenderableObject:GetPitch() - (g_Pi/18)- (g_Pi*2*math.sin(self.PitchCameraMove)/18.0))
--	self.Camera:SetPitch(self.RenderableObject:GetPitch())
--	--self.Camera:SetDirection(self:GetDirectionEnemy())
--	self.Camera:MakeTransform()
--	self.Camera:UpdateFrustum()
--end
--function CEnemyLUA:SetMeshTransform()
--	local l_MeshPosition = self.CharacterController:GetPosition()
--	l_MeshPosition.y = l_MeshPosition.y - self.Height/2
--	self.RenderableObject:SetPosition(l_MeshPosition)
--	if self:GetActualState() ~= "atacar" then
--		self.RenderableObject:SetYaw(-self.CharacterController:GetYaw() + g_HalfPi)
--		self.RenderableObject:SetPitch(self.CharacterController:GetPitch())
--		self.RenderableObject:SetRoll(self.CharacterController:GetRoll())
--	end
--	self.RenderableObject:MakeTransform()
--end
--
--function CEnemyLUA:AddLife(amount)
--	self.Life = self.Life + amount
--end
--
--function CEnemyLUA:AddDamage(amount)
--	self.Life = self.Life - amount
--	--engine:Trace("Vida actual: "..self.Life)
--end
--
--function CEnemyLUA:SetYaw(yaw)
--	self.RenderableObject:SetYaw(-yaw + g_HalfPi)
--end
--
--function CEnemyLUA:GetYaw()
--	return self.RenderableObject:GetYaw()
--end
--
--function CEnemyLUA:SetPitch(pitch)
--	self.RenderableObject:SetPitch(pitch)
--end
--
--function CEnemyLUA:GetPitch()
--	return self.RenderableObject:GetPitch()
--end
--
--function CEnemyLUA:GetActualState()
--	return self.Brain:GetActualState()
--end
--
--function CEnemyLUA:ChangeState(NewState)
--	self.Brain:SetNextState(NewState)
--end
--
--function CEnemyLUA:GetAnimationModel()
--	return self.RenderableObject
--end
--
--function CEnemyLUA:GetName()
--	return self.Name
--end
--
--function CEnemyLUA:SetTimeToShoot(aTimeToShoot)
--	self.TimeToShoot = aTimeToShoot
--end
--
--function CEnemyLUA:GetTimeToShoot()
--	return self.TimeToShoot
--end
--
--function CEnemyLUA:GetCountTimeShoot()
--	return self.CountTimeShoot
--end
--
--function CEnemyLUA:SetCountTimeShoot(CountTimeShoot)
--	self.CountTimeShoot = CountTimeShoot
--end
--
--function CEnemyLUA:GetHeight()
--	return self.CharacterController:GetHeight()
--end
--
--function CEnemyLUA:GetDirection()
--	local Yaw = self:GetYaw()
--	local Pitch = self:GetPitch()
--	local Direction = Vect3f( math.cos( Yaw ) * math.cos( Pitch ), math.sin( Pitch ), math.sin( Yaw ) * math.cos( Pitch ) )
--	return Direction
--end
--
--function CEnemyLUA:GetDirectionEnemy()
--	local Yaw = self.CharacterController:GetYaw()
--	local Pitch = self.CharacterController:GetPitch()
--	local Direction = Vect3f( math.cos( Yaw ) * math.cos( Pitch ), math.sin( Pitch ), math.sin( Yaw ) * math.cos( Pitch ) )
--	return Direction
--end
--
--function CEnemyLUA:GetCharacterController()
--	return self.CharacterController
--end
--
--function CEnemyLUA:GetCamera()
--	return self.Camera
--end
--
--function CEnemyLUA:MakeShoot(aDirection)
--	 lPosition = self.CharacterController:GetPosition() + aDirection * 0.4
--	 lPosition.y = lPosition.y + (self:GetHeight() / 2.0)
--	--lPosition = self.Camera:GetPosition()
--	lShoot = CShootLUA( self.ShootSpeed, aDirection, lPosition, self.Damage )	
--	enemy_manager:AddShoot(lShoot)
--	if self.Type == "drone" then
--		engine:Trace("He entrado en el disparo del drone")
--		local YawTmp = math.atan2(aDirection.z, aDirection.x)
--		local PitchTmp = math.atan2(aDirection.y, math.sqrt( (aDirection.z * aDirection.z) + (aDirection.x * aDirection.x) ) )
--		self.RenderableObject:SetYaw(YawTmp + g_HalfPi)
--		self.RenderableObject:SetPitch(PitchTmp - (g_Pi/18))
--		
--		self.RenderableObject:MakeTransform()
--	end
--	--Play shoot enemy sound
--end
--
--function CEnemyLUA:GetLife()
--	return self.Life
--end
--
--function CEnemyLUA:GetBrain()
--	return self.Brain
--end
--
--function CEnemyLUA:GetPitchCameraMove()
--	return self.PitchCameraMove
--end
--
--function CEnemyLUA:SetPitchCameraMove(aPitchCameraMove)
--	self.PitchCameraMove = aPitchCameraMove
--end
--
--function CEnemyLUA:GetType()
--	return self.Type
--end
--
--function CEnemyLUA:GetDead()
--	return self.Life <= 0
--end
--
--function CEnemyLUA:MoveToPos( aPos )
--	if self:IsInPos( aPos ) then
--		return true
--	end
--	
--	local CharacterController = self:GetCharacterController()
--	lPos = CharacterController:GetPosition()
--    lAStar = enemy_manager:GetAStar( self.Room )
--
--    if ( not self.PathCalculated ) then  
--		engine:Trace("He entrado a calcular la posicion")
--        self.Path = lAStar:GetPath( lPos, aPos )
--        self.PathCalculated = true
--    end
--	
--    lTargetPos = lPos
--    lTargetPos.y = 0
--	lWaypointPos = self.Path:GetResource(self.ActualPathPoint)
--	--self.Gizmo:SetPosition( lWaypointPos )
--	--self.Gizmo:MakeTransform()
--	self.GizmoPosition = lWaypointPos
--	lWaypointPos.y = 0.0
--    lDist = lTargetPos:Distance( lWaypointPos )
--	
--	count = self.Path:size()
--	--engine:Trace("El tamaño de la path es: "..lDist)
--    if ( (count - self.ActualPathPoint) > 2 ) then
--        if ( lDist < 1 ) then
--            self.ActualPathPoint = self.ActualPathPoint + 1
--		end
--        lTargetPos = self.Path:GetResource(self.ActualPathPoint)
--    end
--    lTargetPos.y = 0
--	
--	--engine:Trace("la posicion a la que voy: "..lTargetPos:ToString())
--	self:MoveToWaypoint(lTargetPos)
--    
--    if ( self.Path:GetResource(count - 1):Distance( aPos ) > 5.0 ) then
--        self.PathCalculated = false
--		self.ActualPathPoint = 1
--	end
--	
--	engine:Trace("Estoy en "..lPos:ToString()..". Voy a "..self.Path:GetResource(count - 1):ToString()..".")
--	
--	return false
--end
--
--function CEnemyLUA:IsInPos( aPos )
--	local CharacterController = self:GetCharacterController()
--	local ActualPos = CharacterController:GetPosition()
--	local FinalPos = aPos
--	FinalPos.y = 0.0
--	ActualPos.y = 0.0
--	local DistVector = FinalPos - ActualPos
--	local Distance = DistVector:Length()
--	return Distance < self.Delta
--end
--
--function CEnemyLUA:MoveToPlayer(PositionPlayer)
--	CharacterController = self:GetCharacterController()	
--	lPos = CharacterController:GetPosition()
--    lAStar = enemy_manager:GetAStar(self.Room)
--
--    if ( not self.PathCalculated ) then   
--        self.Path = lAStar:GetPath( lPos, PositionPlayer )
--        self.PathCalculated = true
--    end
--	
--    lTargetPos = lPos
--    lTargetPos.y = 0
--	lWaypointPos = self.Path:GetResource(self.ActualPathPoint)
--	--self.Gizmo:SetPosition( lWaypointPos )
--	--self.Gizmo:MakeTransform()
--	self.GizmoPosition = lWaypointPos
--	lWaypointPos.y = 0.0
--    lDist = lTargetPos:Distance( lWaypointPos )
--	
--	count = self.Path:size()
--	
--    if ( (count - self.ActualPathPoint) > 2 ) then
--        if ( lDist < 0.6 ) then
--            self.ActualPathPoint = self.ActualPathPoint + 1
--		end
--        lTargetPos = self.Path:GetResource(self.ActualPathPoint)
--    end
--    lTargetPos.y = 0
--	
--	self:MoveToWaypoint(lTargetPos)
--    
--    if ( self.Path:GetResource(count - 1):Distance( PositionPlayer ) > 5.0 ) then
--        self.PathCalculated = false
--		self.ActualPathPoint = 1
--	end
--end
--
--
--function CEnemyLUA:MoveToWaypoint(PositionPlayer)
--	local dt = timer:GetElapsedTime()
--	CharacterController = self:GetCharacterController()
--	local ActualPos = Vect3f(CharacterController:GetPosition())
--	local FinalPos = Vect3f()
--	if PositionPlayer == Vect3f(0.0) then
--		FinalPos = Vect3f(self.Waypoints[self.ActualWaypoint])
--	else		
--		FinalPos = PositionPlayer
--	end
--	--self.Gizmo:SetPosition( FinalPos )
--	--self.Gizmo:MakeTransform()
--	self.GizmoPosition = FinalPos
--	local lPosGizmo = self.CharacterController:GetPosition()
--	FinalPos.y = 0
--	ActualPos.y = 0
--	local Dir = FinalPos - ActualPos	
--	Dir.y = 0.0
--	if CheckVector(Dir) then
--		Dir:Normalize()
--	end
--	DirYaw = math.atan2( Dir.z, Dir.x )
--	Yaw = CharacterController:GetYaw()
--	YawDif = DirYaw - Yaw
--    PrevYaw = Yaw
--		
--	local YawDifCom = DirYaw - 2*g_Pi - Yaw
--	if math.abs( YawDifCom ) < math.abs( YawDif ) then
--		YawDif = YawDifCom
--	else
--		local YawDifCom = DirYaw + 2*g_Pi - Yaw
--		if math.abs( YawDifCom ) < math.abs( YawDif ) then
--			YawDif = YawDifCom
--		end
--	end
--		
--    if ( math.abs( YawDif ) < 0.1 ) then
--		CharacterController:Move( Dir * self.Velocity, dt )
--		self.RenderableObject:ChangeAnimation(self.CurrentAnimation, 0.2, 1)
--		Yaw = DirYaw;
--    else
--        CharacterController:Move( Vect3f( 0.0 ), dt )
--		if YawDif > 0 then
--			YawDif = 1
--			self.RenderableObject:ChangeAnimation("turn_left", 0.2, 1)
--		else
--			YawDif = -1
--			self.RenderableObject:ChangeAnimation("turn_right", 0.2, 1)
--		end
--        Yaw = Yaw + (YawDif * self.TurnSpeed * dt)
--
--        if ( ( Yaw < DirYaw and PrevYaw > DirYaw ) or ( Yaw > DirYaw and PrevYaw < DirYaw ) ) then
--            Yaw = DirYaw
--		end
--    end
--	
--    CharacterController:SetYaw( Yaw )
--    self:SetYaw( Yaw )
--end
--
--function CEnemyLUA:IsInWaypoint()
--	CharacterController = self:GetCharacterController()
--	local ActualPos = CharacterController:GetPosition()
--	local FinalPos = self.Waypoints[self.ActualWaypoint]
--	FinalPos.y = 0.0
--	ActualPos.y = 0.0
--	local DistVector = FinalPos - ActualPos
--	local Distance = DistVector:Length()
--	return Distance < self.Delta
--end
--
--function CEnemyLUA:GetVelocity()
--	return self.Velocity
--end
--
--function CEnemyLUA:SetVelocity(velocity)
--	self.Velocity = velocity
--	self.RenderableObject:SetVelocity(self.Velocity)
--end
--
--function CEnemyLUA:GetRoom()
--	return self.Room
--end
--
--function CEnemyLUA:ChangeRoute(position)
--	self.Alarmado = true
--	self.ActualPathPoint = 1
--	self.PathCalculated = false
--	self.PositionAlarm = position[1]
--end
--
--function CEnemyLUA:ChangeAnimation(animation, delayin, delayout)
--	engine:Trace("Next animation: "..animation)
--	if not self.IsHurting then
--		engine:Trace("Is not hurting.")
--		self.CurrentAnimation = animation
--		self.RenderableObject:ChangeAnimation(animation, delayin, delayout)
--	end
--end
--
--function CEnemyLUA:RotateToPlayer()
--	local dt = timer:GetElapsedTime()
--	CharacterController = self:GetCharacterController()
--	local ActualPos = Vect3f(CharacterController:GetPosition())
--	local FinalPos = g_Player:GetPosition()
--	FinalPos.y = 0
--	ActualPos.y = 0
--	local Dir = FinalPos - ActualPos	
--	Dir.y = 0.0
--	if CheckVector(Dir) then
--		Dir:Normalize()
--	end
--	DirYaw = math.atan2( Dir.z, Dir.x )
--	Yaw = CharacterController:GetYaw()
--	YawDif = DirYaw - Yaw
--    PrevYaw = Yaw
--		
--	local YawDifCom = DirYaw - 2*g_Pi - Yaw
--	if math.abs( YawDifCom ) < math.abs( YawDif ) then
--		YawDif = YawDifCom
--	else
--		local YawDifCom = DirYaw + 2*g_Pi - Yaw
--		if math.abs( YawDifCom ) < math.abs( YawDif ) then
--			YawDif = YawDifCom
--		end
--	end
--		
--    if ( math.abs( YawDif ) < 0.1 ) then
--		Yaw = DirYaw;
--    else
--        CharacterController:Move( Vect3f( 0.0 ), dt )
--		if YawDif > 0 then
--			YawDif = 1
--		else
--			YawDif = -1
--		end
--        Yaw = Yaw + (YawDif * self.TurnSpeed * dt)
--
--        if ( ( Yaw < DirYaw and PrevYaw > DirYaw ) or ( Yaw > DirYaw and PrevYaw < DirYaw ) ) then
--            Yaw = DirYaw
--		end
--    end
--	
--    CharacterController:SetYaw( Yaw )
--    self:SetYaw( Yaw )
--end
--
--function CEnemyLUA:SetIsHurting()
--	if self.Life > 0 then
--		self.IsHurting = true
--		countdowntimer_manager:SetActive(self.Name.."HurtTime", true)
--	end
--end
--
--function CEnemyLUA:CheckHurting()
--	if self.Life > 0 then
--		if self.IsHurting then
--			if countdowntimer_manager:IsActive(self.Name.."HurtTime") then
--				if countdowntimer_manager:isTimerFinish(self.Name.."HurtTime") then
--					self.IsHurting = false
--					countdowntimer_manager:Reset(self.Name.."HurtTime", false)					
--				end
--			end
--		end
--	else
--		self.IsHurting = false
--	end
--end
--
--function CEnemyLUA:SetSuspected( aBool )
--	engine:Trace("Setting suspected "..self:GetName())
--	self.Suspected = aBool
--end
--
--function CEnemyLUA:GetSuspected()
--	return self.Suspected
--end
--
--function CEnemyLUA:SetSuspectedPosition( aPos )
--	self.SuspectedPosition = aPos
--end
--
--function CEnemyLUA:GetSuspectedPosition()
--	return self.SuspectedPosition
--end