class 'CPatrolEnemyLUA' (CEnemyLUA)

function CPatrolEnemyLUA:__init(waypoints)
	CEnemyLUA.__init(self)
	
	self.Waypoints = waypoints
	self.ActualWaypoint = 0
	
	self.Delta = 0.01
	
	engine:Trace("Enemy Patrol initialized")
end

function CPatrolEnemyLUA:Update()

end

function CPatrolEnemyLUA:NextWaypoint()
	self.ActualWaypoint = self.ActualWaypoint + 1
	if self.ActualWaypoint >= #self.Waypoints then
		self.ActualWaypoint = 0
	end
end

function CPatrolEnemyLUA:MoveToWaypoint()
	local dt = timer:GetElapsedTime()
	
	local ActualPos = self.CharacterController:GetPosition()
	local FinalPos = self.Waypoints[self.ActualWaypoint]
	local Dir = FinalPos - ActualPos
	Dir:Normalize()
	self.CharacterController:Move(Dir, dt)
end

function IsInWaypoint()
	local ActualPos = self.CharacterController:GetPosition()
	local FinalPos = self.Waypoints[self.ActualWaypoint]
	
	local DistVector = FinalPos - ActualPos
	local Distance = Dist:Length()
	
	return Distance < self.Delta
end