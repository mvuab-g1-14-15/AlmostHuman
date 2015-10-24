dofile("./data/scripts/PlayerController.lua")
dofile("./data/scripts/Blaster.lua")
dofile("./data/scripts/Grenade.lua")
dofile("./data/scripts/StealthAttack.lua")

class "CPlayer"

function CPlayer:__init()
	self.PlayerController = CPlayerController()
	
	self.GrenadeQueue = {}
	self.GrenadeQueue[#self.GrenadeQueue + 1] = CGrenade()
	self.GrenadeQueue[#self.GrenadeQueue + 1] = CGrenade()
	
	self.Blaster = CBlaster()
	self.StealthAttack = CStealthAttack()
	self.Grenade = nil
	
	self.InsideBarrel = false
	self.BarrelName = ""
	
	self.MeshOffset = Vect3f(0.5, 0.0, 0.0)
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource("Player")
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel("Player", "Player")
		self.RenderableObject:SetRoomName( "sala1" )
		renderable_objects_manager_characters:AddResource("Player", self.RenderableObject)
	end

	local l_MeshPosition = self.PlayerController:GetPosition()
	l_MeshPosition.y = l_MeshPosition.y - self.PlayerController:GetHeight() * 2.0
	self.RenderableObject:SetPosition(l_MeshPosition);
	self.RenderableObject:SetYaw(-self.PlayerController:GetYaw() + 4*g_HalfPi);
	self.RenderableObject:MakeTransform();
	engine:Trace("He entrado en el init sala1")
	
	self.Animation = "idle"
	self.Life = 100.0
	self.Room = "sala1"
	
	self.Light = light_manager:GetResource( "PlayerLight" );
	
	if self.Light == nil then
		self.Light = CreateOmniLight()
		self.Light:SetName("PlayerLight" )
		self.Light:SetIntensity( 1 )
		self.Light:SetEndRangeAttenuation( 1.0 )
		self.Light:SetColor( CColor(0.5, 0.5, 0.5, 1.0 ) )
		self.Light:SetPosition( Vect3f(0, 0, 0)	)
		self.Light:SetRenderShadows( false )
	end
	light_manager:AddResource(self.Light:GetName(), self.Light)
	
	--self.BillboardPlayer = billboard_manager:CreateInstance("blash", Vect3f(0, 0, 0), true)
end

function CPlayer:Update()
	self.PlayerController:Update()
	
	local l_MeshOffset = self:GetMeshOffset()
	local l_MeshPosition = self.PlayerController:GetPosition()
	self.Light:SetPosition( l_MeshPosition );
	
	l_MeshPosition = l_MeshPosition + l_MeshOffset
	self.RenderableObject:SetPosition(l_MeshPosition)
	self.RenderableObject:SetYaw(-self.PlayerController:GetYaw() + g_HalfPi)
	
	self.RenderableObject:SetPitch(camera_manager:GetCurrentCamera():GetPitch())

	self.RenderableObject:MakeTransform();
	self.BlasterPosition = self.RenderableObject:GetBonePosition("CATRigRArmPalm")
	self.Blaster:Update(self.BlasterPosition)
	--self.BillboardPlayer:ChangePosition( self.BlasterPosition )
	
	if not self.Blaster:GetIsShooting() then
		if self.PlayerController:GetIsMoving() then
			if self.PlayerController:GetIsRunning() then
				self:SetAnimation("run")
			else
				self:SetAnimation("walk")
			end
		else
			self:SetAnimation("idle")
		end
	end
	
	if not g_ConsoleActivate and not g_CinematicActive then
		if action_manager:DoAction("ThrowGrenade") and #self.GrenadeQueue > 0 then
		   self.Grenade = table.remove(self.GrenadeQueue, 1)
		   self.Grenade:Throw()
		end
		
		if action_manager:DoAction("ExplodeGrenade") and self.Grenade ~= nil then
			self.Grenade:Explode()
			self.Grenade = nil
		end
		
		if self.InsideBarrel then
			if action_manager:DoAction("EnterBarrel") then
				self:ExitBarrel()
			end
		end
	end
	
	self.StealthAttack:Update()
	if self.Life <= 0 then
		FunctionGameOver()
		self.PlayerController:SetPosition(Respawn())
		self.Life= 100.0
		enemy_manager:Reinit(self.Room)
	end
	
	if self.Animation == "shoot" or self.Animation == "charge_loop" then
		self.RenderableObject:ChangeAnimationAction(self.Animation, 0.5, 0)
	else
		self.RenderableObject:ChangeAnimation(self.Animation, 0.5, 0)
	end
end

function CPlayer:SetRoom( aName )
	self.Room = aName
end

function CPlayer:SetAnimation( aName )
	self.Animation = aName
end

function CPlayer:GetMeshOffset()
	return self.PlayerController:GetMeshOffset( self.MeshOffset )
end
function CPlayer:SetPosition(position)
	self.PlayerController:SetPosition(position)
end

function CPlayer:AddDamage(amount)
	if self.InsideBarrel then
		lBarrel = g_Barrels[self.BarrelName]
		lBarrel:SetIsSafe(false)
		self:ExitBarrel()
	else
		if self.PlayerController.Weak then
			amount =  25.0
		end
		self.Life = self.Life - amount
	end
	--engine:Trace("Life: ".. self.Life)
end

function CPlayer:SetWeak(weak)
	self.PlayerController.Weak = weak
end

function CPlayer:GetLife()
	return self.Life
end

function CPlayer:GetBlaster()
	return self.Blaster
end

function CPlayer:GetPosition()
	return self.PlayerController:GetPosition()
end

function CPlayer:GetYaw()
	return self.PlayerController:GetYaw()
end

function CPlayer:GetHeight()
	return self.PlayerController:GetHeight()
end

function CPlayer:AddEnergy(amount)
	local TotalEnergy = self.Blaster:GetEnergy()+amount
	if TotalEnergy > 100.0 then
		TotalEnergy = 100.0
	end
	self.Blaster:SetEnergy(TotalEnergy)
end

function CPlayer:GetRenderableObject()
	return self.RenderableObject;
end

function CPlayer:SetEnergy(amount)
	self.Blaster:SetEnergy(amount)
end

function CPlayer:HideInBarrel( aName )
	if not self.InsideBarrel then
		--engine:Trace("No estoy oculto")
		local l_Can = true
		if not self.PlayerController.Crouch then
			--engine:Trace("No estoy agachado")
			l_Can = self.PlayerController:MakeCrouch()
		end
		
		if l_Can then
			--engine:Trace("Puedo entrar en el barril")
			scene_renderer_commands_manager:SetVisibleCommand("InsideBarrel", true)
			self.InsideBarrel = true
			self.BarrelName = aName
		end
		return l_Can
	else
		local l_Can = true
		if self.PlayerController.Crouch then
			l_Can = self.PlayerController:MakeCrouch()
		end
		
		if l_Can then
			scene_renderer_commands_manager:SetVisibleCommand("InsideBarrel", false)
			self.InsideBarrel = false
		end
		return l_Can
	end
end

function CPlayer:ExitBarrel()
	--engine:Trace("BarrelName = "..self.BarrelName)
	if self:HideInBarrel( self.BarrelName ) then
		lBarrel = g_Barrels[self.BarrelName]
		lBarrel:ExitBarrel(self:GetPosition())
		self.BarrelName = ""
	end
end

function CPlayer:GetIsHidden()
	if self.InsideBarrel then
		lBarrel = g_Barrels[self.BarrelName]
		if lBarrel:GetIsSafe() then
			return not self.PlayerController:GetIsMoving()
		else
			return false
		end
	else
		return false
	end
end

function CPlayer:GetIsInBarrel()
	return self.InsideBarrel
end

function CPlayer:GetActualBarrel()
	return g_Barrels[self.BarrelName];
end

function CPlayer:GetIsRunning()
	return self.PlayerController:GetIsRunning()
end

function CPlayer:GetIsCrouch()
	return self.PlayerController:GetIsCrouch()
end