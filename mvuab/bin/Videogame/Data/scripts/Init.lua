dofile("./data/scripts/Blaster.lua")
dofile("./data/scripts/Player.lua")

local g_Blaster = nil
local g_Player = nil

function load_basics()
	-- basic loads
end

function load_gameplay()
    g_Blaster = CBlaster()
    g_Player = CPlayer()
end

function update_gameplay()
	g_Blaster:Update()
	g_Player:Update()
end