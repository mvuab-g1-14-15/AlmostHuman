tiempoDeEspera = 0

function init_enemy()
	enemy = enemy_manager:GetActualEnemy()
	enemy:SetCurrentPoint(0)
end

function check_next_state()
	enemy = enemy_manager:GetActualEnemy()
	local l_CurrentState = enemy:GetCurrentState()
	local l_NextState = l_CurrentState
	--engine:Trace("Current state: " .. l_CurrentState)
	
	local l_DistanceToPlayer = PlayerDistance(enemy)
	local l_PlayerInSight = PlayerVisibility(enemy)
	
	--engine:Trace("Distance to player: " .. l_DistanceToPlayer)
	--[[if l_PlayerInSight then
		engine:Trace("Is viewing player: true")
	else
		engine:Trace("Is viewing player: false")
	end]]
	if l_CurrentState == "inicial" then
		l_NextState = "andando"
	end
	if l_CurrentState == "inicial" or l_CurrentState == "esperar" or l_CurrentState == "andando" then
		if l_DistanceToPlayer <= 8 and l_PlayerInSight then
			l_NextState = "perseguir"
		end
	end
	if l_CurrentState == "inicial" or l_CurrentState == "esperar" or l_CurrentState == "andando" or l_CurrentState == "perseguir" then
		if l_DistanceToPlayer < 4 and l_PlayerInSight then
			l_NextState = "atacar"
		end
	end
	--if l_DistanceToPlayer > 5 or not l_PlayerInSight then
	--	l_NextState = "andando"
	--end
	if l_NextState ~= l_CurrentState then
		enemy:ChangeState(l_NextState)
		enemy:GetAnimationModel():ChangeAnimation(l_NextState, 0.2, 1.0)
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
		enemy:ChangeState("esperar")
		enemy:GetAnimationModel():ChangeAnimation("esperar", 0.2, 1.0)
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
	local timerName = "Espera" .. enemy:GetName()

	if not countdowntimer_manager:ExistTimer(timerName) then
		countdowntimer_manager:AddTimer(timerName, 2.0, false)
	else
		countdowntimer_manager:SetActive(timerName, true)
	end
	
	engine:Trace("Tiempo esperando: " .. countdowntimer_manager:GetElpasedTime(timerName))
	
	if countdowntimer_manager:isTimerFinish(timerName) then
		enemy:ChangeState("andando")
		enemy:GetAnimationModel():ChangeAnimation("andando", 0.2, 1.0)
		countdowntimer_manager:Reset(timerName, false)
	end
end

function atacar()
	local l_PlayerInSight = PlayerVisibility(enemy)
	--engine:Trace("Tiempo disparando" .. enemy:GetTimeToShoot() )
	if l_PlayerInSight then
		enemy = enemy_manager:GetActualEnemy()
		enemy:SetTimeToShoot(enemy:GetTimeToShoot() + timer:GetElapsedTime())
		engine:Trace("Tiempo disparando" .. enemy:GetTimeToShoot() )
		if enemy:GetTimeToShoot() >= enemy:GetMaxTimeToShoot() then
			g_Player:AddDamage(5)
			enemy:SetTimeToShoot(0.0)
		end
	else
		enemy:SetTimeToShoot(0.0)
		enemy:ChangeState("andando")
		enemy:GetAnimationModel():ChangeAnimation("andando", 0.2, 1.0)
	end
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