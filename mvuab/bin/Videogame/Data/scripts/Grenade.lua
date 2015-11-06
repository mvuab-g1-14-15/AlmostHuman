
class 'CGrenade'

function CGrenade:__init()
    self.MaxDamage = 80.0
    
    self.MinDistantce = 1.0
	self.MaxDistantce = 5.0
	
	self.Exploded = false
    self.Throwed = false
	
	self.Actor = nil
end

function CGrenade:CalculateDamage(l_DistanceToEnemy)
    return self.MaxDamage * (1.0 - math.max(0.0, (l_DistanceToEnemy - self.MinDistantce) / (self.MaxDistantce - self.MinDistantce)))
end

function CGrenade:Throw()
	AddGrenade(physic_manager, "Grenade", "Sphere", Vect3f(0.1, 0.1, 0.1), camera_manager:GetCurrentCamera():GetPosition(), 1)
	self.Actor = physic_manager:GetActor("Grenade")
	
	self.Actor:AddImpulseAtPos(camera_manager:GetCurrentCamera():GetDirection(), camera_manager:GetCurrentCamera():GetPosition(), 3.0, true)
	self.Throwed = true
end

function CGrenade:Explode()
	l_PhysXActors = vPUD(physic_manager:OverlapSphereController(self.MaxDistantce, self.Actor:GetPosition(), 2 ^ 2))
	l_Size = l_PhysXActors:size()
	self.Exploded = true
	
	for i = 1, l_Size do
		l_Enemy = enemy_manager:GetResource(l_PhysXActors:GetResource(i - 1):GetName())
		l_Enemy:AddDamage(self:CalculateDamage((self.Actor:GetPosition() - l_Enemy:GetPosition()):Length()))
	end
	------engine:Trace( "BOOOM " )
	--action_manager:ReleasePhysicActor(self.Actor)	
end
