class "CEnemyLUA"

function CEnemyLUA:__init(Node, state_machine, core_enemy)

	self.Life = core_enemy:GetLife()
	self.RespawnTime = core_enemy:GetRespawnTime()
    self.TimeToShoot = core_enemy:GetTimeToShoot()
    self.ShootAccuracy = core_enemy:GetShootAccuracy()
	self.CountTimeShoot = 0.0
	self.Name = "Enemy" .. id_manager:GetId()
	
	self.Radius = 0.4
	self.Height = 2.0
	
	physic_manager:AddController(self.Name, self.Radius, self.Height, 0.2, 0.01, 0.5, Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), CollisionGroup.ECG_ENEMY.value, -10.0)
	self.CharacterController = physic_manager:GetController(self.Name)
	
	self.CharacterController:SetYaw(Node:GetAttributeFloat("yaw", 0.0))
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		engine:Trace("Añadiendo "..self.Name)
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
	
	self.Brain = CBrain("inicial", state_machine)
	
	engine:Trace("CEnemyLUA: " .. self.Name .. " initialized")
end


function CEnemyLUA:Update()
	self:SetMeshTransform()
	self.Brain:Update()
end

function CEnemyLUA:SetMeshTransform()
	local l_MeshPosition = self.CharacterController:GetPosition()
	l_MeshPosition.y = l_MeshPosition.y - self.Height/2
	self.RenderableObject:SetPosition(l_MeshPosition)
	self.RenderableObject:SetYaw(-self.CharacterController:GetYaw() + g_HalfPi)
	self.RenderableObject:SetPitch(self.CharacterController:GetPitch())
	self.RenderableObject:SetRoll(self.CharacterController:GetRoll())
	self.RenderableObject:MakeTransform()
end

function CEnemyLUA:AddLife(amount)
	self.Life = self.Life + amount
end

function CEnemyLUA:SetYaw(yaw)
	self.RenderableObject:SetYaw(-yaw + g_HalfPi)
end

function CEnemyLUA:GetYaw()
	return self.RenderableObject:GetYaw()
end

function CEnemyLUA:SetPitch(pitch)
	self.RenderableObject:SetPitch(pitch)
end

function CEnemyLUA:GetPitch()
	return self.RenderableObject:GetPitch()
end

function CEnemyLUA:GetActualState()
	return self.Brain:GetActualState()
end

function CEnemyLUA:ChangeState(NewState)
	self.Brain:SetNextState(NewState)
end

function CEnemyLUA:GetAnimationModel()
	return self.RenderableObject
end

function CEnemyLUA:GetName()
	return self.Name
end

function CEnemyLUA:SetTimeToShoot(aTimeToShoot)
	self.TimeToShoot = aTimeToShoot
end

function CEnemyLUA:GetTimeToShoot()
	return self.TimeToShoot
end

function CEnemyLUA:GetCountTimeShoot()
	return self.CountTimeShoot
end

function CEnemyLUA:SetCountTimeShoot(CountTimeShoot)
	self.CountTimeShoot = CountTimeShoot
end

function CEnemyLUA:GetHeight()
	return self.CharacterController:GetHeight()
end

function CEnemyLUA:GetDirection()
	local Yaw = self:GetYaw()
	local Pitch = self:GetPitch()
	local Direction = Vect3f( cos( Yaw ) * cos( Pitch ), sin( Pitch ), sin( Yaw ) * cos( Pitch ) )
	return Direction
end

function CEnemyLUA:GetCharacterController()
	return self.CharacterController
end
