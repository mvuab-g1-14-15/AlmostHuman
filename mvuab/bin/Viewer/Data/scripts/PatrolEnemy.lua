dofile("./data/scripts/Enemy.lua")

class 'CPatrolEnemyLUA' (CEnemyLUA)

function CPatrolEnemyLUA:__init(waypoints)
	CEnemyLUA.__init(self, waypoints[1])
	
	self.Waypoints = waypoints
	self.ActualWaypoint = 1
	
	self.Delta = 0.05
	
	engine:Trace("CPatrolEnemyLUA: " .. self.Name .. " initialized")
end

function CPatrolEnemyLUA:Update()
	CEnemyLUA.Update(self)
	if self:IsInWaypoint() then
		self:NextWaypoint()
	end
	self:MoveToWaypoint()
end

function CPatrolEnemyLUA:NextWaypoint()
	self.ActualWaypoint = self.ActualWaypoint + 1
	if self.ActualWaypoint > #self.Waypoints then
		self.ActualWaypoint = 1
	end
end

function CPatrolEnemyLUA:MoveToWaypoint()
	local dt = timer:GetElapsedTime()
	
	local ActualPos = self.CharacterController:GetPosition()
	local FinalPos = self.Waypoints[self.ActualWaypoint]
	local Dir = FinalPos - ActualPos
	Dir.y = 0.0
	Dir:Normalize()
	self.CharacterController:Move(Dir, dt)
	
	--engine:Trace("Actual position " .. self.CharacterController:GetPosition():ToString())
end

function CPatrolEnemyLUA:IsInWaypoint()
	local ActualPos = self.CharacterController:GetPosition()
	local FinalPos = self.Waypoints[self.ActualWaypoint]
	
	local DistVector = FinalPos - ActualPos
	local Distance = DistVector:Length()
	
	return Distance < self.Delta
end