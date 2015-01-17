function init ()
	core = Singleton_Core.get_singleton()
	action_manager = core:GetActionManager()
	graphics_manager = core:GetGraphicsManager()
	renderable_objects_manager = core:GetRenderableObjectsManager()
	cinematic = CCinematic("Data/cinematic.xml")
	cinematic:Stop()
	renderable_objects_manager:AddResource(cinematic:GetName(), cinematic)
	timer = core:GetTimer()
	pos = Vect3f(0, 0, 0)
end

function update ()
	local dt = timer:GetElapsedTime()
	local speed = 1;
	if action_manager:DoAction("Left") then
		pos.x = pos.x - speed * dt
	end
	if action_manager:DoAction("Right") then
		pos.x = pos.x + speed * dt
	end
	if action_manager:DoAction("Backward") then
		pos.z = pos.z - speed * dt
	end
	if action_manager:DoAction("Forward") then
		pos.z = pos.z + speed * dt
	end
	
	if pos.x > 2 then
		cinematic:Play(false)
	end
end

function render ()
	local t = Mat44f()
	t:SetPos(pos)
	graphics_manager:SetTransform(t)
	graphics_manager:DrawCube(1)
	t:SetIdentity()
	graphics_manager:SetTransform(t)
end