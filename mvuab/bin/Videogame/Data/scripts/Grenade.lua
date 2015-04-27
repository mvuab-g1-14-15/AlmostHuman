
class 'CGrenade'

function CGrenade:__init()
    self.TimeToExplode = 0.0
    self.MaxDamage = 80.0
    
    self.MinDistantce = 1.0
	self.MaxDistantce = 5.0
    
    self.Position = Vect3f()
    self.Velocity = Vect3f()
    self.Acceleration = Vect3f(0.0, -9.81, 0.0)
    
    self.ThrowAngle = 30.0 * g_Pi / 180.0
	
	self.Exploded = false
    self.Throwed = false
end

function CGrenade:CalculateDamage(l_DistanceToEnemy)
    return self.MaxDamage * ( 1.0 - math.max(0.0, ( l_DistanceToEnemy - self.MinDistantce ) / (self.MaxDistantce - self.MinDistantce ) ) )
end

function CGrenade:Throw()
    self.Position = camera_manager:GetCurrentCamera():GetPosition()
    self.Velocity = camera_manager:GetCurrentCamera():GetDirection() * 1.0
    
    self.Throwed = true
    self:TimeToBoom()
end

function CGrenade:TimeToBoom()
    self.TimeToExplode = self.TimeToExplode - timer:GetElapsedTime()
    
    if(self.TimeToExplode <= 0.0) then
		l_PhysXActors = vPUD(physic_manager:OverlapSphereController(self.MaxDistantce, self.Position, 2 ^ 2))
		l_Size = l_PhysXActors:size()
		self.Exploded = true
		
		for i = 1, l_Size do
			l_Enemy = enemy_manager:GetResource(l_PhysXActors:GetResource(i - 1):GetName())
			engine:Trace("Damage: " .. self:CalculateDamage((self.Position - l_Enemy:GetPosition()):Length()))
		end		
    end
end

function CGrenade:Update()
    if action_manager:DoAction("ThrowGrenade") and not self.Throwed then
        self:Throw()
        return
    end
    
    if not self.Throwed then
		return
	end
	
	if self.Exploded then
		return
	end
    
	l_Dt = timer:GetElapsedTime()
    self.Velocity = self.Velocity + self.Acceleration * l_Dt
    self.Position = self.Position + self.Velocity * l_Dt
    
    self:TimeToBoom()
end