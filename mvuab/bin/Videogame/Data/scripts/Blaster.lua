
class 'CBlaster'

function CBlaster:__init()
    self.m_TimePressed = 0.0
    self.m_MaxTimePressed = 5 
    Singleton_Core.get_singleton():trace("Hello CBlaster INIT")
end

function CBlaster:CalculateDamage( aOriShoot, aEnemyPosition )
    return 10;
end

function CBlaster:Shoot()
	local lEnemy = self:GetEnemyFromRay()
    
	if lEnemy ~= nil then
		local damage = CBlaster:CalculateDamage( Singleton_Core.get_singleton():GetCameraManager():GetCurrentCamera():GetPos(), lEnemy:GetPosition() );
		lEnemy:AddDamage( damage )
		Singleton_Core.get_singleton():trace("Enemy -> Actual HP: " .. lEnemy:GetLife() .. " Damage: " .. damage)
	end
end

function CBlaster:IsMaxTime()
	if( m_TimePressed > m_MaxTimePressed) then 
		return true 
	else 
		return false 
	end
end

function CBlaster:ApplyDamage(l_EnemyName, damage)
	local l_enemy = Singleton_Core.get_singleton():GetEnemyManager():getEnemy(l_EnemyName)
end

function CBlaster:GetEnemyFromRay()
	local l_OriRay = Singleton_Core.get_singleton():GetCameraManager():GetCurrentCamera():GetPos()
	local l_DirRay = Singleton_Core.get_singleton():GetCameraManager():GetCurrentCamera():GetDirection()
    l_DirRay:Normalize()
	local l_ImpactMask = 2 ^ CollisionGroup.ECG_ENEMY.value
	local l_EnemyName = Singleton_Core.get_singleton():GetPhysicsManager():RaycastClosestActorName(l_OriRay, l_DirRay, l_ImpactMask)
    Singleton_Core.get_singleton():trace(l_EnemyName)
    return Singleton_Core.get_singleton():GetEnemyManager():getEnemy(l_EnemyName)
end

function CBlaster:Update()
	local l_ActionManager = Singleton_Core.get_singleton():GetActionManager()
    
    if l_ActionManager:DoAction("Shoot") then
		CBlaster:Shoot()
	end
end