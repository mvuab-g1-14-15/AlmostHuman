class "CIdManager"

function CIdManager:__init()
	self.id = 0
end

function CIdManager:GetId()
	local return_id = self.id
	self.id = self.id + 1
	return return_id
end