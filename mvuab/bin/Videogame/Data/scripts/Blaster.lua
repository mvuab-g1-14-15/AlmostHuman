dofile("./data/scripts/Blash.lua")

class 'CBlaster'

function CBlaster:__init()
    self.TimePressed = 0.0
    self.MaxTimePressed = 1.0
	
	self.BaseDamage = 20.0
	self.MaxDamage = 70.0
	
	self.IsAcumulatorSound = false
	
	self.Energy = 100.0
	self.Multiplicador = 5
	
	self.ShootSpeed = 50.0
	
	self.Shoots = {}
	
	self.IsCharging = false
	FinishShooting = false
	
	self.Blash = CBlash( "Player" )
	self.Ammunition = {}
	self.MaxAmmo = 25;
	self.AmmoId = 0
	for i=1,self.MaxAmmo do
		table.insert( self.Ammunition, CAmmo(i, "ammo") )
		self.AmmoId = self.AmmoId + 1
	end
	
	self.PlayerIsShooting = false
	
	self.AmmoCharged = CAmmoCharged()
end

function CBlaster:CalculateDamage()
    local l_Percentage = self.TimePressed / self.MaxTimePressed
	local damage = (1 - l_Percentage) * self.BaseDamage + l_Percentage * self.MaxDamage
	if l_Percentage == 1 then
		damage = self.MaxDamage + self.MaxDamage / 2.0
	end
	return damage
end

function CBlaster:HasAmmo()
	if #self.Ammunition == 0 then
		return false
	else
		return true
	end
end

function CBlaster:UpdateAmmo()
	for i=1,#self.Ammunition do
		lAmmo = self.Ammunition[i]
		lAmmo:Update()
	end
end

function CBlaster:EndAmmo()
	for i=1,#self.Ammunition do
		lAmmo = self.Ammunition[i]
		lAmmo:End()
	end
end

function CBlaster:Shoot( aPosition )
	self.Blash:Begin(aPosition)
	local lAlreadyShoot = false;
	for i=1,#self.Ammunition do
		lAmmo = self.Ammunition[i];
		if not lAmmo:IsActive() and not lAlreadyShoot then
			local lDirection = camera_manager:GetCurrentCamera():GetDirection()
			lAmmo:Begin( aPosition, lDirection, 30.0, self:CalculateDamage() );
			lAlreadyShoot = true;
		end
		-- Update the impacted ones
		if lAmmo:IsImpacted() then
			lAmmo:End();
		end
	end
end

function CBlaster:ShootCharged( aPosition )
	self.Blash:Begin(aPosition)
	local lDirection = camera_manager:GetCurrentCamera():GetDirection();
	self.AmmoCharged:Begin( aPosition , lDirection, self:CalculateDamage() );
end

function CBlaster:GetEnemyFromRay()
	local l_OriRay = camera_manager:GetCurrentCamera():GetPosition()
	local l_DirRay = camera_manager:GetCurrentCamera():GetDirection()
    l_DirRay:Normalize()
	local l_ImpactMask = BitOr(2 ^ CollisionGroup.ECG_ENEMY.value, 2 ^ CollisionGroup.ECG_ESCENE.value)
	local l_EnemyName = physic_manager:RaycastClosestActorName(l_OriRay, l_DirRay, l_ImpactMask)
    return enemy_manager:GetResource(l_EnemyName)
end

function CBlaster:Update( aPosition )
	if not g_ConsoleActivate and not g_CinematicActive then
		if self.Energy > 1 and not g_Player:GetIsCrouch() then
			if action_manager:DoAction("ShootDown") and not self.IsCharging then
				g_Player:SetAnimation("fast_shoot")
				self.Energy = self.Energy - 1
				sound_manager:PlayEvent( "Play_Short_Shoot_Event", "Logan" )
				self.PlayerIsShooting = true;
				self:Shoot( aPosition )
			elseif action_manager:DoAction("ShootUp") then
				self.PlayerIsShooting = false;
			elseif action_manager:DoAction("ShootChargedDown") and not self.IsCharging then
				self.IsCharging = true
				self.TimePressed = 0;
				g_Player:SetAnimation("charge")
				sound_manager:PlayEvent( "Play_Acumulator_Long_Shoot_Event", "Logan" )
			elseif action_manager:DoAction("ShootChargedUp") and self.IsCharging then
				self.IsCharging = false
				self.Energy = self.Energy - (self.TimePressed*self.Multiplicador)
				self:ShootCharged( aPosition )
				sound_manager:PlayEvent( "Play_Long_Shoot_Event", "Logan" )
			end
		else
			if not g_Player:GetIsCrouch() then
				if action_manager:DoAction("ShootDown") or action_manager:DoAction("ShootChargedUp") then
					sound_manager:PlayEvent( "Play_No_Shoot_Logan", "Logan" )
				end
			end
		end
		
		if self.IsCharging == true then
			if self.MaxTimePressed > self.TimePressed then
				self.TimePressed = self.TimePressed + g_FrameTime
			else
				self.TimePressed = self.MaxTimePressed
			end
		end
		
		if self.Energy < 1 then
			self.Energy = 0;
		end
	end
	
	--Update the blash
	local lDirection = camera_manager:GetCurrentCamera():GetDirection()
	self.Blash:Update( aPosition );
	self:UpdateAmmo();
	self.AmmoCharged:Update( aPosition + lDirection* 0.1 )
end

function CBlaster:GetIsCharging()
	return self.IsCharging
end

function CBlaster:GetIsShooting()
	return self.PlayerIsShooting; 
end

function CBlaster:GetEnergy()
	return self.Energy
end

function CBlaster:SetEnergy(amount)
	self.Energy = amount
end
