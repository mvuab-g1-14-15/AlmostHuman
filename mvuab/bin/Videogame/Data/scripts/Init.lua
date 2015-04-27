dofile("./data/scripts/Grenade.lua")
dofile("./data/scripts/Blaster.lua")
dofile("./data/scripts/Player.lua")

local g_Grenade = nil
local g_Blaster = nil
local g_Player = nil


local initialized = false

function load_basics()
	-- basic loads
end

function load_gameplay()
    g_Blaster = CBlaster()
	g_Grenade = CGrenade()
    g_Player = CPlayer()
	
	initialized = true
end

function update_gameplay()
	if not initialized then
		load_gameplay()
	end
	
	g_Blaster:Update()
	g_Grenade:Update()
	
	if( CameraType.Free.value == camera_manager:GetCurrentCamera():GetCameraType() ) then 
		UpdateFree()
	else
		g_Player:Update()
	end
	CheckLevel()
	
	if action_manager:DoAction("ChangeRoom") then
	local newPosition = ChangeRoom()
		g_Player:SetPosition(newPosition)
	end
end