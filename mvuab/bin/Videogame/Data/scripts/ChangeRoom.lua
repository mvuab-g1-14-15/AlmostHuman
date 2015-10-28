g_Rooms = { Vect3f( 136.84, -7.99, -62 ), Vect3f( 76.51, -10.60, -31.10 ), Vect3f( 53.72, -16.66, -17.16 ), Vect3f( 53.80, -16.23, 41.77 ), Vect3f( -53.01, 35.79, 62.32 ),Vect3f( -347.22, 21.80, 56.73 )}
g_ActualRoom = 1

function ChangeRoom()
    g_ActualRoom = g_ActualRoom + 1
	if g_ActualRoom > #g_Rooms then
		g_ActualRoom = 1
	end
	
	if g_ActualRoom == 1 then
		g_Player:SetRoom("sala1")
		g_Player:SetCheckpoint("sala1", Vect3f( 136.84, -7.99, -62 ), 100.0, 100.0)
	end
	if g_ActualRoom == 2 then
		g_Player:SetRoom("sala2")
		g_Player:SetCheckpoint("sala2", Vect3f( 76.51, -10.60, -31.10 ), 100.0, 100.0)
	end
	if g_ActualRoom == 3 then
		g_Player:SetRoom("pasillo")
		g_Player:SetCheckpoint("pasillo", Vect3f( 53.72, -16.66, -17.16 ), 100.0, 100.0)
	end
	if g_ActualRoom == 4 then
		g_Player:SetRoom("sala3")
		g_Player:SetCheckpoint("sala3", Vect3f( 53.80, -16.23, 41.77), 100.0, 100.0)
	end
	if g_ActualRoom == 5 then
		g_Player:SetRoom("sala4")
		g_Player:SetCheckpoint("sala4", Vect3f( -53.01, 35.79, 62.32 ), 100.0, 100.0)
	end
	
	sound_manager:PlayEvent("Open_Close_Door_Event","TestGameObject3d")
	return g_Rooms[g_ActualRoom]
end

function Respawn()
	sound_manager:PlayEvent("Open_Close_Door_Event","TestGameObject3d")
	return g_Rooms[g_ActualRoom]
end