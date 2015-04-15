
class 'CGrenade'

function CGrenade:__init()
    self.TimeToExplode = 10.0
    self.MaxDamage = 80.0
    
    self.MinDistantce = 1.0
	self.MaxDistantce = 5.0
    
    self.Position = Vect3f()
    self.Velocity = Vect3f()
    self.Acceleration = Vect3f(0.0, -9.81, 0.0)
    
    self.ThrowAngle = 30.0 * g_Pi / 180.0
    self.Throwed = false
	
    core:Trace("CGrenade initialized")
end

function CGrenade:CalculateDamage()
    local l_DistanceToEnemy = 1.0
    
    if(l_DistanceToEnemy < self.MinDistantce) then
		return self.MaxDamage
    elseif(l_DistanceToEnemy > self.MaxDistantce) then
		return 0.0
	end
    
    return self.MaxDamage * (l_DistanceToEnemy / self.MaxDistantce)
end

function CGrenade:Throw()
    self.Position = camera_manager:GetCurrentCamera():GetPosition()
    self.Velocity = camera_manager:GetCurrentCamera():GetDirection() * 10.0
    
    self.Throwed = true
    self:TimeToExplode()
end

function CGrenade:TimeToExplode()
    self.TimeToExplode = self.TimeToExplode - timer:GetElapsedTime()
    
    if(self.TimeToExplode <= 0.0) then
    end
end

function CGrenade:Update()
    if action_manager:DoAction("ThrowGrenade") then
        self.Throw()
        return
    end
    
    if not self.Throwed then
		return
	end
    
    self.Velocity = self.Velocity + self.Acceleration * dt
    self.Position = self.Position + self.Velocity * dt
    
    self:TimeToExplode()
end