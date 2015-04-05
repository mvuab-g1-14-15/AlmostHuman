tiempoDeEspera = 0

function set_initial_waypoint()
	enemy = enemy_manager:GetActualEnemy()
	enemy:SetCurrentPoint(0)
end

function check_next_state()
	enemy = enemy_manager:GetActualEnemy()
	local l_CurrentState = enemy:getCurrentState()
	local l_NextState = l_CurrentState
	core:trace("Current state: " .. l_CurrentState)
	
	local l_DistanceToPlayer = PlayerDistance(enemy)
	local l_PlayerInSight = PlayerVisibility(enemy)
	
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
	enemy = enemy_manager:GetActualEnemy()
	local targetPosition = enemy:getTargetPosition()
	
	local enemy_pos = enemy:GetPosition()
	local vector_distx = targetPosition.x - enemy_pos.x
	local vector_distz = targetPosition.z - enemy_pos.z
	if (vector_distz < 0.1 and vector_distz > -0.1) and (vector_distx < 0.1 and vector_distx > -0.1)  then
		local currentPoint = enemy:GetCurrentPoint()
		local lTargetPosition = VectorWaypoints(enemy:GetWaypoints()):getResource(currentPoint)
		enemy:setTargetPosition(lTargetPosition)
		enemy:SetTargetPositionOriginal(lTargetPosition)
		if currentPoint+1 == (enemy:getCount()) then
			enemy:SetExit(true)
		else	
			enemy:SetCurrentPoint(currentPoint+1)
		end
	end
end

function esperar()
	enemy = enemy_manager:GetActualEnemy()
	local timer = core:GetTimer()
	tiempoDeEspera = tiempoDeEspera + timer:GetElapsedTime()
	if tiempoDeEspera > 2 then
		enemy:SetExit(true)
		tiempoDeEspera = 0
		enemy:SetCurrentPoint(0)
	end
end

function atacar()
end

function perseguir()
end