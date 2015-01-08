function init ()
	input_manager = CInputManager()
	graphics_manager = CGraphicsManager()
	pos = Vect3f(0, 0, 0)
end

function update ()
	if input_manager:IsDownUp(0x1E) then
		pos.x = pos.x - 1
	end
	if input_manager:IsDownUp(0x20) then
		pos.x = pos.x + 1
	end
	if input_manager:IsDownUp(0x1F) then
		pos.z = pos.z - 1
	end
	if input_manager:IsDownUp(0x11) then
		pos.z = pos.z + 1
	end
end

function render ()
	local t = Mat44f()
	t:SetPos(pos)
	graphics_manager:SetTransform(t)
	graphics_manager:DrawBox(1,1,1)
	t:SetIdentity()
	graphics_manager:SetTransform(t)
end