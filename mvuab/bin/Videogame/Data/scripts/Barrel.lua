class "CBarrel"

function CBarrel:__init(aName, aPosition)
	self.Pos = aPosition
	self.Name = aName
	self.Radius = 0.2
	self.Height = 0.8
	
	self.IsSafe = true
	
	self.Trigger = CreateTrigger( self.Name, self.Pos, Vect3f(self.Radius*2.0, self.Height, self.Radius*2.0), true, true, true, "HiddenBarrelOnEnter('"..self.Name.."')", "HiddenBarrelOnEnter('"..self.Name.."')", "HiddenBarrelOnLeave('"..self.Name.."')")
	trigger_manager:AddTrigger( self.Trigger )
	
	--physic_manager:AddActorCapsule(self.Name, self.Radius, self.Height)
	--self.Actor = physic_manager:GetActor(self.Name)
	--self.Actor:MoveGlobalPosition( self.Pos )
	
	self.RenderableObject = renderable_objects_manager_characters:GetResource(self.Name)
	if self.RenderableObject == nil then
		self.RenderableObject = CreateInstanceMesh(self.Name, "Barrel")
		renderable_objects_manager_characters:AddResource(self.Name, self.RenderableObject)
	end
	
	self.RenderableObject:SetPosition(self.Pos)
	self.RenderableObject:MakeTransform()
	
	--engine:Trace("CBarrel: " .. self.Name .. " initialized")
	--engine:Trace("CBarrel Pos: " .. self.Pos:ToString())
end

function CBarrel:Destroy()
	physic_manager:ReleasePhysicActor(self.Actor)
	renderable_objects_manager_characters:RemoveResource(self.Name)
	--engine:Trace("CBarrel: " .. self.Name .. " destroyed")
end

function CBarrel:RestoreBarrel()
	--self.Actor:SetActive( true )
	self.RenderableObject:SetActive( true )
	
	trigger_manager:ModifyTrigger( self.Name, self.Pos, Vect3f(self.Radius*2.0, self.Height, self.Radius*2.0), true, true, true, "HiddenBarrelOnEnter('"..self.Name.."')", "HiddenBarrelOnEnter('"..self.Name.."')", "HiddenBarrelOnLeave('"..self.Name.."')")
	self.Trigger:SetActive(true)
end

function CBarrel:ExitBarrel( aPos )
	--engine:Trace("He pasado por aqui: "..aPos:ToString())
	final_pos = aPos;
	final_pos.y = final_pos.y - self.Height * 0.5
	self.Pos = final_pos
	
	--self.Actor:MoveGlobalPosition( self.Pos )
	
	self.RenderableObject:SetPosition(self.Pos)
	self.RenderableObject:MakeTransform()
	
	trigger_manager:ModifyTrigger( self.Name, self.Pos, Vect3f(self.Radius, self.Height, self.Radius), false, false, true, "", "", "HiddenBarrelExit('"..self.Name.."')")
	self.Trigger:SetActive(true)
end

function CBarrel:SetStateInside()
	--self.Actor:SetActive( false )
	self.RenderableObject:SetActive( false )
	--trigger_manager:ReleaseTrigger(self.Name)
	-- Setear al player dentro del barril
	g_Player:HideInBarrel(self.Name)
	
	g_bInBarrel = false
	
	self.Trigger:SetActive(false)
end

function CBarrel:SetIsSafe( aSafe )
	self.IsSafe = aSafe
end

function CBarrel:GetIsSafe()
	return self.IsSafe
end