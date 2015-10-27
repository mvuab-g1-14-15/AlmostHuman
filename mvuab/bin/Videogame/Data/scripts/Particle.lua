class 'CParticle'

function CParticle:__init( aCoreName, aPosition )
	self.Core 	  = particle_system_manager:GetResource( aCoreName )
	self.Instance = CParticleInstance( aPosition );
	self.Core:AddInstance( self.Instance );
	self.Instance:ChangeVisibility( false );
end

function CParticle:Init( aPosition )
	self.Instance:ChangeVisibility( true )
	self:ChangePosition( aPosition )
	self.Core:ResetEmitters();
end

function CParticle:Begin( aPosition )
	self.Instance:ChangeVisibility( true )
	self:ChangePosition( aPosition )
end

function CParticle:ChangePosition( aPosition )
	self.Instance:ChangePosition( aPosition )
end

function CParticle:SetDirection( aDir )
	self.Core:SetFixedDirection( aDir )
end

