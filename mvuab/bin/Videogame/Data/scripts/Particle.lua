class 'CParticle'

function CParticle:__init( aName, aCoreName, aRoomName )
	self.Instance = particle_system_manager:GetInstance( aName, aCoreName, aRoomName );
	self.Instance:ChangeVisibility( false );
end

function CParticle:Init( aPosition )
	self.Instance:ChangePosition( aPosition );
	self.Instance:Reset();
	self.Instance:ChangeVisibility( true )
end

function CParticle:Begin( aPosition )
	self.Instance:ChangeVisibility( true )
	self:ChangePosition( aPosition )
end

function CParticle:ChangePosition( aPosition )
	self.Instance:ChangePosition( aPosition )
end

function CParticle:SetDirection( aDir )
	self.Instance:SetDirection( aDir )
end

function CParticle:Hide()
	self.Instance:ChangeVisibility( false )
end

function CParticle:Show()
	self.Instance:ChangeVisibility( true )
end

