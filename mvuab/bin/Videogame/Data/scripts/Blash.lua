class 'CBlash'
--Clase que implementa la primera rafaga del disparo de logan

function CBlash:__init()
	self.Active = false
	countdowntimer_manager:AddTimer("BlashTTL", 0.05, false) 
	self.BillboardBlash = billboard_manager:CreateInstance("blash", Vect3f(0, 0, 0), false)
end

function CBlash:Begin( aPosition )
	self.Active = true
	countdowntimer_manager:SetActive("BlashTTL", true)
	self.BillboardBlash:ChangePosition( aPosition )
	self.BillboardBlash:ChangeVisibility( true )
end

function CBlash:End()
	self.Active = false;
	self.BillboardBlash:ChangeVisibility( false )
	countdowntimer_manager:Reset("BlashTTL", false)
end

function CBlash:Update()
	if not g_ConsoleActivate and not g_CinematicActive and self.Active then
		if countdowntimer_manager:isTimerFinish("BlashTTL") then
			self:End()
		end
	end
end
