class 'CBlash'
--Clase que implementa la primera rafaga del disparo de logan

function CBlash:__init( aTimeName )
	self.Active = false
	self.TimerName = aTimeName
	countdowntimer_manager:AddTimer(self.TimerName, 0.05, false) 
	self.BillboardBlash = billboard_manager:CreateInstance("blash", Vect3f(0, 0, 0), false)
	engine:Trace("Blash ctor"..self.TimerName)
end

function CBlash:Begin( aPosition )
	engine:Trace( "Blash is "..aPosition:ToString() )
	self.Active = true
	countdowntimer_manager:SetActive(self.TimerName, true)
	self.Angle = math.random(0.1, 2* math.pi)
	self.BillboardBlash:SetAngle( self.Angle )
	self.BillboardBlash:ChangePosition( aPosition )
	self.BillboardBlash:ChangeVisibility( true )
end

function CBlash:End()
	engine:Trace("Blash end")
	self.Active = false;
	self.BillboardBlash:ChangeVisibility( false )
	countdowntimer_manager:Reset(self.TimerName, false)
end

function CBlash:Update( aPosition )
	if not g_ConsoleActivate and not g_CinematicActive and self.Active then
		--engine:Trace("Blash update ")
		self.BillboardBlash:ChangePosition( aPosition )
		if countdowntimer_manager:isTimerFinish(self.TimerName) then
			self:End()
		end
	end
end
