class "CEnemyLUA"

function CEnemyLUA:__init(Node, state_machine, core_enemy)

	self.Life = core_enemy:GetLife()
	self.RespawnTime = core_enemy:GetRespawnTime()
    self.TimeToShoot = core_enemy:GetTimeToShoot()
	self.TimeBurst = 0.3
	self.NumShootBurst = 3
	self.ActualShootBurst = 0
    self.ShootAccuracy = core_enemy:GetShootAccuracy()
	self.Damage = core_enemy:GetDamage()
	self.CountTimeShoot = 0.0
	self.Name = Node:GetAttributeString("name", "no_name")
	self.Type = Node:GetAttributeString("type", "no_enemy")
	self.Radius = 0.4
	self.Height = 2.0
	self.PitchCameraMove = 0.0
	self.Room = Node:GetAttributeString("room", "no_name")
	self.Suspected = false
	self.SuspectedPosition = Vect3f(0.0)
	
	self.ShootSpeed = 50.0
	self.Velocity = 2.0
	self.Delta = 0.5
	
	self.IsHurting = false
	countdowntimer_manager:AddTimer(self.Name.."HurtTime", 1.0, false)
	
	self.Alarmado = false
	self.PositionAlarm = Vect3f(0,0,0)
	self.IdRouteAlarm = Node:GetAttributeInt("route_alarm", -1)
	
	if physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), CollisionGroup.ECG_ENEMY.value, -10.0) == false then 
		physic_manager:ReleasePhysicController(physic_manager:GetController(self.Name))
		physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), CollisionGroup.ECG_ENEMY.value, -10.0)
	end
	self.CharacterController = physic_manager:GetController(self.Name)
	self.CharacterController:SetYaw(Node:GetAttributeFloat("yaw", 0.0))
	self.CharacterController:SetScale(Node:GetAttributeVect3f("scale", Vect3f( 1.0, 1.0, 1.0)))
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, Node:GetAttributeString("mesh", "default_mesh"))
		self.RenderableObject:SetRoomName( Node:GetAttributeString("room", "") )
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	
	local l_MeshPosition = self.CharacterController:GetPosition()
	l_MeshPosition.y = l_MeshPosition.y - self.Height/2
	self.RenderableObject:SetVelocity(self.Velocity)
	self.RenderableObject:SetPosition(l_MeshPosition)
	self.RenderableObject:SetYaw(-self.CharacterController:GetYaw() + g_HalfPi)
	self.RenderableObject:SetPitch(self.CharacterController:GetPitch())
	self.RenderableObject:SetRoll(self.CharacterController:GetRoll())
	self.RenderableObject:SetScale(self.CharacterController:GetScale())
	self.RenderableObject:MakeTransform()

	self.Brain = CBrain(Node:GetAttributeString("state", "inicial"), state_machine)
	
	self.CurrentAnimation = Node:GetAttributeString("state", "inicial")
	self.BackAnimation = self.CurrentAnimation
	--self:ChangeState("perseguir")
	--self:GetAnimationModel():ChangeAnimation("perseguir", 0.2, 1.0)
	
	camera_manager:NewCamera(CameraType.Free.value, self.Name, Vect3f( 0.0, 1.0, 0.0), Vect3f( 0.0 ))
	self.Camera = camera_manager:GetCamera(self.Name)
	self.Camera:SetZFar(20.0)
	if self.Type == "drone" then
		self.Camera:SetFovInRadians(20.0)
	end
	
	self:UpdateCamera()
	
	AddEnemy(Node:GetAttributeString("texture_enemy", ""), Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), Node:GetAttributeInt("width", 50.0), Node:GetAttributeInt("height", 50.0), self.CharacterController:GetYaw(), Node:GetAttributeString("get_position_script", "no_script"), Node:GetAttributeString("orientation", "no_script"), self.Name)
	
	--engine:Trace("CEnemyLUA: " .. self.Name .. " initialized")
end

function CEnemyLUA:Destroy()
	physic_manager:ReleasePhysicController(self.CharacterController)
	renderable_objects_manager_characters:RemoveResource(self.Name)
	--engine:Trace("He muerto Vida actual: "..self.Life)
end

function CEnemyLUA:Update()
	self:SetMeshTransform()
	self.Brain:Update()
	self:UpdateCamera()
	self:CheckHurting()
end

