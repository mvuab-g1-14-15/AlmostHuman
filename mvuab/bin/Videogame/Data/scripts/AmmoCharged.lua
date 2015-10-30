class 'CAmmoCharged'

function CAmmoCharged:__init()
	self.Active 	     = false 
	self.Impacted 		 = false;
	self.Speed			 = 15;
	self.ChargedParticle = CParticle( "AmmoChargedParticle", "blaster", "core" )
	self.CurrentDistance = 0
end

function CAmmoCharged:IsActive()
	return self.Active;
end

function CAmmoCharged:IsImpacted()
	return self.Impacted;
end

function CAmmoCharged:Begin( aPosition, aDirection, aDamage )
	self.Active 	     = true;
	self.Impacted 		 = false;
	self.Direction       = aDirection;
	self.Position        = aPosition;
	self.Damage 		 = aDamage;
	self.CurrentDistance = 0;
	self.ChargedParticle:Init( aPosition );
	self.ChargedParticle:SetDirection( aDirection );
end

function CAmmoCharged:Update( aPosition )
	if not g_ConsoleActivate and not g_CinematicActive and self.Active then
		if not self.Impacted then
			self.ChargedParticle:ChangePosition( aPosition )
			dt              = timer:GetElapsedTime()
			lVelocity 		= self.Direction * self.Speed * dt
			lLength 		= lVelocity:Length()
			local lNewPosition 	= self.Position + lVelocity
			hit_info = physic_manager:RaycastCollisionGroup( self.Position, self.Direction, 0xffffff, 200.0 );
			local lNewVector = lNewPosition - self.Position;
			self.CurrentDistance = self.CurrentDistance + lNewVector:Length();
			if not (hit_info.Distance == 0.0) then
				lCollisionPoint = Vect3f(hit_info.CollisionPoint)
				lDistance = lCollisionPoint:Distance( lNewPosition )
				if ( lDistance < lLength ) then
					self.Impacted = true
					self.Position = lCollisionPoint
					local lName = hit_info.Name
					if lName == "Player" then
						g_Player:AddDamage(self.Damage)
					else
						enemy_manager:AddDamage(lName, self.Damage)
					end
				else
					self.Position = lNewPosition
				end
			else
				self.Position = lNewPosition
			end
		end
	end
end
