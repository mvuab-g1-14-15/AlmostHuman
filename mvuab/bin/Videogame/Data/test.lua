function init ()
	core = Singleton_Core.get_singleton()
	action_manager = core:GetActionManager()
	graphics_manager = core:GetGraphicsManager()
	pos = Vect3f(0, 0, 0)
end

function update ()
	if action_manager:DoAction("Left") then
		pos.x = pos.x - 0.1
	end
	if action_manager:DoAction("Right") then
		pos.x = pos.x + 0.1
	end
	if action_manager:DoAction("Backward") then
		pos.z = pos.z - 0.1
	end
	if action_manager:DoAction("Forward") then
		pos.z = pos.z + 0.1
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