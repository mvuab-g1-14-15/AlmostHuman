
class 'CStealthAttack'

function CStealthAttack:__init()
	self.Doing = false
	
	self.MaxDistance = 1.0
	self.MaxAngle = 45.0
	
    engine:Trace("StealthAttack initialized")
end

function CStealthAttack:Update()
	self:UpdateInput()
	
	if self.Doing then
		local enemy = physic_manager:GetClosestEnemy()
		if enemy == nil then
			self.Doing = false
			return
		end
			
		local dist = PlayerDistance(enemy)
		if dist < self.MaxDistance then
			local vPlayerEnemy = enemy:GetPosition() - GetPlayerPosition()
			vPlayerEnemy:Normalize()
			local enemyDirection = enemy:GetDirection()
			
			local angle = GetAngleEnemyPlayer(enemy)
			
			engine:Trace("Angle: " .. angle)
		end
	end
	
	self.Doing = false
end

function CStealthAttack:UpdateInput()
	if action_manager:DoAction("StealthAttack") then
		self.Doing = true
		engine:Trace("Stealth attack press")
	end
end