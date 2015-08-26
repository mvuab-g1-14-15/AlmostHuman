class "CEnemyLUA"

function CEnemyLUA:__init(Node, state_machine, core_enemy)

	self.Life = core_enemy:GetLife()
	self.RespawnTime = core_enemy:GetRespawnTime()
    self.TimeToShoot = core_enemy:GetTimeToShoot()
    self.ShootAccuracy = core_enemy:GetShootAccuracy()
	self.Damage = core_enemy:GetDamage()
	self.CountTimeShoot = 0.0
	self.Name = Node:GetAttributeString("name", "no_name")
	self.Type = Node:GetAttributeString("type", "no_enemy")
	self.Radius = 0.4
	self.Height = 2.0
	self.PitchCameraMove = 0.0
	physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), CollisionGroup.ECG_ENEMY.value, -10.0)
	self.CharacterController = physic_manager:GetController(self.Name)
	
	self.CharacterController:SetYaw(Node:GetAttributeFloat("yaw", 0.0))
	
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
	
	
	
	self.Brain = CBrain("inicial", state_machine)
	
	--self:ChangeState("perseguir")
	--self:GetAnimationModel():ChangeAnimation("perseguir", 0.2, 1.0)
	
	camera_manager:NewCamera(CameraType.Free.value, self.Name, Vect3f( 0.0, 1.0, 0.0), Vect3f( 0.0 ))
	self.Camera = camera_manager:GetCamera(self.Name)
	self.Camera:SetZFar(20.0)
	self:UpdateCamera()
	
	AddEnemy(Node:GetAttributeString("texture_enemy", ""), Node:GetAttributeVect3f("pos", Vect3f(0,0,0)), Node:GetAttributeInt("width", 50.0), Node:GetAttributeInt("height", 50.0), self.CharacterController:GetYaw(), Node:GetAttributeString("get_position_script", "no_script"), Node:GetAttributeString("orientation", "no_script"), self.Name)
	
	engine:Trace("CEnemyLUA: " .. self.Name .. " initialized")
end

function CEnemyLUA:Destroy()
	physic_manager:ReleasePhysicController(self.CharacterController)
	renderable_objects_manager_characters:RemoveResource(self.Name)
	engine:Trace("He muerto Vida actual: "..self.Life)
end

function CEnemyLUA:Update()
	self:SetMeshTransform()
	self.Brain:Update()
	self:UpdateCamera()
end

function CEnemyLUA:UpdateCamera()
	lPosition = self.CharacterController:GetPosition()
	lPosition.y = lPosition.y + self:GetHeight()
	lPosition = lPosition + self:GetDirection()
	self.Camera:SetPosition(lPosition)
	self.Camera:SetYaw(self.CharacterController:GetYaw())
	if not (self:GetActualState() == "atacar") then
		if self.PitchCameraMove >= 360.0 then
			self.PitchCameraMove = 0
		end
		self.PitchCameraMove = self.PitchCameraMove + timer:GetElapsedTime()
	end
	self.Camera:SetPitch(self.CharacterController:GetPitch() - (g_Pi/18)- (g_Pi*2*math.sin(self.PitchCameraMove)/18.0))
	--self.Camera:SetDirection(self:GetDirectionEnemy())
	self.Camera:MakeTransform()
	self.Camera:UpdateFrustum()
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

function CEnemyLUA:AddDamage(amount)
	self.Life = self.Life - amount
	engine:Trace("Vida actual: "..self.Life)
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
	local Direction = Vect3f( math.cos( Yaw ) * math.cos( Pitch ), math.sin( Pitch ), math.sin( Yaw ) * math.cos( Pitch ) )
	return Direction
end

function CEnemyLUA:GetDirectionEnemy()
	local Yaw = self.CharacterController:GetYaw()
	local Pitch = self.CharacterController:GetPitch()
	local Direction = Vect3f( math.cos( Yaw ) * math.cos( Pitch ), math.sin( Pitch ), math.sin( Yaw ) * math.cos( Pitch ) )
	return Direction
end

function CEnemyLUA:GetCharacterController()
	return self.CharacterController
end

function CEnemyLUA:GetCamera()
	return self.Camera
end

function CEnemyLUA:MakeShoot(aDirection)
	lPosition = self.CharacterController:GetPosition() + aDirection * 0.4
	lPosition.y = lPosition.y + (self:GetHeight() / 2.0)
	lShoot = CShootLUA( 5.0, aDirection, lPosition, self.Damage )	
	g_EnemyManager:AddShoot(lShoot)
end

function CEnemyLUA:GetLife()
	return self.Life
end

function CEnemyLUA:GetBrain()
	return self.Brain
end

function CEnemyLUA:GetPitchCameraMove()
	return self.PitchCameraMove
end

function CEnemyLUA:SetPitchCameraMove(aPitchCameraMove)
	self.PitchCameraMove = aPitchCameraMove
end

function CEnemyLUA:GetType()
	return self.Type
end

function CEnemyLUA:GetDead()
	return self.Life <= 0
end