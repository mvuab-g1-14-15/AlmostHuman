g_Rooms = {Vect3f( -0.8, 0.55, 4.87 ), Vect3f( 8, -12.5, 18 ), Vect3f( 40, -18.5, -8), Vect3f( 141, 31.5, -17 ), Vect3f( 104, 20.5, 198 )}
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