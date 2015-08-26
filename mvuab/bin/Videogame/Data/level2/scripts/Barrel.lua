class "CBarrel"

function CBarrel:__init(aPosition, aName)
	self.Pos = aPosition
	self.Name = aName
	self.Radius = 0.2
	self.Height = 0.8
	
	physic_manager:AddActorCapsule(self.Name, self.Radius, self.Height)
	self.Actor = physic_manager:GetActor(self.Name)
	self.Actor:MoveGlobalPosition( aPosition )
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateAnimatedInstanceModel(self.Name, "Barrel")
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	engine:Trace("CBarrel: " .. self.Name .. " initialized")
end

function CBarrel:Destroy()
	physic_manager:ReleasePhysicActor(self.Actor)
	renderable_objects_manager_characters:RemoveResource(self.Name)
	engine:Trace("CBarrel: " .. self.Name .. " destroyed")
end