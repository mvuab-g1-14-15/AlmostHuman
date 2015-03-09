core = Singleton_Core.get_singleton()
Enemy_Manager = core:GetEnemyManager()
physicManager = core:GetPhysicsManager()
tiempoDeEspera = 0
function set_initial_waypoint()
	enemy = Enemy_Manager:GetActualEnemy()
	local m_WaypointPoint = VectorWaypoints(enemy:GetWaypoints())
	m_WaypointPoint:push_back(Vect3f(10,0,10))
    m_WaypointPoint:push_back(Vect3f(10,0,-10))
    m_WaypointPoint:push_back(Vect3f(-10,0,-10))
    m_WaypointPoint:push_back(Vect3f(-10,0,10))
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
		enemy:ChangeState(m_VectorNameState:getResource(0))
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
	if (vector_distz < 0.2 and vector_distz > -0.2) and (vector_distx < 0.2 and vector_distx > -0.2)  then
		local currentPoint = enemy:GetCurrentPoint()
		if currentPoint == (enemy:getCount()) then
			enemy:SetExit(true)
		else
			
			enemy:setTargetPosition(VectorWaypoints(enemy:GetWaypoints()):getResource(currentPoint))
			enemy:SetTargetPositionOriginal(VectorWaypoints(enemy:GetWaypoints()):getResource(currentPoint))
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
		m_WaypointPoint = VectorWaypoints(enemy:GetWaypoints())
		m_WaypointPoint:clear()
		enemy:SetWaypoints(m_WaypointPoint)
	end
end
