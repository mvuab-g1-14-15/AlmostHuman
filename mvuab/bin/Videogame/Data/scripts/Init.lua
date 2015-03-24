loadfile "include.lua"

local l_Blaster = loadfile "Blaster.lua"
local l_Player = loadfile "Player.lua"

function load_basics()
 -- basic loads
end

function load_gameplay()
	l_Blaster = CBlaster()
	l_Player = CPlayer()
end

function update_gameplay()
	l_Blaster:Update()
end