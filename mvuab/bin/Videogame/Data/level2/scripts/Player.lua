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
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource("Player")
	if self.RenderableObject == nil then
		self.RenderableObject = CreateInstanceMesh("Player", "Player")
		renderable_objects_manager_characters:AddResource("Player", self.RenderableObject)
	end

	local l_MeshPosition = self.PlayerController:GetPosition()
	l_MeshPosition.y = l_MeshPosition.y - self.PlayerController:GetHeight() * 2.0
	self.RenderableObject:SetPosition(l_MeshPosition);
	self.RenderableObject:SetYaw(-self.PlayerController:GetYaw() + g_HalfPi);

	self.RenderableObject:MakeTransform();
	
	self.Life = 100.0
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
	end
	
	self.Blaster:Update()
	self.StealthAttack:Update()
	if self.Life <= 0 then
		self.PlayerController:SetPosition(Respawn())
		self.Life= 100.0
	end
	--engine:Trace( "Player life: " .. self.Life )
end

function CPlayer:SetPosition(position)
	self.PlayerController:SetPosition(position)
end

function CPlayer:AddDamage(amount)
	self.Life = self.Life - amount
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