
dofile("./data/scripts/Player.lua")

g_Blaster = nil
g_Player = nil

local initialized = false

function load_basics()
	-- basic loads
end

function load_gameplay()
    g_Player = CPlayer()
	g_HUD = CHUD()
	
	initialized = true
end

function update_gameplay()
	if not initialized then
		load_gameplay()
	end
	
	g_HUD:Update()
	
	if( CameraType.Free.value == camera_manager:GetCurrentCamera():GetCameraType() ) then 
		UpdateFree()
	else
		g_Player:Update()
	end
	
	CheckLevel()
	
	if action_manager:DoAction("ChangeRoom") then
		g_Player:SetPosition(ChangeRoom())
	end
end