class "CEnemyLUA"

function CEnemyLUA:__init() 
	self.Life = 100
	
	physic_manager:AddController("Player" .. id_manager:GetId(), self.Radius, self.Height/2.0, 0.2, 0.5, 0.5, self.Position, CollisionGroup.ECG_PLAYER.value, -490)
	self.CharacterController = physic_manager:GetController("Player")
	
	engine:Trace("Enemy initialized")
end

function CEnemyLUA:Update()

end