function CEnemyLUA:UpdateCamera()
	lPosition = self.CharacterController:GetPosition()
	lPosition.y = lPosition.y + self:GetHeight()
	lPosition.x = lPosition.x + self:GetDirection().x
	lPosition.z = lPosition.z + self:GetDirection().z
	self.Camera:SetPosition(lPosition)
	self.Camera:SetYaw(-self.RenderableObject:GetYaw() + g_HalfPi)
	if not (self:GetActualState() == "atacar") then
		if self.PitchCameraMove >= 360.0 then
			self.PitchCameraMove = 0
		end
		self.PitchCameraMove = self.PitchCameraMove + timer:GetElapsedTime()
	end
	--self.Camera:SetPitch(self.RenderableObject:GetPitch() - (g_Pi/18)- (g_Pi*2*math.sin(self.PitchCameraMove)/18.0))
	self.Camera:SetPitch(self.RenderableObject:GetPitch())
	--self.Camera:SetDirection(self:GetDirectionEnemy())
	self.Camera:MakeTransform()
	self.Camera:UpdateFrustum()
end
function CEnemyLUA:SetMeshTransform()
	local l_MeshPosition = self.CharacterController:GetPosition()
	l_MeshPosition.y = l_MeshPosition.y - self.Height/2
	self.RenderableObject:SetPosition(l_MeshPosition)
	if self:GetActualState() ~= "atacar" then
		self.RenderableObject:SetYaw(-self.CharacterController:GetYaw() + g_HalfPi)
		self.RenderableObject:SetPitch(self.CharacterController:GetPitch())
		self.RenderableObject:SetRoll(self.CharacterController:GetRoll())
	end
	self.RenderableObject:MakeTransform()
end

function CEnemyLUA:AddLife(amount)
	self.Life = self.Life + amount
end

function CEnemyLUA:AddDamage(amount)
	self.Life = self.Life - amount
	--engine:Trace("Vida actual: "..self.Life)
end

function CEnemyLUA:SetYaw(yaw)
	self.RenderableObject:SetYaw(-yaw + g_HalfPi)
end

function CEnemyLUA:GetYaw()
	return self.RenderableObject:GetYaw()
end

function CEnemyLUA:SetPitch(pitch)
	self.RenderableObject:SetPitch(pitch)
end

function CEnemyLUA:GetPitch()
	return self.RenderableObject:GetPitch()
end

function CEnemyLUA:GetActualState()
	return self.Brain:GetActualState()
end

function CEnemyLUA:ChangeState(NewState)
	self.Brain:SetNextState(NewState)
end

function CEnemyLUA:GetAnimationModel()
	return self.RenderableObject
end

function CEnemyLUA:GetName()
	return self.Name
end

function CEnemyLUA:SetTimeToShoot(aTimeToShoot)
	self.TimeToShoot = aTimeToShoot
end

function CEnemyLUA:GetTimeToShoot()
	return self.TimeToShoot
end

function CEnemyLUA:GetCountTimeShoot()
	return self.CountTimeShoot
end

function CEnemyLUA:SetCountTimeShoot(CountTimeShoot)
	self.CountTimeShoot = CountTimeShoot
end

function CEnemyLUA:GetHeight()
	return self.CharacterController:GetHeight()
end

function CEnemyLUA:GetDirection()
	local Yaw = self:GetYaw()
	local Pitch = self:GetPitch()
	local Direction = Vect3f( math.cos( Yaw ) * math.cos( Pitch ), math.sin( Pitch ), math.sin( Yaw ) * math.cos( Pitch ) )
	return Direction
end

function CEnemyLUA:GetDirectionEnemy()
	local Yaw = self.CharacterController:GetYaw()
	local Pitch = self.CharacterController:GetPitch()
	local Direction = Vect3f( math.cos( Yaw ) * math.cos( Pitch ), math.sin( Pitch ), math.sin( Yaw ) * math.cos( Pitch ) )
	return Direction
end

function CEnemyLUA:GetCharacterController()
	return self.CharacterController
end

function CEnemyLUA:GetCamera()
	return self.Camera
end

function CEnemyLUA:MakeShoot(aDirection)
	 lPosition = self.CharacterController:GetPosition() + aDirection * 0.4
	 lPosition.y = lPosition.y + (self:GetHeight() / 2.0)
	--lPosition = self.Camera:GetPosition()
	lShoot = CShootLUA( self.ShootSpeed, aDirection, lPosition, self.Damage )	
	g_EnemyManager:AddShoot(lShoot)
	if self.Type == "drone" then
		engine:Trace("He entrado en el disparo del drone")
		local YawTmp = math.atan2(aDirection.z, aDirection.x)
		local PitchTmp = math.atan2(aDirection.y, math.sqrt( (aDirection.z * aDirection.z) + (aDirection.x * aDirection.x) ) )
		self.RenderableObject:SetYaw(YawTmp + g_HalfPi)
		self.RenderableObject:SetPitch(PitchTmp - (g_Pi/18))
		
		self.RenderableObject:MakeTransform()
	end
	--Play shoot enemy sound
