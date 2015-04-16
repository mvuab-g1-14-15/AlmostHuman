
class 'CBlaster'

function CBlaster:__init()
    self.TimePressed = 0.0
    self.MaxTimePressed = 3.0
	
	self.BaseDamage = 5.0
	self.MaxDamage = 20.0
	
    core:Trace("Blaster initialized")
end

function CBlaster:CalculateDamage()
    local l_Percentage = self.TimePressed / self.MaxTimePressed
	local damage = (1 - l_Percentage) * self.BaseDamage + l_Percentage * self.MaxDamage
	if l_Percentage == 1 then
		damage = self.MaxDamage + self.MaxDamage / 2.0
	end
	return damage
end

function CBlaster:Shoot()
	local lEnemy = self:GetEnemyFromRay()
    
	if lEnemy ~= nil then
		local damage = self:CalculateDamage()
		lEnemy:AddDamage( damage )
		core:Trace("Enemy -> Actual HP: " .. lEnemy:GetLife() .. " Damage: " .. damage)
	end
end

function CBlaster:GetEnemyFromRay()
	local l_OriRay = camera_manager:GetCurrentCamera():GetPosition()
	local l_DirRay = camera_manager:GetCurrentCamera():GetDirection()
    l_DirRay:Normalize()
	self:CreateParticles(l_OriRay, l_DirRay)
	local l_ImpactMask = 2 ^ CollisionGroup.ECG_ENEMY.value
	local l_EnemyName = physic_manager:RaycastClosestActorName(l_OriRay, l_DirRay, l_ImpactMask)
    return enemy_manager:GetResource(l_EnemyName)
end

function CBlaster:CreateParticles(position, direction)
	l_Emitter = particle_manager:CreateTrailEmitter()
	l_Emitter:SetTextureName( "Data/textures/red_smoke.png" )
	
	l_Emitter:SetActive( true )
    l_Emitter:SetEmitterLifeTime(10)
    l_Emitter:SetLifeTime(1.0, 5.0)
	l_Emitter:SetTimeToEmit(0.1);
	
    l_Emitter:SetAcceleration(Vect3f(0.0))
    l_Emitter:SetPosition(position)
    l_Emitter:SetVelocity(direction)
    
	l_Emitter:SetSize(0.1, 0.5)
    l_Emitter:SetPitch(0.0, 180.0)
    l_Emitter:SetYaw(0.0, 360.0)
	
	l_Emitter:SetSize(0.1, 1.0)
	l_Emitter:SetRandom(1, 5)
    l_Emitter:Generate(1)
	particle_manager:AddEmitter( l_Emitter )
end

function CBlaster:Update()
    if action_manager:DoAction("Shoot") then
		if self.TimePressed < self.MaxTimePressed then
			self.TimePressed = self.TimePressed + timer:GetElapsedTime()
		else
			self.TimePressed = self.MaxTimePressed
		end
	end
    if action_manager:DoAction("ShootUp") then
		self:Shoot()
		self.TimePressed = 0.0
	end
end