core = Singleton_Core.get_singleton()
physicManager = core:GetPhysicsManager()
engine = Singleton_Engine.get_singleton()
function OnEnter()
	process = engine:GetProcess()
	physicUserData = process:GetNewPUD("Box6")
	physicUserData:SetColor(0,1,0,1)
	physicUserData:SetPaint(true)
	process:AddPudVector(physicUserData)
	physicActor = process:GetNewPhysicActor(physicUserData)
	physicActor:AddBoxShape(Vect3f( 1, 1, 1), Vect3f( 5, 8, 5), Vect3f( 0, 0, 0), Vect3f( 0, 0, 0))
	physicActor:CreateBody(1, 0.5, 0.5)
	process:AddPhysicActorVector(physicActor)
	physicManager:AddPhysicActor(physicActor)

end

function OnLeave(other_shape)
	process = engine:GetProcess()
	process:SetSalir(true)
	physicActor = physicManager:GetActor(other_shape)
	physicUserData = physicActor:GetUserData()
	physicUserData:SetColor(1,1,0,1)
end

function OnStay(other_shape)
	process = engine:GetProcess()
	physicUserData = process:GetLastPUDInserted()
	physicUserData:SetColor(1,0,0,1)
	
	physicActor2 = physicManager:GetActor(other_shape)
	physicUserData2 = physicActor2:GetUserData()
	physicUserData2:SetColor(0,1,1,1)
end