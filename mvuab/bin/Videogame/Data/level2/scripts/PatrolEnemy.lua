dofile("./data/level2/scripts/Enemy.lua")

class 'CPatrolEnemyLUA' (CEnemyLUA)

function CPatrolEnemyLUA:__init(Node, waypoints, state_machine, core_enemy)
	CEnemyLUA.__init(self, Node, state_machine, core_enemy)
	self.Waypoints = waypoints
	self.ActualWaypoint = 1
	self.ActualPathPoint = 1
	self.TurnSpeed = 1
	self.Delta = 0.2
	self.PathCalculated = false
	self.Path = vecVect3f()
	--engine:Trace("CPatrolEnemyLUA: " .. CEnemyLUA.GetName(self) .. " initialized")
end

function CPatrolEnemyLUA:Destroy()
	CEnemyLUA.Destroy(self)
end

function CPatrolEnemyLUA:Update()
	CEnemyLUA.Update(self)
end

function CPatrolEnemyLUA:GetPosition()
	return self:GetCharacterController():GetPosition()
end

function CPatrolEnemyLUA:NextWaypoint()
	self.ActualWaypoint = self.ActualWaypoint + 1
	if self.ActualWaypoint > #self.Waypoints then
		self.ActualWaypoint = 1
	end
end

function CPatrolEnemyLUA:MoveToWaypoint(PositionPlayer)
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

function CPatrolEnemyLUA:MoveToPlayer(PositionPlayer) --this better in the CEnemy base class
	
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

function CPatrolEnemyLUA:IsInWaypoint()
	CharacterController = CEnemyLUA.GetCharacterController(self)
	local ActualPos = CharacterController:GetPosition()
	local FinalPos = self.Waypoints[self.ActualWaypoint]
	FinalPos.y = 0.0
	ActualPos.y = 0.0
	local DistVector = FinalPos - ActualPos
	local Distance = DistVector:Length()
	return Distance < self.Delta
end

