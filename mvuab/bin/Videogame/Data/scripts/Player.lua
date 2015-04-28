dofile("./data/scripts/PlayerController.lua")

class "CPlayer"

function CPlayer:__init() 
	self.PlayerController = CPlayerController()
	
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