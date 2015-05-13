class "CEnemyLUA"

function CEnemyLUA:__init(position)
	self.Life = 100
	
	self.Name = "Enemy" .. id_manager:GetId()
	
	self.Radius = 0.4
	self.Height = 2.0
	self.Position = position
	
	physic_manager:AddController(self.Name, self.Radius, self.Height/2.0, 0.2, 0.05, 0.5, self.Position, CollisionGroup.ECG_ENEMY.value, -490)
	self.CharacterController = physic_manager:GetController(self.Name)
	
	engine:Trace("Enemy initialized")
end

function CEnemyLUA:Update()

end

function CEnemyLUA:AddLife(amount)
	self.Life = self.Life + amount
end