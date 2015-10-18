dofile("./data/scripts/Blash.lua")
class 'CBlaster'

function CBlaster:__init()
    self.TimePressed = 0.0
    self.MaxTimePressed = 3.0
	
	self.BaseDamage = 20.0
	self.MaxDamage = 50.0
	
	self.IsAcumulatorSound = false
	
	self.Energy = 100.0
	self.Multiplicador = 2
	
	self.ShootSpeed = 50.0
	
	self.Shoots = {}
	
	self.IsShooting = false
	FinishShooting = false
	countdowntimer_manager:AddTimer("BlasterFinish", 0.2, false)
	
	self.Blash = CBlash()
	
    --engine:Trace("Blaster initialized")
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
	local lDirection = camera_manager:GetCurrentCamera():GetDirection()
	local lPosition = camera_manager:GetCurrentCamera():GetPosition() + lDirection * 0.5
	local lShoot = CShoot(self.ShootSpeed, lDirection, lPosition, self:CalculateDamage())
	lShoot:Init()
	table.insert(self.Shoots, lShoot)
	
	local lEnemies = enemy_manager:GetEnemiesAtDistance( 5.0 )
	local lPlayerPos = g_Player:GetPosition()
	for i = 1, #lEnemies do
		lEnemy = lEnemies[i]
		lEnemy:SetSuspected(true)
		lEnemy:SetSuspectedPosition(lPlayerPos)
	end
	
	self.Blash:Begin(lPosition)

--[[ Old code
	local lEnemy = self:GetEnemyFromRay()
    
	if lEnemy ~= nil then
		local damage = self:CalculateDamage()
		lEnemy:AddDamage( damage )
		--engine:Trace("Enemy -> Actual HP: " .. lEnemy:GetLife() .. " Damage: " .. damage)
	end
	]]
end

function CBlaster:GetEnemyFromRay()
	local l_OriRay = camera_manager:GetCurrentCamera():GetPosition()
	local l_DirRay = camera_manager:GetCurrentCamera():GetDirection()
    l_DirRay:Normalize()
	self:CreateParticles(l_OriRay, l_DirRay)
	local l_ImpactMask = BitOr(2 ^ CollisionGroup.ECG_ENEMY.value, 2 ^ CollisionGroup.ECG_ESCENE.value)
	local l_EnemyName = physic_manager:RaycastClosestActorName(l_OriRay, l_DirRay, l_ImpactMask)
    return enemy_manager:GetResource(l_EnemyName)
end

function CBlaster:CreateParticles(position, direction)
	--l_Emitter = particle_manager:CreateSphereEmitter()
	--l_Emitter:SetTextureName( "Data/textures/red_smoke.png" )
	--
	--l_Emitter:SetActive( true )
    --l_Emitter:SetEmitterLifeTime(10)
    --l_Emitter:SetLifeTime(1.0, 5.0)
	--l_Emitter:SetTimeToEmit(0.1);
	--
    --l_Emitter:SetAcceleration(Vect3f(0.0))
    --l_Emitter:SetPosition(position)
    --l_Emitter:SetVelocity(direction)
    --
	--l_Emitter:SetSize(0.1, 0.5)
    --l_Emitter:SetPitch(0.0, 180.0)
    --l_Emitter:SetYaw(0.0, 360.0)
	--
	--l_Emitter:SetSize(0.1, 1.0)
	--l_Emitter:SetRandom(1, 1)
    --l_Emitter:Generate(1, true)
	--particle_manager:AddEmitter( l_Emitter )
end

function CBlaster:Update()
	if not g_ConsoleActivate and not g_CinematicActive then
		if action_manager:DoAction("Shoot") then
			if self.Energy > 1 then
				self.IsShooting = true
				self.FinishShooting = false
				if self.TimePressed == 0 then
					g_Player:SetAnimation("idle_to_shoot")
				end
				if self.TimePressed < self.MaxTimePressed then
					--Implementar shoot acumulado
					self.TimePressed = self.TimePressed + timer:GetElapsedTime()
				else
					self.TimePressed = self.MaxTimePressed
				end
				if self.TimePressed  > (self.MaxTimePressed * 0.1) and not self.IsAcumulatorSound then 
					sound_manager:PlayEvent( "Play_Acumulator_Long_Shoot_Event", "Logan" )
					g_Player:SetAnimation("charge_loop")
					self.IsAcumulatorSound = true
				end
			end
		end
		if action_manager:DoAction("ShootUp") then
			if self.Energy > 1 then
				if self.TimePressed < (self.MaxTimePressed * 0.1) then
					sound_manager:PlayEvent( "Play_Short_Shoot_Event", "Logan" )
					self.Energy = self.Energy - 1
					
				else
					sound_manager:PlayEvent( "Play_Long_Shoot_Event", "Logan" )
					self.Energy = self.Energy - (self.TimePressed*self.Multiplicador)
				end
				self:Shoot()
				g_Player:SetAnimation("shoot")
			else
			--SONIDO DE PEDO AQUI
			end
			self.TimePressed = 0.0
			self.IsAcumulatorSound = false
			self.FinishShooting = true
			countdowntimer_manager:SetActive("BlasterFinish", true)
		end
		
		if self.FinishShooting then
			if countdowntimer_manager:isTimerFinish("BlasterFinish") then
				self.IsShooting = false
				self.FinishShooting = false
				countdowntimer_manager:Reset("BlasterFinish", false)
			end
		end
			

		for i = #self.Shoots,1,-1 do
			local lShoot = self.Shoots[i]
			lShoot:Update()
			if lShoot:Impacted() then
				table.remove(self.Shoots, i)
				-- Run the garbage collector for remove the shoot in cpp
				collectgarbage()
			end
		end
	end
	
	self.Blash:Update()
end

function CBlaster:GetIsShooting()
	return self.IsShooting
end

function CBlaster:GetEnergy()
	return self.Energy
end

function CBlaster:SetEnergy(amount)
	self.Energy = amount
end
