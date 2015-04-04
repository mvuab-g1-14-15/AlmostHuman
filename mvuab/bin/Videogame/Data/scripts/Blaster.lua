
class 'CBlaster'

function CBlaster:__init()
    self.TimePressed = 0.0
    self.MaxTimePressed = 3.0
	
	self.BaseDamage = 5.0
	self.MaxDamage = 20.0
	
    core:trace("Blaster initialized")
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
		core:trace("Enemy -> Actual HP: " .. lEnemy:GetLife() .. " Damage: " .. damage)
	end
end

function CBlaster:GetEnemyFromRay()
	local l_OriRay = camera_manager:GetCurrentCamera():GetPosition()
	local l_DirRay = camera_manager:GetCurrentCamera():GetDirection()
    l_DirRay:Normalize()
	local l_ImpactMask = 2 ^ CollisionGroup.ECG_ENEMY.value
	local l_EnemyName = physic_manager:RaycastClosestActorName(l_OriRay, l_DirRay, l_ImpactMask)
    return enemy_manager:getEnemy(l_EnemyName)
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