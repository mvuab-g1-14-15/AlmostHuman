dofile("./data/scripts/PlayerController.lua")
dofile("./data/scripts/Blaster.lua")
dofile("./data/scripts/Grenade.lua")

class "CPlayer"

function CPlayer:__init() 
	self.PlayerController = CPlayerController()
	
	self.GrenadeQueue = {}
	self.GrenadeQueue[#self.GrenadeQueue + 1] = CGrenade()
	self.GrenadeQueue[#self.GrenadeQueue + 1] = CGrenade()
	
	self.Blaster = CBlaster()
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
	
	if action_manager:DoAction("ThrowGrenade") and #self.GrenadeQueue > 0 then
       self.Grenade = table.remove(self.GrenadeQueue, 1)
	   self.Grenade:Throw()
    end
	
	if action_manager:DoAction("ExplodeGrenade") and self.Grenade ~= nil then
		self.Grenade:Explode()
		self.Grenade = nil
	end
	
	self.Blaster:Update()
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