end

function CEnemyLUA:GetLife()
	return self.Life
end

function CEnemyLUA:GetBrain()
	return self.Brain
end

function CEnemyLUA:GetPitchCameraMove()
	return self.PitchCameraMove
end

function CEnemyLUA:SetPitchCameraMove(aPitchCameraMove)
	self.PitchCameraMove = aPitchCameraMove
end

function CEnemyLUA:GetType()
	return self.Type
end

function CEnemyLUA:GetDead()
	return self.Life <= 0
end

function CEnemyLUA:MoveToPos( aPos )
	if self:IsInPos( aPos ) then
		return true
	end
	
	local CharacterController = self:GetCharacterController()
	lPos = CharacterController:GetPosition()
    lAStar = g_EnemyManager:GetAStar( self.Room )

    if ( not self.PathCalculated ) then  
		engine:Trace("He entrado a calcular la posicion")
        self.Path = lAStar:GetPath( lPos, aPos )
        self.PathCalculated = true
    end
	
    lTargetPos = lPos
    lTargetPos.y = 0
	lWaypointPos = self.Path:GetResource(self.ActualPathPoint)
	lWaypointPos.y = 0.0
    lDist = lTargetPos:Distance( lWaypointPos )
	
	count = self.Path:size()
	engine:Trace("El tamaño de la path es: "..lDist)
    if ( (count - self.ActualPathPoint) > 2 ) then
        if ( lDist < 0.8 ) then
            self.ActualPathPoint = self.ActualPathPoint + 1
		end
        lTargetPos = self.Path:GetResource(self.ActualPathPoint)
    end
    lTargetPos.y = 0
	
	engine:Trace("la posicion a la que voy: "..lTargetPos:ToString())
	self:MoveToWaypoint(lTargetPos)
    
    if ( self.Path:GetResource(count - 1):Distance( aPos ) > 5.0 ) then
        self.PathCalculated = false
		self.ActualPathPoint = 1
	end
	
	return false
end

function CEnemyLUA:IsInPos( aPos )
	local CharacterController = self:GetCharacterController()
	local ActualPos = CharacterController:GetPosition()
	local FinalPos = aPos
	FinalPos.y = 0.0
	ActualPos.y = 0.0
	local DistVector = FinalPos - ActualPos
	local Distance = DistVector:Length()
	return Distance < self.Delta
end

function CEnemyLUA:MoveToPlayer(PositionPlayer)
	CharacterController = self:GetCharacterController()	
	lPos = CharacterController:GetPosition()
    lAStar = g_EnemyManager:GetAStar(self.Room)

    if ( not self.PathCalculated ) then   
        self.Path = lAStar:GetPath( lPos, PositionPlayer )
        self.PathCalculated = true
    end
	
    lTargetPos = lPos
    lTargetPos.y = 0
	lWaypointPos = self.Path:GetResource(self.ActualPathPoint)
	lWaypointPos.y = 0.0
    lDist = lTargetPos:Distance( lWaypointPos )
	
	count = self.Path:size()
	
    if ( (count - self.ActualPathPoint) > 2 ) then
        if ( lDist < 0.6 ) then
            self.ActualPathPoint = self.ActualPathPoint + 1
		end
        lTargetPos = self.Path:GetResource(self.ActualPathPoint)
    end
    lTargetPos.y = 0
	
	self:MoveToWaypoint(lTargetPos)
    
    if ( self.Path:GetResource(count - 1):Distance( PositionPlayer ) > 5.0 ) then
        self.PathCalculated = false
		self.ActualPathPoint = 1
	end
end


