class "CBarrel"

function CBarrel:__init(aPosition,aName,core_barrel)

	self.Pos = Position
	self.Name = aName
	self.Type = "Barrel"
	self.Radius = 0.4
	self.Height = 2.0
	physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), CollisionGroup.ECG_ENEMY.value, -10.0)
	self.CharacterController = physic_manager:GetController(self.Name)
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, Node:GetAttributeString("mesh", "default_mesh"))
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	
	local l_MeshPosition = self.CharacterController:GetPosition()
	l_MeshPosition.y = l_MeshPosition.y - self.Height/2
	self.RenderableObject:SetPosition(l_MeshPosition)
	self.RenderableObject:SetYaw(-self.CharacterController:GetYaw() + g_HalfPi)
	self.RenderableObject:SetPitch(self.CharacterController:GetPitch())
	self.RenderableObject:SetRoll(self.CharacterController:GetRoll())
	self.RenderableObject:MakeTransform()

	AddEnemy(Node:GetAttributeString("texture_enemy", ""), self.Pos, Node:GetAttributeInt("width", 50.0), Node:GetAttributeInt("height", 50.0), self.CharacterController:GetYaw(), Node:GetAttributeString("get_position_script", "no_script"), Node:GetAttributeString("orientation", "no_script"), self.Name)
	
	engine:Trace("CEnemyLUA: " .. self.Name .. " initialized")
end

function CBarrel:CreateBarrel(aPos)
	

end

function CBarrel:Destroy()
	physic_manager:ReleasePhysicController(self.CharacterController)
	renderable_objects_manager_characters:RemoveResource(self.Name)
	engine:Trace("He muerto Vida actual: "..self.Life)
end