g_Rooms = { Vect3f( 130, -8, -52.12 ), Vect3f( 76.51, -10.60, -31.10 ), Vect3f( 76.30, -9.66, -34.77 ), Vect3f( 54.53, -16.61, 39.80 ), Vect3f( 40.07, -17.19, 52.25 ), Vect3f( -51.74, 62.45, 70.35 )}
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