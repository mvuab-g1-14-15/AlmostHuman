g_Rooms = {Vect3f( 77, 1.5, 7 ), Vect3f( 64, -15.5, -38 ), Vect3f( 141, 31.5, -17 ), Vect3f( 104, 20.5, 198 )}g_ActualRoom = 1

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