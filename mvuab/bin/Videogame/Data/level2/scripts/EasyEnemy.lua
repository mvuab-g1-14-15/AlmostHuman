dofile("./data/level2/scripts/Enemy.lua")

class 'CEasyEnemyLUA' (CEnemyLUA)

function CEasyEnemyLUA:__init(Node, waypoints, state_machine, core_enemy)
	CEnemyLUA.__init(self, Node, state_machine, core_enemy)
	-- self.IdRouteAlarm = Node:GetAttributeInt("route_alarm", -1)
	-- self.ActualPathPoint = 1
	-- self.TurnSpeed = 1
	-- self.Delta = 0.2
	-- self.Speed = 5.0
	-- self.PathCalculated = false
	-- self.Path = vecVect3f()
	self.YawPlayerMove = 0
	self.YawInitial = 0
	self.YawDirection = 1
	self.MidRangeDegree = 45
	
	self.Waypoints = waypoints
	--self.IdRouteAlarm = Node:GetAttributeInt("route_alarm", -1)
	self.ActualWaypoint = 1
	self.ActualPathPoint = 1
	self.PathCalculated = false
	self.TurnSpeed = 1
	self.Delta = 0.2
	self.Path = vecVect3f()
	self.PositionStay = Node:GetAttributeVect3f("pos", Vect3f(0,0,0))
	--engine:Trace("CEasyEnemyLUA: " .. CEnemyLUA.GetName(self) .. " initialized")
end

function CEasyEnemyLUA:Destroy()
	CEnemyLUA.Destroy(self)
end

function CEasyEnemyLUA:Update()
	--if CEnemyLUA.GetActualState(self) ~= "atacar" and CEnemyLUA.GetActualState(self) ~= "perseguir" then
	--	if self.YawPlayerMove >= 360.0 then
	--		self.YawPlayerMove = 0
	--	end
	--	self.YawPlayerMove = self.YawPlayerMove + timer:GetElapsedTime()
	--	lCharacterController = CEnemyLUA.GetCharacterController(self)
	--	lCharacterController:SetYaw(lCharacterController:GetYaw() + 22.5*math.sin(g_Pi*self.YawPlayerMove/18.0))
	--end
	
	CEnemyLUA.Update(self)
end

function CEasyEnemyLUA:GetPosition()
	return self:GetCharacterController():GetPosition()
end

function CEasyEnemyLUA:NextWaypoint()
	self.ActualWaypoint = self.ActualWaypoint + 1
	if self.ActualWaypoint > #self.Waypoints then
		self.ActualWaypoint = 1
	end
end

-- function CEasyEnemyLUA:MoveToWaypoint(PositionPlayer)
	-- local dt = timer:GetElapsedTime()
	-- CharacterController = CEnemyLUA.GetCharacterController(self)
	-- local ActualPos = Vect3f(CharacterController:GetPosition())
	-- local FinalPos = Vect3f()
	-- local Dir = Vect3f(0.0)	
	-- local Yaw = CharacterController:GetYaw()
	-- PositionPlayer.y = 0
	-- if CheckVector(PositionPlayer) == false then
		-- PositionPlayer.x = self.PositionStay.x
		-- PositionPlayer.y = 0
		-- PositionPlayer.z = self.PositionStay.z
	-- end
	-- local ActualPosAux = ActualPos
	-- ActualPosAux.y = 0
	-- --engine:Trace("Move Player to position"..PositionPlayer:ToString())
	-- --engine:Trace("Posición actual "..ActualPos:ToString())
	-- --engine:Trace("Distancia entre la posicion y el destino"..PositionPlayer:Distance(ActualPos))
	-- if PositionPlayer:Distance(ActualPosAux) > 0.5 then
		-- if self.YawPlayerMove >= 0 then
			-- self.YawPlayerMove = -1
		-- end
		-- FinalPos = PositionPlayer
		-- FinalPos.y = 0
		-- ActualPos.y = 0
		-- Dir = FinalPos - ActualPos	
		-- Dir.y = 0.0
		-- if CheckVector(Dir) then
			-- Dir:Normalize()
		-- end
		-- DirYaw = math.atan2( Dir.z, Dir.x )
		-- YawDif = DirYaw - Yaw
		-- PrevYaw = Yaw
			
		-- local YawDifCom = DirYaw - 2*g_Pi - Yaw
		-- if math.abs( YawDifCom ) < math.abs( YawDif ) then
			-- YawDif = YawDifCom
		-- else
			-- local YawDifCom = DirYaw + 2*g_Pi - Yaw
			-- if math.abs( YawDifCom ) < math.abs( YawDif ) then
				-- YawDif = YawDifCom
			-- end
		-- end
			
		-- if ( math.abs( YawDif ) < 0.1 ) then
			-- CharacterController:Move( Dir * self.Speed, dt )
			-- Yaw = DirYaw;
		-- else
			-- CharacterController:Move( Vect3f( 0.0 ), dt )
			-- if YawDif > 0 then
				-- YawDif = 1
			-- else
				-- YawDif = -1
			-- end
			-- Yaw = Yaw + (YawDif * self.TurnSpeed * dt)

			-- if ( ( Yaw < DirYaw and PrevYaw > DirYaw ) or ( Yaw > DirYaw and PrevYaw < DirYaw ) ) then
				-- Yaw = DirYaw
			-- end
		-- end
		-- CharacterController:SetYaw( Yaw )
	-- else
		-- CharacterController:Move( Vect3f( 0.0 ), dt )
		-- if self.YawPlayerMove == -1 then
			-- self.YawInitial = Yaw
			-- self.YawPlayerMove = 0
		-- end
		-- if self.YawPlayerMove >= g_DoublePi then
			-- self.YawPlayerMove = 0
		-- end
		-- self.YawPlayerMove = self.YawPlayerMove + dt

		-- Yaw = self.YawInitial + g_Pi*self.MidRangeDegree*math.sin(self.YawPlayerMove)/180.0
	-- end
	
	-- CEnemyLUA.SetYaw( self, Yaw )
    
    
-- end

-- function CEasyEnemyLUA:MoveToPlayer(PositionPlayer)
	
	-- CharacterController = CEnemyLUA.GetCharacterController(self)	
	-- lPos = CharacterController:GetPosition()
    -- lAStar = g_EnemyManager:GetAStar()

    -- if ( not self.PathCalculated ) then   
        -- self.Path = lAStar:GetPath( lPos, PositionPlayer )
        -- self.PathCalculated = true
    -- end
	
    -- lTargetPos = lPos
    -- lTargetPos.y = 0
	-- lWaypointPos = self.Path:GetResource(self.ActualPathPoint)
	-- lWaypointPos.y = 0.0
    -- lDist = lTargetPos:Distance( lWaypointPos )
	
	-- count = self.Path:size()
	
    -- if ( (count - self.ActualPathPoint) > 2 ) then
        -- if ( lDist < 0.6 ) then
            -- self.ActualPathPoint = self.ActualPathPoint + 1
		-- end
        -- lTargetPos = self.Path:GetResource(self.ActualPathPoint)
    -- end
    -- lTargetPos.y = 0
	
	-- self:MoveToWaypoint(lTargetPos)
    
    
    -- if ( self.Path:GetResource(count - 1):Distance( PositionPlayer ) > 5.0 ) then
        -- self.PathCalculated = false
		-- self.ActualPathPoint = 1
	-- end
-- end

-- function CEasyEnemyLUA:ChangeRoute(waypoints)
	-- self.Waypoints = waypoints
	-- self.ActualWaypoint = 1
	-- self.ActualPathPoint = 1
	-- self.PathCalculated = false
-- end

