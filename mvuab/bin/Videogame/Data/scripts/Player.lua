dofile("./data/scripts/PlayerController.lua")

class "CPlayer"

function CPlayer:__init() 
	self.m_PlayerController = CPlayerController()
end

function CPlayer:Update()
	self.m_PlayerController:Update()
end

function CPlayer:SetPosition(position)
	self.m_PlayerController:SetPosition(position)
end