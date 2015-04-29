class "CHUD"

function CHUD:__init()
	self.Window = gui_manager:GetWindow("HUD.xml")
	self.Life = self.Window:GetElement("Life")
	self.Life:SetProgress(g_Player:GetLife())
	self.Energy = self.Window:GetElement("Energy")
	l_Energy = g_Blaster:GetEnergy()
	self.Energy:SetProgress(l_Energy)
	self.Grenades = self.Window:GetElement("Grenades")
	--self.Map = self.Window:GetElement("Map")
end

function CHUD:Update()
	l_Life = g_Player:GetLife()
	if l_Life >= 0 then
		self.Life:SetProgress(l_Life)
	end
	l_Energy = g_Blaster:GetEnergy()
	self.Energy:SetProgress(l_Energy)
end
