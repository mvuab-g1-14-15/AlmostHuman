g_Rooms = { Vect3f( 136.84, -7.99, -62 ), Vect3f( 76.51, -10.60, -31.10 ), Vect3f( 53.72, -16.66, -17.16 ), Vect3f( 53.80, -16.23, 41.77 ), Vect3f( -53.01, 35.79, 62.32 )}
g_ActualRoom = 1

function ChangeRoom()
    g_ActualRoom = g_ActualRoom + 1
	if g_ActualRoom > #g_Rooms then
		g_ActualRoom = 1
	end
	
	if g_ActualRoom == 1 then
		g_Player:SetRoom("sala1")
	end
	if g_ActualRoom == 2 then
		g_Player:SetRoom("sala2")
	end
	if g_ActualRoom == 3 then
		g_Player:SetRoom("pasillo")
	end
	if g_ActualRoom == 4 then
		g_Player:SetRoom("sala3")
	end
	if g_ActualRoom == 5 then
		g_Player:SetRoom("sala4")
	end
	
	sound_manager:PlayEvent("Open_Close_Door_Event","TestGameObject3d")
	return g_Rooms[g_ActualRoom]
end

function Respawn()
	sound_manager:PlayEvent("Open_Close_Door_Event","TestGameObject3d")
	return g_Rooms[g_ActualRoom]
end