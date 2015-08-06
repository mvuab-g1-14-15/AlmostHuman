tiempoDeEspera = 0

function init_enemy()
	enemy = g_EnemyManager:GetActualEnemy()
	timerPerseguir = "Perseguir Player"
end

function check_next_state()
	
	enemy = g_EnemyManager:GetActualEnemy()
	local l_CurrentState = enemy:GetActualState()
	local l_NextState = l_CurrentState	
	local l_PlayerInSight = PlayerVisibility(enemy)
	local l_DistanceToPlayer = PlayerDistance(enemy)
	if l_CurrentState ~= "perseguir" then	
		if l_CurrentState == "inicial" then
			l_NextState = "andando"
		end
		--if l_CurrentState == "inicial" or l_CurrentState == "esperar" or l_CurrentState == "andando" then
		--	if l_DistanceToPlayer <= 8 and l_PlayerInSight then
		--		l_NextState = "perseguir"
		--	end
		--end
		if l_CurrentState == "inicial" or l_CurrentState == "esperar" or l_CurrentState == "andando" or l_CurrentState == "perseguir" then
			if l_DistanceToPlayer < 8 and l_PlayerInSight then
				l_NextState = "atacar"
				if countdowntimer_manager:IsActive(timerPerseguir) then
					countdowntimer_manager:Reset(timerPerseguir, false)
				end
			end
		end	
	elseif countdowntimer_manager:ExistTimer(timerPerseguir) and countdowntimer_manager:IsActive(timerPerseguir) then
			if countdowntimer_manager:isTimerFinish(timerPerseguir) then
				l_NextState = "andando"
				countdowntimer_manager:Reset(timerPerseguir, false)
			end
	elseif l_DistanceToPlayer < 8 and l_PlayerInSight then
			l_NextState = "atacar"
			if countdowntimer_manager:IsActive(timerPerseguir) then
				countdowntimer_manager:Reset(timerPerseguir, false)
			end
	end
	
	if l_NextState ~= l_CurrentState then
		enemy:ChangeState(l_NextState)
		enemy:GetAnimationModel():ChangeAnimation(l_NextState, 0.2, 1.0)
	end	
	
end

function andar()
	local dt = timer:GetElapsedTime()
	enemy = g_EnemyManager:GetActualEnemy()
	
	if enemy:IsInWaypoint() then
		enemy:NextWaypoint()
		--if (enemy:GetActualState() == "andando" ) then
		--	enemy:ChangeState("esperar")
		--	enemy:GetAnimationModel():ChangeAnimation("andando", 0.2, 1.0)
		--end
	else
		enemy:MoveToWaypoint(Vect3f(0.0))
	end
end

function esperar()
	enemy = g_EnemyManager:GetActualEnemy()
	local timerName = "Espera" .. enemy:GetName()

	if not countdowntimer_manager:ExistTimer(timerName) then
		countdowntimer_manager:AddTimer(timerName, 2.0, false)
	else
		countdowntimer_manager:SetActive(timerName, true)
	end
	
	--engine:Trace("Tiempo esperando: " .. countdowntimer_manager:GetElpasedTime(timerName))
	
	if countdowntimer_manager:isTimerFinish(timerName) and enemy:GetActualState() == "esperar" then
		enemy:ChangeState("andando")
		enemy:GetAnimationModel():ChangeAnimation("andando", 0.2, 1.0)
		countdowntimer_manager:Reset(timerName, false)
	end
end

function atacar()
	enemy = g_EnemyManager:GetActualEnemy()
	local l_PlayerInSight = PlayerVisibility(enemy)
	
	if l_PlayerInSight then
		enemy:SetCountTimeShoot(enemy:GetCountTimeShoot() + timer:GetElapsedTime())		
		if enemy:GetCountTimeShoot() >= enemy:GetTimeToShoot() then
			--engine:Trace("Enemy shooting")
			local lDir = GetPlayerDirection(enemy:GetPosition())
			enemy:MakeShoot(lDir)
			enemy:SetCountTimeShoot(0.0)
		end
	else
		enemy:SetCountTimeShoot(0.0)
		if enemy:GetActualState() == "atacar" then
			enemy:ChangeState("perseguir")
			enemy:GetAnimationModel():ChangeAnimation("perseguir", 0.2, 1.0)
			if not countdowntimer_manager:ExistTimer(timerPerseguir) then
				countdowntimer_manager:AddTimer(timerPerseguir, 10.0, false)
			else
				countdowntimer_manager:SetActive(timerPerseguir, true)
			end
		end
	end
end

function perseguir()
	enemy = g_EnemyManager:GetActualEnemy()
	local l_TargetPos = GetPlayerPosition()
	enemy:MoveToPlayer(l_TargetPos)	
end