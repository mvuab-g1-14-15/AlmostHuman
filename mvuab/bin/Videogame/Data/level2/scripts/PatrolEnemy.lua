dofile("./data/level2/scripts/Enemy.lua")

class 'CPatrolEnemyLUA' (CEnemyLUA)

function CPatrolEnemyLUA:__init(Node, waypoints, state_machine, core_enemy)
	self.Enemy = CEnemyLUA(Node, state_machine, core_enemy)
	self.Waypoints = waypoints
	self.ActualWaypoint = 1
	
	self.Delta = 0.05
	self.PathCalculated = false
	self.Path = vecVect3f()
	engine:Trace("CPatrolEnemyLUA: " .. self.Enemy:GetName() .. " initialized")
end


function CPatrolEnemyLUA:Update()
	self.Enemy:Update()
end

function CPatrolEnemyLUA:GetPosition()
	return self.Enemy:GetCharacterController():GetPosition()
end

function CPatrolEnemyLUA:NextWaypoint()
	self.ActualWaypoint = self.ActualWaypoint + 1
	if self.ActualWaypoint > #self.Waypoints then
		self.ActualWaypoint = 1
	end
end

function CPatrolEnemyLUA:MoveToWaypoint(PositionPlayer)
	local dt = timer:GetElapsedTime()
	CharacterController = self.Enemy:GetCharacterController()
	local ActualPos = Vect3f(CharacterController:GetPosition())
	local FinalPos = Vect3f()
	engine:Trace("traza 1")
	if PositionPlayer == Vect3f(0.0) then
		engine:Trace("traza 2")	
		FinalPos = self.Waypoints[self.ActualWaypoint]
	else
		engine:Trace("traza 3")
		FinalPos = PositionPlayer
	end
	
	engine:Trace("traza 4")
	local Dir = FinalPos - ActualPos	
	engine:Trace("traza 5")
	Dir.y = 0.0
	engine:Trace("traza 5")
	Dir:Normalize()
	engine:Trace("traza 5")
	DirYaw = aTan2( Dir.z, Dir.x )
	Yaw = CharacterController:GetYaw()
	YawDif = DirYaw - Yaw
    PrevYaw = Yaw
	
	engine:Trace("traza 6")
    if ( Abs( YawDif ) < 0.1 ) then
		engine:Trace("traza 7")
        CharacterController:Move( Dir * 2.0, dt )
    else
		engine:Trace("traza 8")
        CharacterController:Move( Vect3f( 0.0 ), dt )
		if YawDif > 0 then
			YawDif = 1
		else
			YawDif = -1
		end
        Yaw = Yaw + (YawDif * 1.0 * dt)

        if ( ( Yaw < 0.0 and PrevYaw > 0.0 ) or ( Yaw > 0.0 and PrevYaw < 0.0 ) ) then
            Yaw = DirYaw
		end
    end
	engine:Trace("traza 9")
    CharacterController:SetYaw( Yaw )
    self:SetYaw( Yaw )
	
	--engine:Trace("Actual position " .. self.CharacterController:GetPosition():ToString())
end

function CPatrolEnemyLUA:MoveToPlayer(PositionPlayer)
	CharacterController = self.Enemy:GetCharacterController()	
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
    
	self:MoveToWaypoint(lTargetPos)
    
    PositionPlayer.y = 0.0

    if ( self.Path[self.Path:size() - 1].Distance( PositionPlayer ) > 5.0 ) then
        self.PathCalculated = false
	end
end

function CPatrolEnemyLUA:IsInWaypoint()
	CharacterController = self.Enemy:GetCharacterController()
	local ActualPos = CharacterController:GetPosition()
	local FinalPos = self.Waypoints[self.ActualWaypoint]
	
	local DistVector = FinalPos - ActualPos
	local Distance = DistVector:Length()
	
	return Distance < self.Delta
end

function CPatrolEnemyLUA:AddLife(Amount)
	self.Enemy:AddLife(Amount)
end

function CPatrolEnemyLUA:SetYaw(Yaw)
	self.Enemy:SetYaw(Yaw)
end

function CPatrolEnemyLUA:GetYaw()
	return self.Enemy:GetYaw()
end

function CPatrolEnemyLUA:SetPitch(Pitch)
	self.Enemy:SetPitch(Pitch)
end

function CPatrolEnemyLUA:GetPitch()
	return self.Enemy:GetPitch()
end

function CPatrolEnemyLUA:GetActualState()
	return self.Enemy:GetActualState()
end

function CPatrolEnemyLUA:ChangeState(NewState)
	self.Enemy:ChangeState(NewState)
end

function CPatrolEnemyLUA:GetAnimationModel()
	return self.Enemy:GetAnimationModel()
end

function CPatrolEnemyLUA:GetName()
	return self.Enemy:GetName()
end

function CPatrolEnemyLUA:SetTimeToShoot(aTimeToShoot)
	self.Enemy:SetTimeToShoot(aTimeToShoot)
end

function CPatrolEnemyLUA:GetTimeToShoot()
	return self.Enemy:GetTimeToShoot()
end

function CPatrolEnemyLUA:GetCountTimeShoot()
	return self.Enemy:GetCountTimeShoot()
end

function CPatrolEnemyLUA:SetCountTimeShoot(CountTimeShoot)
	self.Enemy:SetCountTimeShoot(CountTimeShoot)
end

function CPatrolEnemyLUA:GetHeight()
	return self.Enemy:GetHeight()
end

function CPatrolEnemyLUA:GetDirection()
	return self.Enemy:GetDirection()
end