function OnEnter()
	core = Singleton_Core.get_singleton()
	physicManager = core:GetPhysicsManager()
	engine = Singleton_Engine.get_singleton()
	process = engine:GetProcess()
	--physicUserData = process:GetPUD()
	--physicUserData:SetName("Box6")
	--physicUserData:SetPaint(true)
	color = CColor(0,1,0,1)
	--physicUserData:SetColor(color)
	--physicActor = process:GetPhysicActor()
	--physicActor = CPhysicActor(physicUserData)
	--physicActor:AddPlaneShape(Vect3f( -1, 0, 0), 15, 0)
	--physicManager:AddPhysicActor(physicActor)
	
	--process:SetPhysicActor(physicActor)
end

function OnLeave()
	engine = Singleton_Engine.get_singleton()
	process = engine:GetProcess()
	--process:SetSalir(true)
end

function OnStay()
	engine = Singleton_Engine.get_singleton()
	process = engine:GetProcess()
	--physicUserData = process:GetPUD()
	color = CColor(1,0,0,1)
	--physicUserData:SetColor(color)
end