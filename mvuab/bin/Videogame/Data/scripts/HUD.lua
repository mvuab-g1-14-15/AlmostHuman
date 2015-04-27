class "CHUD"

function CHUD:__init()
	self.Window = gui_manager:GetWindow("HUD.xml")
	
	self.Life = self.Window:GetElement("Life")
	self.Energy = self.Window:GetElement("Energy")
	self.Grenades = self.Window:GetElement("Grenades")
	--self.Map = self.Window:GetElement("Map")
end

function CHUD:Update()
	self.Life:SetProgress(g_Player:GetLife())
end