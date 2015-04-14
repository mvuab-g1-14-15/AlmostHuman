
class 'CGrenade'

function CGrenade:__init()
    self.m_TimeToExplode = 10.0
    self.m_MaxDamage = 80.0
    
    self.m_MinDistantce = 1.0
	self.m_MaxDistantce = 5.0
    
    self.m_Position = Vect3f()
    self.m_Velocity = Vect3f()
    self.m_Acceleration = Vect3f(0.0, -9.81, 0.0)
    
    self.m_ThrowAnle = 30.0 * 3.141592 / 180.0
    self.m_Throwed = false
	
    core:Trace("CGrenade initialized")
end

function CGrenade:CalculateDamage()
    local l_DistanceToEnemy = 1.0
    
    if(l_DistanceToEnemy < self.m_MinDistantce) return self.m_MaxDamage
    else if(l_DistanceToEnemy > self.m_MaxDistantce) return 0.0
    
    return self.m_MaxDamage * (l_DistanceToEnemy / self.m_MaxDistantce)
end

function CGrenade:Throw()
    self.m_Position = camera_manager:GetCurrentCamera():GetPosition()
    self.m_Velocity = camera_manager:GetCurrentCamera():GetDirection() * 10.0
    
    self.m_Throwed = true
    self.TimeToExplode()
end

function CGrenade:TimeToExplode()
    self.m_TimeToExplode = self.m_TimeToExplode - timer:GetElapsedTime()
    
    if(self.m_TimeToExplode <= 0.0)
    
    end
end

function CGrenade:Update()
    if action_manager:DoAction("ThrowGrenade") then
        self.Throw()
        do return end
    end
    
    if(!self.m_Throwed) return end
    
    self.m_Velocity = self.m_Velocity + self.m_Acceleration * dt
    self.m_Position = self.m_Position + self.m_Velocity * dt;
    
    self.TimeToExplode()
end