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