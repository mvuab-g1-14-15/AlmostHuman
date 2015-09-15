dofile("./data/level2/scripts/PlayerController.lua")
dofile("./data/level2/scripts/Blaster.lua")
dofile("./data/level2/scripts/Grenade.lua")
dofile("./data/level2/scripts/StealthAttack.lua")

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
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource("Player")
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel("Player", "Player")
		self.RenderableObject:SetRoomName( "sala1" )
		renderable_objects_manager_characters:AddResource("Player", self.RenderableObject)
	end

	local l_MeshPosition = self.PlayerController:GetPosition()
	l_MeshPosition.y = l_MeshPosition.y - self.PlayerController:GetHeight() * 2.0
	self.RenderableObject:SetPosition(l_MeshPosition);
	self.RenderableObject:SetYaw(-self.PlayerController:GetYaw() + g_HalfPi);

	self.RenderableObject:MakeTransform();
	
	self.Life = 100.0
	
	self.CurrentState = "idle"
end

function CPlayer:Update()
	self.PlayerController:Update()
	
	local l_MeshPosition = self.PlayerController:GetPosition()
	l_MeshPosition.y = l_MeshPosition.y - self.PlayerController:GetHeight() * 2.0
	self.RenderableObject:SetPosition(l_MeshPosition);
	self.RenderableObject:SetYaw(-self.PlayerController:GetYaw() + g_HalfPi);

	self.RenderableObject:MakeTransform();
	
	if not g_ConsoleActivate and not g_CinematicActive then
		if action_manager:DoAction("ThrowGrenade") and #self.GrenadeQueue > 0 then
		   self.Grenade = table.remove(self.GrenadeQueue, 1)
		   self.Grenade:Throw()
		end
		
		if action_manager:DoAction("ExplodeGrenade") and self.Grenade ~= nil then
			self.Grenade:Explode()
			self.Grenade = nil
		end
		
		if action_manager:DoAction("LyingDown") then
			self:HideInBarrel()
		end
		
		if self.InsideBarrel then
			if action_manager:DoAction("EnterBarrel") then
				self:ExitBarrel()
			end
		end
	end
	
	self.Blaster:Update()
	self.StealthAttack:Update()
	if self.Life <= 0 then
		self.PlayerController:SetPosition(Respawn())
		self.Life= 100.0
	end
	
	
	----engine:Trace( "Player life: " .. self.Life )
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
		self.Life = self.Life - amount
	end
	--engine:Trace("Life: ".. self.Life)
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

function CPlayer:GetCurrentState()
	return self.CurrentState
end

function CPlayer:SetCurrentState(state)
	self.CurrentState = state
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