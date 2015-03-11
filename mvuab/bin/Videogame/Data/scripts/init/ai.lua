core = Singleton_Core.get_singleton()
Enemy_Manager = core:GetEnemyManager()
tiempoDeEspera = 0

function set_initial_waypoint()
	enemy = Enemy_Manager:GetActualEnemy()
	local process = Singleton_Engine.get_singleton():GetProcess()
	local enemyPosition = Vect3f(enemy:GetPosition())
	enemyPosition.y = 0
	local m_WaypointPoint = VectorWaypoints(process:GetAStarScene():GetPath(enemyPosition, Vect3f(6, 0, 6)))
	enemy:SetWaypoints(m_WaypointPoint)
	enemy:SetCurrentPoint(0)
	enemy:SetExit(true)
end

function change_state()
	enemy = Enemy_Manager:GetActualEnemy()
	local m_CurrentState = enemy:getCurrentState()
	local m_VectorNameState = VectorStates(enemy:getStateMachine():GetStateName())
	local posicion = m_VectorNameState:getIdByResource(m_CurrentState)
	if posicion == (m_VectorNameState:size()-1) then
		enemy:ChangeState(m_VectorNameState:getResource(1))
	else
		enemy:ChangeState(m_VectorNameState:getResource(posicion+1))
	end
end

function andar()
	enemy = Enemy_Manager:GetActualEnemy()
	local targetPosition = enemy:getTargetPosition()
	
	local enemy_pos = enemy:GetPosition()
	local vector_distx = targetPosition.x - enemy_pos.x
	local vector_distz = targetPosition.z - enemy_pos.z
	if (vector_distz < 0.1 and vector_distz > -0.1) and (vector_distx < 0.1 and vector_distx > -0.1)  then
		local currentPoint = enemy:GetCurrentPoint()
		enemy:setTargetPosition(VectorWaypoints(enemy:GetWaypoints()):getResource(currentPoint))
		enemy:SetTargetPositionOriginal(VectorWaypoints(enemy:GetWaypoints()):getResource(currentPoint))
		if currentPoint+1 == (enemy:getCount()) then
			enemy:SetExit(true)
		else	
			enemy:SetCurrentPoint(currentPoint+1)
		end
	end
end

function esperar()
	enemy = Enemy_Manager:GetActualEnemy()
	local timer = core:GetTimer()
	tiempoDeEspera = tiempoDeEspera + timer:GetElapsedTime()
	if tiempoDeEspera > 2 then
		enemy:SetExit(true)
		tiempoDeEspera = 0
		enemy:SetCurrentPoint(0)
	end
end