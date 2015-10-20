class 'CAmmo'
--Clase que implementa la primera rafaga del disparo de logan

function CAmmo:__init()
	engine:Trace("ammo ctor")
	self.Active = false
	self.BillboardAmmo = billboard_manager:CreateInstance("ammo", Vect3f(0, 0, 0), false)
	self.Light = CreateOmniLight()
	self.Light:SetName("AmmoLight")
    self.Light:SetIntensity( 0.65 )
    self.Light:SetEndRangeAttenuation( 2.0 )
    self.Light:SetColor( CColor(0.0, 0.0, 1.0, 1.0 ) )
    self.Light:SetPosition( Vect3f(0, 0, 0)	)
    self.Light:SetRenderShadows( false )
	light_manager:AddResource(self.Light:GetName(), self.Light)
	
	self.mShoot = CreateInstanceMesh("AmmoSolid", "Blaster");
	scene:GetResource("core"):GetLayer("solid"):AddResource(self.mShoot:GetName(), self.mShoot);
	self.mShootGlow = CreateInstanceMesh("AmmoGlow", "Blaster");
	scene:GetResource("core"):GetLayer("glow"):AddResource(self.mShootGlow:GetName(), self.mShootGlow);

	self.mShoot:SetPosition( Vect3f(0, 0, 0));
	self.mShootGlow:SetPosition( Vect3f(0, 0, 0));
	
	self.mShoot:MakeTransform();
    self.mShootGlow:MakeTransform();
	
	self.Impacted = false;
	self.MaxDistance = 100;
	self.CurrentDistance = 0;
end

function CAmmo:Impacted()
	return self.Impacted;
end

function CAmmo:Begin( aPosition, aDirection, aSpeed )
	engine:Trace( "ammo is "..aPosition:ToString() )
	self.Active = true
	
	-- set the position to the data
	self.mShoot:ChangePosition(aPosition);
	self.mShootGlow:ChangePosition(aPosition);
	self.BillboardAmmo:ChangePosition( aPosition );
	self.Light:SetPosition( aPosition );
	
	--all must be visible
	self.Light:ChangeVisibility( true );
	self.mShootGlow:ChangeVisibility( true );
	self.mShoot:ChangeVisibility( true );
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
	self.mShootGlow:ChangeVisibility( false )
	self.mShoot:ChangeVisibility( false )
	self.BillboardAmmo:ChangeVisibility( false )
end

function CAmmo:Update()
	if not g_ConsoleActivate and not g_CinematicActive and self.Active then
		if not self.Impacted then
			dt              = timer:GetElapsedTime()
			lVelocity 		= self.Direction * self.Speed * dt
			lLength 		= lVelocity:Length()
			lNewPosition 	= self.Position + lVelocity
		
			hit_info = physic_manager:RaycastCollisionGroup( self.Position, self.Direction, 0xffffff, 200.0 );
			engine:Trace("hit_info_name->"..hit_info:GetName())
			self.CurrentDistance = self.CurrentDistance + self.Position:Length(lNewPosition);
			if not (hit_info.Distance == 0.0) then
				lCollisionPoint = Vect3f(hit_info.CollisionPoint)
				lDistance = lCollisionPoint:Distance( lNewPosition )
				if ( lDistance < lLength ) then
					self.Impacted = true
					self.Position = lCollisionPoint
				else
					self.Position = lNewPosition
				end
			else
				self.Position = lNewPosition
			end
			
			-- If the ammo has not impacted to something, do not allow to be updated
			if self.CurrentDistance > self.MaxDistance then
				self.Impacted = true
			end
			
			-- set the position to the data
			self.mShoot:ChangePosition(self.Position);
			self.mShootGlow:ChangePosition(self.Position);
			self.BillboardAmmo:ChangePosition( self.Position );
			self.Light:SetPosition( self.Position );
		end
	end
end
