dofile("./data/level2/scripts/Enemy.lua")

class 'CDroneEnemyLUA' (CEnemyLUA)

function CDroneEnemyLUA:__init(Node, waypoints, state_machine, core_enemy)
	CEnemyLUA.__init(self, Node, state_machine, core_enemy)
	self.Waypoints = waypoints
	self.ActualWaypoint = 1
	self.ActualPathPoint = 1
	self.TurnSpeed = 1
	self.Delta = 0.2
	self.PathCalculated = false
	self.Path = vecVect3f()
	self.HeightFly = Node:GetAttributeVect3f("pos", Vect3f(0,0,0)).y
	--engine:Trace("CDroneEnemyLUA: " .. CEnemyLUA.GetName(self) .. " initialized")
end

function CDroneEnemyLUA:Destroy()
	CEnemyLUA.Destroy(self)
end

function CDroneEnemyLUA:Update()
	--UPDATE MESH
	local lCharacterController = CEnemyLUA.GetCharacterController(self)
	local lMeshPosition = lCharacterController:GetPosition()
	lMeshPosition.y = self.HeightFly - CEnemyLUA.GetHeight(self)/2
	lCharacterController:SetPosition(lMeshPosition)
	lRenderableObject = CEnemyLUA.GetAnimationModel(self)
	lRenderableObject:SetPosition(lMeshPosition)
	if CEnemyLUA.GetActualState(self) ~= "atacar" then
		lRenderableObject:SetYaw(-lCharacterController:GetYaw() + g_HalfPi)
		lRenderableObject:SetPitch(lCharacterController:GetPitch())
		lRenderableObject:SetRoll(lCharacterController:GetRoll())
	end
	lRenderableObject:MakeTransform()
	--UPDATE BRAIN
	CEnemyLUA.GetBrain(self):Update()
	--UPDATE CAMERA
	local lPosition = lCharacterController:GetPosition()
	lPosition.y = self.HeightFly - CEnemyLUA.GetHeight(self)
	lPosition = lPosition + CEnemyLUA.GetDirection(self)
	local lCamera = CEnemyLUA.GetCamera(self)
	lCamera:SetPosition(lPosition)
	lCamera:SetYaw(lCharacterController:GetYaw())
	local lPitchCameraMove = CEnemyLUA.GetPitchCameraMove(self)
	if not (CEnemyLUA.GetActualState(self) == "atacar") then
		if lPitchCameraMove >= 360.0 then
			CEnemyLUA.SetPitchCameraMove(self, 0)
		end
		CEnemyLUA.SetPitchCameraMove(self, lPitchCameraMove + timer:GetElapsedTime() )
	end
	lCamera:SetPitch(-g_Pi/4)
	lCamera:MakeTransform()
	lCamera:UpdateFrustum()
end

function CDroneEnemyLUA:GetPosition()
	return self:GetCharacterController():GetPosition()
end

function CDroneEnemyLUA:NextWaypoint()
	self.ActualWaypoint = self.ActualWaypoint + 1
	if self.ActualWaypoint > #self.Waypoints then
		self.ActualWaypoint = 1
	end
end

function CDroneEnemyLUA:MoveToWaypoint(PositionPlayer)
	local dt = timer:GetElapsedTime()
	CharacterController = CEnemyLUA.GetCharacterController(self)
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
		CharacterController:Move( Dir * 5.0, dt )
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
    CEnemyLUA.SetYaw( self, Yaw )
end

function CDroneEnemyLUA:MoveToPlayer(PositionPlayer)
	
	CharacterController = CEnemyLUA.GetCharacterController(self)	
	lPos = CharacterController:GetPosition()
    lAStar = g_EnemyManager:GetAStar()

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

function CDroneEnemyLUA:IsInWaypoint()
	CharacterController = CEnemyLUA.GetCharacterController(self)
	local ActualPos = CharacterController:GetPosition()
	local FinalPos = self.Waypoints[self.ActualWaypoint]
	FinalPos.y = 0.0
	ActualPos.y = 0.0
	local DistVector = FinalPos - ActualPos
	local Distance = DistVector:Length()
	return Distance < self.Delta
end

