g_Rooms = { Vect3f( 136.84, -7.99, -62 ), Vect3f( 76.51, -10.60, -31.10 ), Vect3f( 53.72, -16.66, -17.16 ), Vect3f( 53.80, -16.23, 41.77 ), Vect3f( -11.47, 45.95, 64.47 )}
g_ActualRoom = 1

function ChangeRoom()
    g_ActualRoom = g_ActualRoom + 1
	if g_ActualRoom > #g_Rooms then
		g_ActualRoom = 1
	end
	sound_manager:PlayEvent("Open_Close_Door_Event","TestGameObject3d")
	return g_Rooms[g_ActualRoom]
end

function Respawn()
	sound_manager:PlayEvent("Open_Close_Door_Event","TestGameObject3d")
	return g_Rooms[g_ActualRoom]
end