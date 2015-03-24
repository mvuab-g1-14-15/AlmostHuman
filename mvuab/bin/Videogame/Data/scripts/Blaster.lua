local core = Singleton_Core.get_singleton()
local action_manager = core:GetActionManager()
local camera_manager = core:GetCameraManager()
local timer = core:GetTimer()
local physic_manager = core:GetPhysicsManager()
local enemy_manager = core:GetEnemyManager()

class 'CBlaster'

function CBlaster:__init()
    self.m_TimePressed = 0.0
    self.m_MaxTimePressed = 5 
    core:trace("Hello CBLASTER INIT")
end

function CBlaster:CalculateDamage( aOriShoot, aEnemyPosition )
    return 10;
end

function CBlaster:Shoot()
	local lEnemy = self:GetEnemyFromRay()
	if( lEnemy ~= nil ) then
		local damage = self:CalculateDamage( camera_manager:GetCurrentCamera():GetPos(), lEnemy.GetPos() );
		lEnemy:AddDamage( damage )
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
	local l_enemy = enemy_manager:getEnemy(l_EnemyName)
end

function CBlaster:GetEnemyFromRay()
	local l_OriRay = camera_manager:GetCurrentCamera():GetPos()
	local l_DirRay = camera_manager:GetCurrentCamera():GetLookAt()
	local l_ImpactMask = bit.blshift(1, ECG_ENEMY)

	local l_EnemyName = physic_manager:RaycastClosestActorName(l_OriRay,l_DirRay,l_ImpactMask)
	core:trace(l_EnemyName)
	return enemy_manager:getEnemy(l_EnemyName)
end

function CBlaster:Update()
	local l_ActionManager = Singleton_Core.get_singleton():GetActionManager()
    
    if l_ActionManager:DoAction("Shoot") then
		CBlaster:Shoot()
	end
end