class "CBoss"

function CBoss:__init()
	--Basic properties
	self.Name = "Boss"
	
	self.Radius = 1.0
	self.Height = 4.0
	
	self.NearPos = Vect3f(0.0)
	self.MediumPos = Vect3f(0.0)
	self.FarPos = Vect3f(0.0)
	
	--Renderable object
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, "boss")
		self.RenderableObject:SetRoomName( "sala4" )
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	self.RenderableObject:SetScale( Vect3f(0.8) );
	self.RenderableObject:MakeTransform()
	
	--Character controller
	if physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, Vect3f(0,0,0), CollisionGroup.ECG_ENEMY.value, -10.0) == false then 
		physic_manager:ReleasePhysicController(physic_manager:GetController(self.Name))
		physic_manager:AddController(self.Name, self.Radius, (self.Height/2.0)+0.25, 0.2, 0.01, 0.5, Vect3f(0,0,0), CollisionGroup.ECG_ENEMY.value, -10.0)
	end
	self.CharacterController = physic_manager:GetController(self.Name)
	self.CharacterController:SetYaw(0.0)
	self.CharacterController:SetScale(Vect3f( 1.0, 1.0, 1.0))
	
end

function CBoss:Destroy()
end

function CBoss:Update()
	local dt = timer:GetElapsedTime()
	
	self.CharacterController:Move( Vect3f(0.0), dt)
end

-- Getters and setters
function CBoss:GetName()
	return self.Name
end

function CBoss:SetName( aName )
	self.Name = aName
end

function CBoss:GetPosition()
	return self.CharacterController:GetPosition()
end

function CBoss:SetPosition( aPos )
	self.CharacterController:SetPosition( aPos )
	self.RenderableObject:SetPosition( aPos )
	self.RenderableObject:MakeTransform()
end