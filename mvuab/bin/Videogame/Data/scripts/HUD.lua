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
	self.Map:SetYawPlayer(g_Player:GetYaw())
end

function CHUD:UpdatePositionEnemy(name)
	enemy = enemy_manager:GetResource(name)
	self.Map:SetPositionEnemy(name, enemy:GetPosition())
end

function CHUD:UpdateYawEnemy(name)
	enemy = enemy_manager:GetResource(name)
	self.Map:SetYawEnemy(name, enemy:GetYaw())
end
