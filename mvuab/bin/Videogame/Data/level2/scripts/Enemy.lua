class "CEnemyLUA"

function CEnemyLUA:__init(Node, state_machine, core_enemy)

	self.Life = core_enemy:GetLife()
	self.RespawnTime = core_enemy:GetRespawnTime()
    self.TimeToShoot = core_enemy:GetTimeToShoot()
    self.ShootAccuracy = core_enemy:GetShootAccuracy()
	
	self.Name = "Enemy" .. id_manager:GetId()
	
	self.Radius = 0.4
	self.Height = 1.8
	
	physic_manager:AddController(self.Name, self.Radius, self.Height/2.0, 0.2, 0.05, 0.5, Node:GetAttributeVect3f("position", Vect3f(0,0,0)), CollisionGroup.ECG_ENEMY.value, -490)
	self.CharacterController = physic_manager:GetController(self.Name)
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateInstanceMesh(self.Name, Node:GetAttributeString("mesh", "default_mesh"))
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	
	local l_MeshPosition = self.CharacterController:GetPosition()
	self.RenderableObject:SetPosition(l_MeshPosition);
	self.RenderableObject:SetYaw(-self.CharacterController:GetYaw() + g_HalfPi);

	self.RenderableObject:MakeTransform();
	
	self.Brain = CBrain("inicial", state_machine)
	
	engine:Trace("CEnemyLUA: " .. self.Name .. " initialized")
end


function CEnemyLUA:Update()
	self:SetMeshTransform()
end

function CEnemyLUA:SetMeshTransform()
	local l_MeshPosition = self.CharacterController:GetPosition()
	self.RenderableObject:SetPosition(l_MeshPosition);
	self.RenderableObject:SetYaw(-self.CharacterController:GetYaw() + g_HalfPi);

	self.RenderableObject:MakeTransform();
end

function CEnemyLUA:AddLife(amount)
	self.Life = self.Life + amount
end