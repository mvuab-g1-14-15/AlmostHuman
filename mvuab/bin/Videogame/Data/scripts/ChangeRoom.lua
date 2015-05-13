g_Rooms = {Vect3f( -0.8, 1.05, 4.87 ), Vect3f( 8, -10, 18 ), Vect3f( 40, -15, -8), Vect3f( 141, 35, -17 ), Vect3f( 104, 22, 198 )}
g_ActualRoom = 1

function ChangeRoom()
    g_ActualRoom = g_ActualRoom + 1
	if g_ActualRoom > #g_Rooms then
		g_ActualRoom = 1
	end
	sound_manager:PlayEvent("Open_Close_Door_Event","TestGameObject3d")
	return g_Rooms[g_ActualRoom]
end