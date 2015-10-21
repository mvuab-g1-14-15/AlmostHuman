class 'CAmmo'
--Clase que implementa la primera rafaga del disparo de logan

function CAmmo:__init( aId )
	self.Id = aId;
	engine:Trace("ammo ctor".. self.Id )
	self.Active = false
	self.BillboardAmmo = billboard_manager:CreateInstance("ammo", Vect3f(0, 0, 0), false)
	self.Light = CreateOmniLight()
	self.Light:SetName("AmmoLight_".. self.Id )
    self.Light:SetIntensity( 0.65 )
    self.Light:SetEndRangeAttenuation( 1.0 )
    self.Light:SetColor( CColor(0.5, 0.5, 1.0, 1.0 ) )
    self.Light:SetPosition( Vect3f(0, 0, 0)	)
    self.Light:SetRenderShadows( false )
	light_manager:AddResource(self.Light:GetName(), self.Light)
	self.Impacted = false;
	self.MaxDistance = 20;
	self.CurrentDistance = 0;
end

function CAmmo:IsActive()
	return self.Active;
end

function CAmmo:IsImpacted()
	return self.Impacted;
end

function CAmmo:Begin( aPosition, aDirection, aSpeed )
	engine:Trace( "ammo is "..aPosition:ToString() )
	self.Active = true
	self.Impacted = false;
	
	self.BillboardAmmo:ChangePosition( aPosition );
	self.Light:SetPosition( aPosition );
	
	--all must be visible
	self.Light:ChangeVisibility( true );
	self.BillboardAmmo:ChangeVisibility( true );
	
	self.Direction       = aDirection;
	self.Position        = aPosition;
	self.Speed	         = aSpeed;
	self.CurrentDistance = 0;
end

function CAmmo:End()
	engine:Trace("ammo end")
	self.Active = false;
	
	--hide all the elements
	self.Light:ChangeVisibility( false )
	self.BillboardAmmo:ChangeVisibility( false )
end

function CAmmo:Update()
	if not g_ConsoleActivate and not g_CinematicActive and self.Active then
		if not self.Impacted then
			dt              = timer:GetElapsedTime()
			lVelocity 		= self.Direction * self.Speed * dt
			lLength 		= lVelocity:Length()
			local lNewPosition 	= self.Position + lVelocity
		
			hit_info = physic_manager:RaycastCollisionGroup( self.Position, self.Direction, 0xffffff, 200.0 );
			--engine:Trace("hit_info_name->"..hit_info:GetName())
			local lNewVector = lNewPosition - self.Position;
			self.CurrentDistance = self.CurrentDistance + lNewVector:Length();
			if not (hit_info.Distance == 0.0) then
				lCollisionPoint = Vect3f(hit_info.CollisionPoint)
				lDistance = lCollisionPoint:Distance( lNewPosition )
				if ( lDistance < lLength ) then
					self.Impacted = true
					engine:Trace("Impacted to something")
					self.Position = lCollisionPoint
				else
					self.Position = lNewPosition
				end
			else
				self.Position = lNewPosition
			end
			
			-- If the ammo has not impacted to something, do not allow to be updated
			if self.CurrentDistance > self.MaxDistance then
				engine:Trace("Impacted max distance")
				self.Impacted = true
				self:End();
			end
			
			-- set the position to the data
			self.BillboardAmmo:ChangePosition( self.Position );
			self.Light:SetPosition( self.Position );
		end
	end
end
