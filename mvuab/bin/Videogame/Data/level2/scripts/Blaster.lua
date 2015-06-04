
class 'CBlaster'

function CBlaster:__init()
    self.TimePressed = 0.0
    self.MaxTimePressed = 3.0
	
	self.BaseDamage = 5.0
	self.MaxDamage = 20.0
	
	self.IsAcumulatorSound = false
	
	self.Energy = 100.0
	self.Multiplicador = 2
	
    engine:Trace("Blaster initialized")
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
		engine:Trace("Enemy -> Actual HP: " .. lEnemy:GetLife() .. " Damage: " .. damage)
	end
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
			if self.TimePressed < self.MaxTimePressed then
				--Implementar soot acumulado
				self.TimePressed = self.TimePressed + timer:GetElapsedTime()
			else
				self.TimePressed = self.MaxTimePressed
			end
			if self.TimePressed  > (self.MaxTimePressed * 0.1) and not self.IsAcumulatorSound then 
				sound_manager:PlayEvent( "Acumulator_Long_Shoot_Event", "TestGameObject2d" )
				

				self.IsAcumulatorSound = true
			end
		end
		if action_manager:DoAction("ShootUp") then
			if self.Energy > 1 then
				if self.TimePressed < (self.MaxTimePressed * 0.1) then
					sound_manager:PlayEvent( "Shoot", "TestGameObject2d" )
					self.Energy = self.Energy - 1
				else
					sound_manager:PlayEvent( "Shoot_Long_Shoot_Event", "TestGameObject2d" )
					self.Energy = self.Energy - (self.TimePressed*self.Multiplicador)
					engine:Trace("Tiempo pulsado: ".. tostring(self.TimePressed).." Energia consumida: "..tostring(self.TimePressed*self.Multiplicador))
				end
				engine:Trace("Energia Total: ".. tostring(self.Energy))
				self:Shoot()
			else
				--Sonido de no munición
			end
			self.TimePressed = 0.0
			self.IsAcumulatorSound = false
		end
	end
end

function CBlaster:GetEnergy()
	return self.Energy
end

function CBlaster:SetEnergy(amount)
	self.Energy = amount
end