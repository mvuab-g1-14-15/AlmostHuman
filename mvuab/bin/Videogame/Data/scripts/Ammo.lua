class 'CAmmo'
--Clase que implementa la primera rafaga del disparo de logan

function CAmmo:__init( aId )
	self.Id = aId;
	self.Active = false
	self.BillboardAmmo = billboard_manager:CreateInstance("ammo", Vect3f(0, 0, 0), false);
	self.BillboardAmmo2 = billboard_manager:CreateInstance("ammo", Vect3f(0, 0, 0), false);
	self.Impacted = false;
	self.MaxDistance = 100;
	self.CurrentDistance = 0;
end

function CAmmo:IsActive()
	return self.Active;
end

function CAmmo:IsImpacted()
	return self.Impacted;
end

function CAmmo:Begin( aPosition, aDirection, aSpeed, aDamage )
	self.Active = true
	self.Impacted = false;
	
	self.BillboardAmmo:ChangePosition( aPosition );
	self.BillboardAmmo2:ChangePosition( aPosition );
	self.BillboardAmmo:ChangeVisibility( true );
	self.BillboardAmmo2:ChangeVisibility( true );
	
	self.Direction       = aDirection;
	self.Position        = aPosition;
	self.Speed	         = aSpeed;
	self.CurrentDistance = 0;
	
	self.Damage = aDamage
end

function CAmmo:End()
	self.Active = false;
	self.BillboardAmmo:ChangeVisibility( false )
	self.BillboardAmmo2:ChangeVisibility( false )
end

function CAmmo:Update()
	if not g_ConsoleActivate and not g_CinematicActive and self.Active then
		if not self.Impacted then
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
					elseif lName == "Boss" then
						lBoss = enemy_manager:GetBoss()
						if lBoss ~= nil then
							lBoss:AddStun(self.Damage)
						end
					else
						enemy_manager:AddDamage(lName, self.Damage)
					end
				else
					self.Position = lNewPosition
				end
			else
				self.Position = lNewPosition
			end
			
			-- If the ammo has not impacted to something, do not allow to be updated
			if self.CurrentDistance > self.MaxDistance then
				----engine:Trace("Impacted max distance")
				self.Impacted = true
				self:End();
			end
			
			-- set the position to the data
			self.BillboardAmmo:ChangePosition( self.Position );
			self.BillboardAmmo2:ChangePosition( self.Position );
		else
			self:End();
		end
	end
end