function CEnemyLUA:MoveToWaypoint(PositionPlayer)
	local dt = timer:GetElapsedTime()
	CharacterController = self:GetCharacterController()
	local ActualPos = Vect3f(CharacterController:GetPosition())
	local FinalPos = Vect3f()
	if PositionPlayer == Vect3f(0.0) then
		FinalPos = Vect3f(self.Waypoints[self.ActualWaypoint])
	else		
		FinalPos = PositionPlayer
	end
	FinalPos.y = 0
	ActualPos.y = 0
	local Dir = FinalPos - ActualPos	
	Dir.y = 0.0
	if CheckVector(Dir) then
		Dir:Normalize()
	end
	DirYaw = math.atan2( Dir.z, Dir.x )
	Yaw = CharacterController:GetYaw()
	YawDif = DirYaw - Yaw
    PrevYaw = Yaw
		
	local YawDifCom = DirYaw - 2*g_Pi - Yaw
	if math.abs( YawDifCom ) < math.abs( YawDif ) then
		YawDif = YawDifCom
	else
		local YawDifCom = DirYaw + 2*g_Pi - Yaw
		if math.abs( YawDifCom ) < math.abs( YawDif ) then
			YawDif = YawDifCom
		end
	end
		
    if ( math.abs( YawDif ) < 0.1 ) then
		CharacterController:Move( Dir * self.Velocity, dt )
		self.RenderableObject:ChangeAnimation(self.CurrentAnimation, 0.2, 1)
		Yaw = DirYaw;
    else
        CharacterController:Move( Vect3f( 0.0 ), dt )
		if YawDif > 0 then
			YawDif = 1
			self.RenderableObject:ChangeAnimation("turn_left", 0.2, 1)
		else
			YawDif = -1
			self.RenderableObject:ChangeAnimation("turn_right", 0.2, 1)
		end
        Yaw = Yaw + (YawDif * self.TurnSpeed * dt)

        if ( ( Yaw < DirYaw and PrevYaw > DirYaw ) or ( Yaw > DirYaw and PrevYaw < DirYaw ) ) then
            Yaw = DirYaw
		end
    end
	
    CharacterController:SetYaw( Yaw )
    self:SetYaw( Yaw )
end

function CEnemyLUA:IsInWaypoint()
	CharacterController = self:GetCharacterController()
	local ActualPos = CharacterController:GetPosition()
	local FinalPos = self.Waypoints[self.ActualWaypoint]
	FinalPos.y = 0.0
	ActualPos.y = 0.0
	local DistVector = FinalPos - ActualPos
	local Distance = DistVector:Length()
	return Distance < self.Delta
end

function CEnemyLUA:GetVelocity()
	return self.Velocity
end

function CEnemyLUA:SetVelocity(velocity)
	self.Velocity = velocity
	self.RenderableObject:SetVelocity(self.Velocity)
end

function CEnemyLUA:GetRoom()
	return self.Room
end

function CEnemyLUA:ChangeRoute(position)
	self.Alarmado = true
	self.ActualPathPoint = 1
	self.PathCalculated = false
	self.PositionAlarm = position[1]
end

function CEnemyLUA:ChangeAnimation(animation, delayin, delayout)
	engine:Trace("Next animation: "..animation)
	if not self.IsHurting then
		engine:Trace("Is not hurting.")
		self.CurrentAnimation = animation
		self.RenderableObject:ChangeAnimation(animation, delayin, delayout)
	end
end

function CEnemyLUA:RotateToPlayer()
	local dt = timer:GetElapsedTime()
	CharacterController = self:GetCharacterController()
	local ActualPos = Vect3f(CharacterController:GetPosition())
	local FinalPos = g_Player:GetPosition()
	FinalPos.y = 0
	ActualPos.y = 0
	local Dir = FinalPos - ActualPos	
	Dir.y = 0.0
	if CheckVector(Dir) then
		Dir:Normalize()
	end
	DirYaw = math.atan2( Dir.z, Dir.x )
	Yaw = CharacterController:GetYaw()
	YawDif = DirYaw - Yaw
    PrevYaw = Yaw
		
	local YawDifCom = DirYaw - 2*g_Pi - Yaw
	if math.abs( YawDifCom ) < math.abs( YawDif ) then
		YawDif = YawDifCom
	else
		local YawDifCom = DirYaw + 2*g_Pi - Yaw
		if math.abs( YawDifCom ) < math.abs( YawDif ) then
			YawDif = YawDifCom
		end
	end
		
    if ( math.abs( YawDif ) < 0.1 ) then
		Yaw = DirYaw;
    else
        CharacterController:Move( Vect3f( 0.0 ), dt )
		if YawDif > 0 then
			YawDif = 1
		else
			YawDif = -1
		end
        Yaw = Yaw + (YawDif * self.TurnSpeed * dt)

        if ( ( Yaw < DirYaw and PrevYaw > DirYaw ) or ( Yaw > DirYaw and PrevYaw < DirYaw ) ) then
            Yaw = DirYaw
		end
    end
	
    CharacterController:SetYaw( Yaw )
    self:SetYaw( Yaw )
end

function CEnemyLUA:SetIsHurting()
	if self.Life > 0 then
		self.IsHurting = true
		countdowntimer_manager:SetActive(self.Name.."HurtTime", true)
	end
end

function CEnemyLUA:CheckHurting()
	if self.Life > 0 then
		if self.IsHurting then
			if countdowntimer_manager:IsActive(self.Name.."HurtTime") then
				if countdowntimer_manager:isTimerFinish(self.Name.."HurtTime") then
					self.IsHurting = false
					countdowntimer_manager:Reset(self.Name.."HurtTime", false)					
				end
			end
		end
	else
		self.IsHurting = false
	end
end