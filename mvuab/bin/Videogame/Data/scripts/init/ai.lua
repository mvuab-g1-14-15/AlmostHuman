tiempoDeEspera = 0

function set_initial_waypoint()
	enemy = enemy_manager:GetActualEnemy()
	enemy:SetCurrentPoint(0)
end

function check_next_state()
	enemy = enemy_manager:GetActualEnemy()
	local l_CurrentState = enemy:GetCurrentState()
	local l_NextState = l_CurrentState
	--core:Trace("Current state: " .. l_CurrentState)
	
	local l_DistanceToPlayer = PlayerDistance(enemy)
	local l_PlayerInSight = PlayerVisibility(enemy)
	
	--core:Trace("Distance to player: " .. l_DistanceToPlayer)
	if l_PlayerInSight then
		core:Trace("Is viewing player: true")
	else
		core:Trace("Is viewing player: false")
	end
	if l_CurrentState == "inicial" then
		l_NextState = "andando"
	end
	if l_CurrentState == "inicial" or l_CurrentState == "parado" or l_CurrentState == "andando" then
		if l_DistanceToPlayer <= 8 and l_PlayerInSight then
			l_NextState = "perseguir"
		end
	end
	if l_CurrentState == "inicial" or l_CurrentState == "parado" or l_CurrentState == "andando" or l_CurrentState == "perseguir" then
		if l_DistanceToPlayer < 4 and l_PlayerInSight then
			l_NextState = "atacar"
		end
	end
	if l_DistanceToPlayer > 5 or not l_PlayerInSight then
		l_NextState = "andando"
	end
	if l_NextState ~= l_CurrentState then
		enemy:ChangeState(l_NextState)
	end
end

function andar()
	local dt = timer:GetElapsedTime()
	enemy = enemy_manager:GetActualEnemy()
	
	local l_TargetPos = enemy:GetTargetPosition()
	l_TargetPos.y = 0.0
	local l_EnemyPos = enemy:GetPosition()
	l_EnemyPos.y = 0.0
	
	local l_DistanceVector = l_TargetPos - l_EnemyPos
	
	if l_DistanceVector:Length() < 0.1  then
		local currentPoint = enemy:GetCurrentPoint()
		local lTargetPosition = VectorWaypoints(enemy:GetWaypoints()):GetResource(currentPoint)
		enemy:SetTargetPosition(lTargetPosition)
		enemy:SetTargetPositionOriginal(lTargetPosition)
		if currentPoint+1 == (enemy:GetCount()) then
			enemy:SetCurrentPoint(0)
		else
			enemy:SetCurrentPoint(currentPoint+1)
		end
	end
	
	if CheckVector(l_DistanceVector) then
		l_DistanceVector:Normalize()
	end
	
	local l_Yaw = math.atan2( l_DistanceVector.z, l_DistanceVector.x)
	enemy:SetYaw(l_Yaw)
	
	l_DistanceVector = l_DistanceVector
	enemy:Move(l_DistanceVector, dt)
end

function esperar()
	enemy = enemy_manager:GetActualEnemy()
	local timer = core:GetTimer()
	tiempoDeEspera = tiempoDeEspera + timer:GetElapsedTime()
	if tiempoDeEspera > 2 then
		enemy:SetOnExit(true)
		tiempoDeEspera = 0
		enemy:SetCurrentPoint(0)
	end
end

function atacar()
	perseguir()
end

function perseguir()
	local dt = timer:GetElapsedTime()
	enemy = enemy_manager:GetActualEnemy()
	
	local l_TargetPos = GetPlayerPosition()
	l_TargetPos.y = 0.0
	local l_EnemyPos = enemy:GetPosition()
	l_EnemyPos.y = 0.0
	
	local l_DistanceVector = l_TargetPos - l_EnemyPos
	
	if CheckVector(l_DistanceVector) then
		l_DistanceVector:Normalize()
	end
	
	local l_Yaw = math.atan2( l_DistanceVector.z, l_DistanceVector.x)
	enemy:SetYaw(l_Yaw)
	
	l_DistanceVector = l_DistanceVector
	enemy:Move(l_DistanceVector, dt)
end