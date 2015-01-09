function init ()
	core = Singleton_Core.get_singleton()
	input_manager = core:GetInputManager()
	graphics_manager = core:GetGraphicsManager()
	pos = Vect3f(0, 0, 0)
end

function update ()
	if input_manager:IsDownUp(0,0xCB) then
		pos.x = pos.x - 1
	end
	if input_manager:IsDownUp(0,0xCD) then
		pos.x = pos.x + 1
	end
	if input_manager:IsDownUp(0,0xD0) then
		pos.z = pos.z - 1
	end
	if input_manager:IsDownUp(0,0xC8) then
		pos.z = pos.z + 1
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