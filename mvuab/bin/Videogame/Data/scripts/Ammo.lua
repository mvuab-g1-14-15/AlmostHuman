class 'CAmmo'
--Clase que implementa la primera rafaga del disparo de logan

function CAmmo:__init()
	engine:Trace("ammo ctor")
	self.Active = false
	self.BillboardAmmo = billboard_manager:CreateInstance("ammo", Vect3f(0, 0, 0), false)
	self.Light = CreateOmniLight()
	self.Light:SetName(self.Name)
    self.Light:SetIntensity( 0.65 )
    self.Light:SetEndRangeAttenuation( 2.0 )
    self.Light:SetColor( CColor(0.0, 0.0, 1.0, 1.0 ) )
    self.Light:SetPosition( self.Position	)
    self.Light:SetRenderShadows( false )
	light_manager:AddResource(self.Name, self.Light)
	
	self.mShoot = CreateInstanceMesh(self.Name, "Blaster");
	scene:GetResource("core"):GetLayer("solid"):AddResource(self.Name, self.mShoot);
	self.mShootGlow = CreateInstanceMesh(self.Name, "Blaster");
	scene:GetResource("core"):GetLayer("glow"):AddResource(self.Name, self.mShootGlow);

	self.mShoot:SetPosition(position);
	self.mShootGlow:SetPosition(position);
	
	self.mShoot:MakeTransform();
    self.mShootGlow:MakeTransform();
end

function CAmmo:Begin( aPosition )
	engine:Trace( "ammo is "..aPosition:ToString() )
	self.Active = true
	
	-- set the position to the data
	self.mShoot:SetPosition(position);
	self.mShootGlow:SetPosition(position);
	self.BillboardAmmo:ChangePosition( aPosition )
	self.Light:SetPosition( aPosition )
	
	-- Transform the renderable objects
	self.mShoot:MakeTransform();
    self.mShootGlow:MakeTransform();
	
	--all must be visible
	self.Light:ChangeVisibility( true )
	self.mShootGlow:ChangeVisibility( true )
	self.mShoot:ChangeVisibility( true )
	self.BillboardAmmo:ChangeVisibility( true )
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
	engine:Trace("ammo update ")
	if not g_ConsoleActivate and not g_CinematicActive and self.Active then
		engine:Trace("ammo update inside")
		-- set the position to the data
		self.mShoot:SetPosition(position);
		self.mShootGlow:SetPosition(position);
		self.BillboardAmmo:ChangePosition( aPosition )
		self.Light:SetPosition( aPosition )

		-- Transform the renderable objects
		self.mShoot:MakeTransform()
		self.mShootGlow:MakeTransform()
	end
end
