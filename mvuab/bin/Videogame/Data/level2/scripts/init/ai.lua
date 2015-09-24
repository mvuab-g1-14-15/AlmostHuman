tiempoDeEspera = 0

function init_enemy()
	enemy = g_EnemyManager:GetActualEnemy()
	timerPerseguir = "Perseguir Player"..enemy:GetName()
end

function check_next_state()
	enemy = g_EnemyManager:GetActualEnemy()
	timerPerseguir = "Perseguir Player"..enemy:GetName()
	local l_CurrentState = enemy:GetActualState()
	local l_NextState = l_CurrentState	
	local l_PlayerInSight = PlayerVisibility(enemy)
	local l_HearSomething = HearPlayer(enemy)
	
	if l_HearSomething then
		engine:Trace("He escuchado algo: "..enemy:GetName())
	end
	
	--local l_DistanceToPlayer = PlayerDistance(enemy)
	if l_CurrentState ~= "perseguir" then
		if l_HearSomething and l_PlayerInSight ~= true then
			l_NextState = "perseguir"
			enemy.Suspected = true
			enemy.SuspectedPosition = g_Player:GetPosition()
		end
		if l_CurrentState == "inicial" then
			l_NextState = "andando"
		end
		if l_CurrentState == "inicial" or l_CurrentState == "esperar" or l_CurrentState == "andando" or l_CurrentState == "perseguir" then
			if l_PlayerInSight then
				l_NextState = "atacar"
				engine:Trace("He entrado en atacar "..enemy:GetName())
				if countdowntimer_manager:ExistTimer(timerPerseguir) then
					countdowntimer_manager:Reset(timerPerseguir, false)
				end
				enemy.Suspected = false
				g_EnemyManager:SetAlarm(true)
			end
		end
	elseif countdowntimer_manager:ExistTimer(timerPerseguir) and countdowntimer_manager:IsActive(timerPerseguir) then
			if countdowntimer_manager:isTimerFinish(timerPerseguir) then
				l_NextState = "andando"
				countdowntimer_manager:Reset(timerPerseguir, false)
			end
	elseif l_PlayerInSight then
			l_NextState = "atacar"
			if countdowntimer_manager:IsActive(timerPerseguir) then
				countdowntimer_manager:Reset(timerPerseguir, false)
			end
			enemy.Suspected = false
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
	else
		enemy:MoveToWaypoint(Vect3f(0.0))
	end
end

function stay()
	local dt = timer:GetElapsedTime()
	enemy = g_EnemyManager:GetActualEnemy()
	if enemy.Suspected then
		if enemy:MoveToPos(enemy.SuspectedPosition) then
			enemy.Suspected = false
		end
	else
		enemy:MoveToWaypoint(Vect3f(0.0))
	end
end

function atacar()
	enemy = g_EnemyManager:GetActualEnemy()
	timerPerseguir = "Perseguir Player"..enemy:GetName()
	timerBurst = "Burst"..enemy:GetName()
	local l_PlayerInSight = PlayerVisibility(enemy)
	
	if l_PlayerInSight then
		if enemy.Suspected then
			enemy.Suspected = false
		end
		enemy:SetCountTimeShoot(enemy:GetCountTimeShoot() + timer:GetElapsedTime())		
		if enemy:GetCountTimeShoot() >= enemy:GetTimeToShoot() then
			if not countdowntimer_manager:ExistTimer(timerBurst) then
				countdowntimer_manager:AddTimer(timerBurst, enemy.TimeBurst, false)
			else
				countdowntimer_manager:SetActive(timerBurst, true)
			end
			if countdowntimer_manager:isTimerFinish(timerBurst) then
				local lDir = GetPlayerDirection(enemy:GetPosition())
				enemy:MakeShoot(lDir)
				enemy.ActualShootBurst = enemy.ActualShootBurst + 1
				countdowntimer_manager:Reset(timerBurst, false)
			end
			if enemy.ActualShootBurst == 0 then
				local lDir = GetPlayerDirection(enemy:GetPosition())
				enemy:MakeShoot(lDir)
				enemy.ActualShootBurst = enemy.ActualShootBurst + 1
			end
			if enemy.ActualShootBurst == enemy.NumShootBurst then
				enemy:SetCountTimeShoot(0.0)
				enemy.ActualShootBurst = 0
			end
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
	if enemy.Suspected then
		if enemy:MoveToPos(enemy.SuspectedPosition) then
			enemy.Suspected = false
		end
	else
		enemy:MoveToPlayer(l_TargetPos)
    end		
end