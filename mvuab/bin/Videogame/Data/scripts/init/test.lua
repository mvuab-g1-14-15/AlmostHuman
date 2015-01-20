local cinematic=nil

function init ()
	core = Singleton_Core.get_singleton()
	action_manager = core:GetActionManager()
	graphics_manager = core:GetGraphicsManager()
	renderable_objects_manager = core:GetRenderableObjectsManager()
	camera_manager = core:GetCameraManager()
	cinematic=renderable_objects_manager:CreateCinematic("Data/cinematic.xml")
	cinematic:Stop()
	timer = core:GetTimer()
	pos = Vect3f(0, 0, 0)
end

function update ()
	local dt = timer:GetElapsedTime()
	local speed = 5;
	
	local current_camera = camera_manager:GetCurrentCamera();
	local vec_up = current_camera:GetVecUp()
	local cam_pos = current_camera:GetPos()
	local dir = pos - cam_pos
	dir:Normalize()
	local dir_per = dir:CrossProduct(vec_up)
	dir_per:Normalize()
	
	camera_offset = Vect3f(-5,5,-5)
	
	current_camera:SetPos(pos+camera_offset)
	
	if action_manager:DoAction("Left") then
		pos = pos + dir_per * speed * dt
	end
	if action_manager:DoAction("Right") then
		pos = pos - dir_per * speed * dt
	end
	if action_manager:DoAction("Backward") then
		pos = pos - dir * speed * dt
	end
	if action_manager:DoAction("Forward") then
		pos = pos + dir * speed * dt
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