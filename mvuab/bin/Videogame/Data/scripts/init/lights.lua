local cinematic=nil
local g_Pi = 3.141618
local g_HalfPi = 3.141618 * 0.5
local g_ForwardSpeed = 5
local g_StrafeSpeed = 6
local g_Speed = 5

function init()
	core = Singleton_Core.get_singleton()
	action_manager = core:GetActionManager()
	graphics_manager = core:GetGraphicsManager()
	light_manager = core:GetLightManager()
	renderable_objects_manager = core:GetRenderableObjectsManager()
	camera_manager = core:GetCameraManager()
	timer = core:GetTimer()
	pos = Vect3f(0, 0, 0)
	
	initialized = true;
end

function update()
	if not initialized then
		init()
	end
	
	local dt = timer:GetElapsedTime()
	local current_light = light_manager:GetLight(1);
	
	local addPos = Vect3f(0, 0, 0)
	
	addPos:Normalize()
	
	if action_manager:DoAction("Left") then
		addPos.x =  1
	elseif action_manager:DoAction("Right") then
		addPos.x =  -1
	end
	if action_manager:DoAction("Backward") then
		addPos.z =  1
	elseif action_manager:DoAction("Forward") then
		addPos.z =  -1
	end
	
	if action_manager:DoAction("MoveUp") then
		addPos.y =  1
	elseif action_manager:DoAction("MoveDown") then
		addPos.y =  -1
	end
	
	pos = current_light:GetPosition()
	current_light:SetPosition( pos + addPos )
	
	core:trace("hola")
	
end

function render()
end