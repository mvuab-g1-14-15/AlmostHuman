class "CHUD"

function CHUD:__init()
	self.Window = gui_manager:GetWindow("HUD.xml")
	self.Life = self.Window:GetElement("Life")
	self.Life:SetProgress(g_Player:GetLife())
	self.Energy = self.Window:GetElement("Energy")
	l_Energy = g_Player:GetBlaster():GetEnergy()
	self.Energy:SetProgress(l_Energy)
	self.Grenades = self.Window:GetElement("Grenades")
	self.Map = self.Window:GetElement("Radar")
end

function CHUD:Update()
	l_Life = g_Player:GetLife()
	if l_Life >= 0 then
		self.Life:SetProgress(l_Life)
	end
	l_Energy = g_Player:GetBlaster():GetEnergy()
	self.Energy:SetProgress(l_Energy)
	--Actualizo la posición del controller
end

function CHUD:UpdatePositionPlayer()
	self.Map:SetPositionPlayer(g_Player:GetPosition())
end

function CHUD:UpdateYawPlayer()
	self.Map:SetYawPlayer((-g_Player:GetYaw()+3.1416/2.0))
end

function CHUD:UpdatePositionEnemy(name)
	enemy = g_EnemyManager:GetResource(name)
	self.Map:SetPositionEnemy(name, enemy:GetPosition())
end

function CHUD:UpdateYawEnemy(name)
	enemy = g_EnemyManager:GetResource(name)
	self.Map:SetYawEnemy(name, enemy:GetCharacterController():GetYaw())
end


local doit=true

function GetMapPosition(ElementPosition3D, GUIMapPosition, GUIMapSize, UVMapSize, MapPsition003D, MapPsition113D)
	local l_PlayerPosition=g_Player:GetPosition()
	
	local l_UVElementMapPosition=GetUVMap(ElementPosition3D, MapPsition003D, MapPsition113D)
	local l_UVPlayerMapPosition=GetUVMap(l_PlayerPosition, MapPsition003D, MapPsition113D)
	
	local l_UVMapPosition=l_UVPlayerMapPosition-UVMapSize*0.5
	local l_Offset=Vect3f((l_UVElementMapPosition.x-l_UVMapPosition.x)/UVMapSize.x,(l_UVElementMapPosition.z-l_UVMapPosition.z)/UVMapSize.z,0)
		
	return GUIMapPosition+Vect3f(l_Offset.x*GUIMapSize.x, l_Offset.y*GUIMapSize.y, 0)
end

function GetMapItemPosition()
	local l_MapPosition=Vect3f(1011, 424, 0)
	local l_MapSize=Vect3f(255, 259, 0)
	local l_UVMapSize=Vect3f(0.04, 0.0, 0.0225)
	local l_ItemPosition=Vect3f(3,0,-4)
	local l_MapPosition00=Vect3f(-20.335, 0.0, 531.173)
	local l_MapPosition11=Vect3f(329.941, 0.0, -44.622)
	
	--[[local l_PlayerPosition=g_Player:GetPosition()
	
	local l_UVItemMapPosition=GetUVMap(l_ItemPosition, l_MapPosition00, l_MapPosition11)
	local l_UVPlayerMapPosition=GetUVMap(l_PlayerPosition, l_MapPosition00, l_MapPosition11)
	
	local l_UVMapPosition=l_UVPlayerMapPosition-l_UVMapSize*0.5
	local l_Offset=Vect3f((l_UVItemMapPosition.x-l_UVMapPosition.x)/l_UVMapSize.x,(l_UVItemMapPosition.z-l_UVMapPosition.z)/l_UVMapSize.z,0)
		
	if doit then
		doit=false
		engine:Trace("eo "..l_UVItemMapPosition.x.." "..l_UVItemMapPosition.y.." "..l_UVItemMapPosition.z)
		engine:Trace("eo2 "..l_UVPlayerMapPosition.x.." "..l_UVPlayerMapPosition.y.." "..l_UVPlayerMapPosition.z)
		engine:Trace("eo3 "..l_UVMapPosition.x.." "..l_UVMapPosition.y.." "..l_UVMapPosition.z)
		engine:Trace("l_Offset "..l_Offset.x.." "..l_Offset.y.." "..l_Offset.z)
	end
	
	
	--<Map name="Radar" pos_in_screen="79 59" width="20" height="36" texture_marco="Data/textures/GUI/Textures_Test/marco2.png" texture_map="Data/textures/GUI/Textures_Test/mapa.tga"
	--width_map="0.04" height_map="0.0225" pos_0_0_3d_map="-20.335 531.173" pos_1_1_3d_map="329.941 -44.622" visible="true" active="true">
	
	--Vect3f 
	return l_MapPosition+Vect3f(l_Offset.x*l_MapSize.x, l_Offset.y*l_MapSize.y, 0)]]
	return GetMapPosition(l_ItemPosition, l_MapPosition, l_MapSize, l_UVMapSize, l_MapPosition00, l_MapPosition11)
end

function CHUD:AddEnemy( aName, aTexture, aPosInMap3d, aposInMap, aWidth, aHeight, aYaw, aPositionScriptComplete, aOrientationScriptComplete )
	engine:Trace("He llegado antes de petar")
	self.Map:AddEnemyLUA( aName, aTexture, aPosInMap3d, aposInMap, aWidth, aHeight, aYaw, aPositionScriptComplete, aOrientationScriptComplete )
end

function CHUD:NormalizePlayerPos(x, z)
	return self.Map:NormalizePlayerPos(x, z)
end