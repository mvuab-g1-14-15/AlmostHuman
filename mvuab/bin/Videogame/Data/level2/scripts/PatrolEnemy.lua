dofile("./data/level2/scripts/Enemy.lua")

class 'CPatrolEnemyLUA' (CEnemyLUA)

function CPatrolEnemyLUA:__init(Node, waypoints, state_machine, core_enemy)
	CEnemyLUA.__init(self, Node, state_machine, core_enemy)
	--self = CEnemyLUA(Node, state_machine, core_enemy)
	self.Waypoints = waypoints
	self.ActualWaypoint = 1
	
	self.Delta = 0.2
	self.PathCalculated = false
	self.Path = vecVect3f()
	engine:Trace("CPatrolEnemyLUA: " .. CEnemyLUA.GetName(self) .. " initialized")
end


function CPatrolEnemyLUA:Update()
	CEnemyLUA.Update(self)
	--self:Update()
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
	
    if ( math.abs( YawDif ) < 0.1 ) then
		CharacterController:Move( Dir * 2.0, dt )
    else
        CharacterController:Move( Vect3f( 0.0 ), dt )
		if YawDif > 0 then
			YawDif = 1
		else
			YawDif = -1
		end
        Yaw = Yaw + (YawDif * 0.60 * dt)

        if ( ( Yaw < 0.0 and PrevYaw > 0.0 ) or ( Yaw > 0.0 and PrevYaw < 0.0 ) ) then
            Yaw = DirYaw
		end
    end
	
    CharacterController:SetYaw( Yaw )
    CEnemyLUA.SetYaw( self, Yaw )
end

function CPatrolEnemyLUA:MoveToPlayer(PositionPlayer)
	CharacterController = CEnemyLUA.GetCharacterController(self)	
	lPos = CharacterController:GetPosition()

    lAStar = g_EnemyManager:GetAStar()

    if ( not self.PathCalculated ) then    
        self.Path = vecVect3f(lAStar:GetPath( lPos, PositionPlayer ))
        self.PathCalculated = true
    end
	
	count = self.Path:size()
	
    for i = 0, count-1 do
        self.Path[i].y = 0.0
	end
	
    lTargetPos = lPos
    lPosAux = lPos
    lPosAux.y = 0.0
    lDist = lPosAux.Distance( self.Path[1] )

    if ( self.Path:size() > 2 ) then
        if ( lDist < 0.6 ) then
            self.Path:erase( self.Path:begin() + 1 )
		end
        lTargetPos = self.Path[1]
    end

    lTargetPos.y = lPos.y
    
	CEnemyLUA.MoveToWaypoint(self, lTargetPos)
    
    PositionPlayer.y = 0.0

    if ( self.Path[self.Path:size() - 1].Distance( PositionPlayer ) > 5.0 ) then
        self.PathCalculated = false
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

