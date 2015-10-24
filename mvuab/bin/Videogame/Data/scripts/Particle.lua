class 'CParticle'

function CParticle:__init( aCoreName, aPosition )
	self.Active   = false
	self.Core 	  = particle_system_manager:GetResource( aCoreName )
	self.Instance = CParticleInstance( aPosition );
	self.Core:AddInstance( self.Instance );
end

function CParticle:Begin( aPosition )
	self.Active = true;
	self.Core:ResetEmitters();
end

function CParticle:End()
	self.Active = false;
	self.Instance:ChangeVisibility( false )
end

function CParticle:ChangePosition( aPosition )
	self.Instance:ChangePosition( aPosition )
end