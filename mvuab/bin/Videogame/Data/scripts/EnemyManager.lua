class "CEnemyManager"

function CEnemyManager:__init()
	self.Enemy = {}
	self.Enemy.sala1 = {}
	self.Enemy.sala2 = {}
	self.Enemy.pasillo = {}
	self.Enemy.sala3 = {}
	self.Enemy.sala4 = {}
	
	self.GarbageMesh = {}
	self.GarbageMesh.sala1 = {}
	self.GarbageMesh.sala2 = {}
	self.GarbageMesh.pasillo = {}
	self.GarbageMesh.sala3 = {}
	self.GarbageMesh.sala4 = {}
	
	self.Shoots = {}
	self.MaxShoots = 100
	for i=1,self.MaxShoots do
		table.insert( self.Shoots, CAmmo(i) )
	end
	
	self.AStar = {}
	self.AStar.sala2 = CAStar( "sala2" )
	self.AStar.sala3 = CAStar( "sala3" )
	
	self.Alarm = {}
	self.Alarm.sala2 = false
	self.Alarm.pasillo = false
	self.Alarm.pasillo = false
	self.Alarm.sala3 = false
	self.Alarm.sala4 = false
	
	--self.AStar.sala2:SetRender()
	--self.AStar.sala3:SetRender()
	
	self:CreateEnemiesSala2()
	self:CreateEnemiesPasillo()
	self:CreateEnemiesSala3()
	self:CreateDesactivateEnemiesSala3()
end

function CEnemyManager:CreateEnemiesSala2()
	local lInfo = {}
	
	-- Patrol 1
	lInfo.name = "Enemy1_S2"
	lInfo.life = 100.0
	lInfo.damage = 10.0
	lInfo.radius = 0.4
	lInfo.height = 2.0
	lInfo.speed = 4.0
	lInfo.position = Vect3f(56.1998, -16.5334, -52.8975)
	lInfo.is_patrol = true
	lInfo.waypoints = { Vect3f(56.1998, -16.5334, -52.8975),
						Vect3f(68.5293, -16.5334, -52.8759),
						Vect3f(68.2447, -16.5334, -65.3863),
						Vect3f(56.1998, -16.5334, -65.033 )}
	lInfo.mesh = "enemy1"
	lInfo.room = "sala2"
	lInfo.use_gizmo = true
	lInfo.on_dead = false
	lInfo.shoot_speed = 50.0
	lInfo.time_to_shoot = 1.0
	lInfo.time_burst = 0.3
	lInfo.num_shoot_burst = 3.0
	lInfo.chase_distance = 20.0
	lInfo.camera_pitch = 0.0
	lInfo.camera_fov = 40.0
	lInfo.camera_far = 20.0
	lInfo.can_see = true
	lInfo.fly = false
	lInfo.can_use_graph = true
	lInfo.alarm = true
	lInfo.alarm_time = 5.0
	lInfo.active = true
	self.Enemy.sala2[lInfo.name] = CEnemy(lInfo)
	
	-- Patrol 2
	lInfo.name = "Enemy2_S2"
	lInfo.position = Vect3f(75.4508, -16.5334, -76.6861)
	lInfo.waypoints = { Vect3f(75.4508, -16.5334, -76.6861),
						Vect3f(49.5813, -16.5334, -76.6861),
						Vect3f(49.681, -16.5334, -43.0683 ),
						Vect3f(74.6564, -16.5334, -43.7134),
						Vect3f(74.6564, -16.5334, -54.4482)}
	self.Enemy.sala2[lInfo.name] = CEnemy(lInfo)

	-- Stairs Enemy
	lInfo.name = "Enemy3_S2"
	lInfo.life = 100.0
	lInfo.damage = 10.0
	lInfo.radius = 0.4
	lInfo.height = 2.0
	lInfo.speed = 4.0
	lInfo.position = Vect3f(63.044, -16.5334, -88.0)
	lInfo.is_patrol = false
	lInfo.mesh = "enemy1"
	lInfo.room = "sala2"
	lInfo.use_gizmo = true
	lInfo.on_dead = true
	lInfo.on_dead_code = "local closeEnemy = enemy_manager:GetCloseEnemyNotSelf(lPos, selfName);"..
						 "if closeEnemy ~= nil then "..
						 "	closeEnemy:SetSuspected(true);"..
						 "	closeEnemy:SetSuspectedPosition(lPos);"..
						 "end;"
	lInfo.shoot_speed = 50.0
	lInfo.time_to_shoot = 2.0
	lInfo.time_burst = 0.3
	lInfo.num_shoot_burst = 3.0
	lInfo.chase_distance = 5.0
	lInfo.camera_pitch = 0.0
	lInfo.camera_fov = 40.0
	lInfo.camera_far = 20.0
	lInfo.can_see = true
	lInfo.fly = false
	lInfo.can_use_graph = true
	lInfo.alarm = true
	lInfo.alarm_time = 10.0
	self.Enemy.sala2[lInfo.name] = CEnemy(lInfo)
end

function CEnemyManager:CreateEnemiesExtraSala2()
	local lInfo = {}
	
	-- Patrol 1
	lInfo.name = "ExtraEnemy1_S2"
	lInfo.life = 100.0
	lInfo.damage = 10.0
	lInfo.radius = 0.4
	lInfo.height = 2.0
	lInfo.speed = 4.0
	lInfo.position = Vect3f(53.8, -16.5334, -54.26)
	lInfo.is_patrol = false
	lInfo.mesh = "enemy1"
	lInfo.room = "sala2"
	lInfo.use_gizmo = true
	lInfo.on_dead = false
	lInfo.shoot_speed = 40.0
	lInfo.time_to_shoot = 1.0
	lInfo.time_burst = 0.3
	lInfo.num_shoot_burst = 3.0
	lInfo.chase_distance = 20.0
	lInfo.camera_pitch = 0.0
	lInfo.camera_fov = 40.0
	lInfo.camera_far = 20.0
	lInfo.can_see = true
	lInfo.fly = false
	lInfo.can_use_graph = true
	lInfo.alarm = true
	lInfo.alarm_time = 10.0
	lInfo.active = true
	self.Enemy.sala2[lInfo.name] = CEnemy(lInfo)

	-- Stairs Enemy
	lInfo.name = "ExtraEnemy2_S2"
	lInfo.life = 100.0
	lInfo.damage = 10.0
	lInfo.radius = 0.4
	lInfo.height = 2.0
	lInfo.speed = 4.0
	lInfo.position = Vect3f(73.14, -16.5334, -86.37)
	lInfo.is_patrol = false
	lInfo.mesh = "enemy1"
	lInfo.room = "sala2"
	lInfo.use_gizmo = true
	lInfo.on_dead = false
	lInfo.shoot_speed = 50.0
	lInfo.time_to_shoot = 2.0
	lInfo.time_burst = 0.3
	lInfo.num_shoot_burst = 3.0
	lInfo.chase_distance = 20.0
	lInfo.camera_pitch = 0.0
	lInfo.camera_fov = 40.0
	lInfo.camera_far = 20.0
	lInfo.can_see = true
	lInfo.fly = false
	lInfo.can_use_graph = true
	lInfo.alarm = true
	lInfo.alarm_time = 10.0
	self.Enemy.sala2[lInfo.name] = CEnemy(lInfo)
end

function CEnemyManager:CreateEnemiesPasillo()
	local lInfo = {}
	
	-- Drone 1
	lInfo.name = "Drone1_P"
	lInfo.life = 50.0
	lInfo.damage = 1.0
	lInfo.radius = 0.5
	lInfo.height = 0.5
	lInfo.speed = 2.0
	lInfo.position = Vect3f(54.0, -13.68, 6.31)
	lInfo.is_patrol = true
	lInfo.waypoints = { Vect3f(54.0, -13.68, 12.0),
						Vect3f(54.0, -13.68, 23.0)}
	lInfo.mesh = "drone"
	lInfo.room = "pasillo"
	lInfo.use_gizmo = true
	lInfo.on_dead = false
	lInfo.shoot_speed = 20.0
	lInfo.time_to_shoot = 1.0
	lInfo.time_burst = 0.2
	lInfo.num_shoot_burst = 5.0
	lInfo.chase_distance = 5.0
	lInfo.camera_pitch = -1.04719755 -- 60 grados
	lInfo.camera_fov = 25.0
	lInfo.camera_far = 20.0
	lInfo.can_see = true
	lInfo.fly = true
	lInfo.can_use_graph = false
	lInfo.alarm = true
	lInfo.alarm_time = 10.0
	lInfo.active = true
	self.Enemy.pasillo[lInfo.name] = CEnemy(lInfo)
end

function CEnemyManager:CreateEnemiesSala3()
	local lInfo = {}
	
	-- Drone 1
	lInfo.name = "Drone1_S3"
	lInfo.life = 50.0
	lInfo.damage = 1.0
	lInfo.radius = 0.5
	lInfo.height = 0.5
	lInfo.speed = 2.0
	lInfo.position = Vect3f(39.0, -12.0, 70.0)
	lInfo.is_patrol = true
	lInfo.waypoints = { Vect3f(39.0, -12.0, 70.0),
						Vect3f(39.0, -12.0, 50.0)}
	lInfo.mesh = "drone"
	lInfo.room = "pasillo"
	lInfo.use_gizmo = true
	lInfo.on_dead = false
	lInfo.shoot_speed = 20.0
	lInfo.time_to_shoot = 1.0
	lInfo.time_burst = 0.2
	lInfo.num_shoot_burst = 5.0
	lInfo.chase_distance = 5.0
	lInfo.camera_pitch = -1.04719755 -- 60 grados
	lInfo.camera_fov = 25.0
	lInfo.camera_far = 20.0
	lInfo.can_see = true
	lInfo.fly = true
	lInfo.can_use_graph = false
	lInfo.alarm = true
	lInfo.alarm_time = 10.0
	lInfo.active = true
	self.Enemy.pasillo[lInfo.name] = CEnemy(lInfo)
	
	-- Drone 2
	lInfo.name = "Drone2_S3"
	lInfo.position = Vect3f(25.0, -12.0, 70.0)
	lInfo.waypoints = { Vect3f(25.0, -12.0, 70.0),
						Vect3f(25.0, -12.0, 52.0)}
	self.Enemy.pasillo[lInfo.name] = CEnemy(lInfo)
end

function CEnemyManager:CreateDesactivateEnemiesSala3()
	local lInfo = {}
	
	-- Desactivate 1
	lInfo.name = "Enemy1_S3"
	lInfo.life = 100.0
	lInfo.damage = 10.0
	lInfo.radius = 0.4
	lInfo.height = 2.0
	lInfo.speed = 4.0
	lInfo.position = Vect3f(-38.44, -14.0, 65.41)
	lInfo.is_patrol = false
	lInfo.mesh = "enemy1"
	lInfo.room = "sala3"
	lInfo.use_gizmo = true
	lInfo.on_dead = false
	lInfo.shoot_speed = 50.0
	lInfo.time_to_shoot = 1.0
	lInfo.time_burst = 0.3
	lInfo.num_shoot_burst = 3.0
	lInfo.chase_distance = 20.0
	lInfo.camera_pitch = 0.0
	lInfo.camera_fov = 40.0
	lInfo.camera_far = 20.0
	lInfo.can_see = true
	lInfo.fly = false
	lInfo.can_use_graph = false
	lInfo.alarm = true
	lInfo.alarm_time = 5.0
	lInfo.active = false
	self.Enemy.sala3[lInfo.name] = CEnemy(lInfo)
	
	-- Desactivate 2
	lInfo.name = "Enemy2_S3"
	lInfo.position = Vect3f(-23.34, -14.0, 65.41)
	self.Enemy.sala3[lInfo.name] = CEnemy(lInfo)
end

function CEnemyManager:ActivateEnemiesSala3()
	for _,lEnemy in pairs (self.Enemy.sala3) do
		lEnemy:SetActive(true)
	end
end

function CEnemyManager:Reinit( aRoom )
	for _,lEnemy in pairs (self.Enemy[aRoom]) do
		lEnemy:SetOnDead(false)
		lEnemy:Destroy()
	end
	self.Enemy[aRoom] = {}
	
	for lName, lBool in pairs(self.GarbageMesh[aRoom]) do
		if lBool then
			renderable_objects_manager_characters:RemoveResource(lName)
		end
		self.GarbageMesh[aRoom][lName] = false
	end
	
	self.Alarm[aRoom] = false
	
	for k in pairs (self.Shoots) do
		self.Shoots[k]:End()
	end
		
	if aRoom == "sala2" then
		self:CreateEnemiesSala2()
	end
	if aRoom == "pasillo" then
		self:CreateEnemiesPasillo()
	end
	if aRoom == "sala3" then
		self:CreateEnemiesSala3()
		self:CreateDesactivateEnemiesSala3()
	end
	if aRoom == "sala4" then
		self.Boss:Destroy()
		self.Boss = nil
	end
end

function CEnemyManager:Update()
	--profiler:AddInit("CEnemyManager:Update()")
	for lRoom in pairs (self.Enemy) do
		for i in pairs(self.Enemy[lRoom]) do
			lActualEnemy = self.Enemy[lRoom][i]
			if lActualEnemy ~= nil then --http://swfoo.com/?p=623 hay que hacerlo así			
				if lActualEnemy:GetLife() > 0 then
					lActualEnemy:Update()
				else
					lActualEnemy:ChangeAnimationAction("death", 0.5, 1.0)
					self.GarbageMesh[lActualEnemy:GetRoom()][lActualEnemy:GetName()] = true
					--Add death particle
					lActualEnemy:Destroy()
					self.Enemy[lRoom][i] = nil
					--table.remove(self.Enemy[lRoom], i)
					--collectgarbage()
				end
			end
		end
	end
	
	if self.Boss ~= nil then
		self.Boss:Update()
		if self.Boss:GetLife() <= 0 then
			--execute dead code
		end
	end
	
	for k in pairs (self.Shoots) do
		if self.Shoots[k]:IsImpacted() then
			self.Shoots[k]:End()
		end
	end
	
	for k in pairs (self.Shoots) do
		self.Shoots[k]:Update()
	end
	--profiler:AddEnd("CEnemyManager:Update()")
end

function CEnemyManager:GetNumEnemy( aRoom )
	lCount = 0
	lRoom = self.Enemy[aRoom]
	for _,lEnemy in pairs (lRoom) do
		if lEnemy:GetLife() > 0.0 then
			lCount = lCount + 1
		end
	end
	return lCount
end

function CEnemyManager:SetAlarm( aRoom )
	lRoom = self.Enemy[aRoom]
	for i in pairs(lRoom) do
		lActualEnemy = lRoom[i]
		lActualEnemy:SetSuspected(true)
		lActualEnemy:SetSuspectedPosition(g_Player:GetPosition())
	end
	
	if not self.Alarm[aRoom] then
		if aRoom == "sala2" then
			self:CreateEnemiesExtraSala2()
		end
		self.Alarm[aRoom] = true
	end
end

function CEnemyManager:CreateBoss()
	if self.Boss == nil then
		self.Boss = CBoss()
	end
end

function CEnemyManager:GetBoss()
	return self.Boss
end

function CEnemyManager:GetCloseEnemy(aPos)
	lDist = 999999.99
	lActualDist = 0.0
	lEnemy = nil
	lActualEnemy = 0

	for lRoom in pairs (self.Enemy) do
		for i in pairs(self.Enemy[lRoom]) do
			lActualEnemy = self.Enemy[lRoom][i]
			lActualDist = lActualEnemy:GetPosition():Distance( aPos )

			if ( lActualDist < lDist ) then
			  lDist = lActualDist
			  lEnemy = lActualEnemy
			end
		end
	end

	return lEnemy
end

function CEnemyManager:GetEnemiesAtDistance( aDist )
	lEnemyList = {}
	local PlayerPos = g_Player:GetPosition()
	for lRoom in pairs (self.Enemy) do
		for i in pairs(self.Enemy[lRoom]) do
			lActualEnemy = self.Enemy[lRoom][i]
			if lActualEnemy ~= nil then
				local EnemyPos = lActualEnemy:GetPosition()
				local Dist = PlayerPos:Distance(EnemyPos)
				if Dist <= aDist then
					table.insert(lEnemyList, lActualEnemy)
				end
			end
		end
	end
	return lEnemyList
end

function CEnemyManager:GetCloseEnemyNotSelf(aPos, aName)
	lDist = 999999.99
	lActualDist = 0.0
	lEnemy = nil
	lActualEnemy = 0

	for lRoom in pairs (self.Enemy) do
		for i in pairs(self.Enemy[lRoom]) do
			lActualEnemy = self.Enemy[lRoom][i]
			if lActualEnemy:GetName() ~= aName then
				lActualDist = lActualEnemy:GetPosition():Distance( aPos )

				if ( lActualDist < lDist ) then
					lDist = lActualDist
					lEnemy = lActualEnemy
				end
			end
		end
	end

	return lEnemy
end

function CEnemyManager:GetEnemy( aName )
	for lRoom in pairs (self.Enemy) do
		if self.Enemy[lRoom][aName] ~= nil then
			return self.Enemy[lRoom][aName]
		end
	end
end

function CEnemyManager:AddDamage(aName, aAmount)
	for lRoom in pairs (self.Enemy) do
		if self.Enemy[lRoom][aName] ~= nil then
			self.Enemy[lRoom][aName]:AddDamage(aAmount)
		end
	end
end

function CEnemyManager:GetAStar( aRoomName )
	return self.AStar[ aRoomName ]
end

function CEnemyManager:AddShoot( aPosition, aDirection, aSpeed, aDamage )
	for k in pairs (self.Shoots) do
		if not self.Shoots[k]:IsActive() then
			self.Shoots[k]:Begin(aPosition, aDirection, aSpeed, aDamage)
			return
		end
	end
end

function CEnemyManager:CollisionWithEnemy(aName, aPos, aRadius)
	lEnemy = self:GetCloseEnemyNotSelf(aPos, aName)
	if lEnemy == nil then
		return false, nil
	end
	local lPos = lEnemy:GetPosition()
	local lDist = lPos:Distance(aPos)
	return lDist < aRadius * 3.0, lEnemy
end

--dofile("./data/level2/scripts/StateMachine.lua")
--dofile("./data/level2/scripts/CoreEnemy.lua")
--dofile("./data/level2/scripts/shoot.lua")
--
--class 'CEnemyManagerLUA'
--
--function CEnemyManagerLUA:__init()
--	--Mapas
--	self.StatesMachine = {}
--	self.CoreEnemy = {}
--	self.Enemy = {}
--	self.ExtraEnemy = {}
--	self.Routes = {}
--	self.ActualEnemy = nil
--	self.AStar = {}
--	self.Shoots = {}
--	self.Alarm = false
--	self.RoomAlarm = false
--	self.ExtraEnemyCount = 0
--	self.ExtraEnemyCountMax = 3
--	self.timerExtraEnemy = "Add Extra Enemy"
--	
--	self.Boss = nil
--	
--	self:Load("Data/enemies/enemies.xml")
--end
--
--function CEnemyManagerLUA:Load(filename)
--	
--	l_File = CXMLTreeNode()
--	Node = CXMLTreeNode()
--    if not l_File:LoadAndFindNode( filename, "enemies", Node ) then
--        --engine:Trace( "File '"..filename.."' not correctly loaded" )
--    end
--
--    if Node:Exists() then
--        count = Node:GetNumChildren();
--
--        for i = 0, count do
--			CurrentNode = Node:GetChildren(i)
--            l_TagName = CurrentNode:GetName()
--
--            if ( l_TagName == "enemy" ) then
--                self:AddNewEnemy( CurrentNode )
--            elseif ( l_TagName == "core_enemy" ) then           
--                self:AddNewCoreEnemy( CurrentNode )                          
--            elseif( l_TagName == "route" ) then           
--                self:AddNewRoute( CurrentNode )
--			elseif ( l_TagName == "extra_enemy" ) then
--				self:AddExtraEnemy( CurrentNode )
--            end
--
--        end
--    end
--	
--	self.AStar[ "sala2" ] = CAStar( "sala2" )
--	self.AStar[ "sala3" ] = CAStar( "sala3" )
--end
--
--function CEnemyManagerLUA:Update()
--	if self.Alarm then
--		if ( self.ExtraEnemyCount < self.ExtraEnemyCountMax and ( not countdowntimer_manager:ExistTimer(self.timerExtraEnemy) or countdowntimer_manager:isTimerFinish(self.timerExtraEnemy) ) ) then			
--			self:GenerateEnemy()
--			countdowntimer_manager:AddTimer(self.timerExtraEnemy, 10.0, false)
--			
--		elseif countdowntimer_manager:isTimerFinish(self.timerExtraEnemy) then
--			countdowntimer_manager:Reset(self.timerExtraEnemy, false)
--		end
--	end
--	for i in pairs (self.Enemy) do
--		if self.Enemy[i] ~= nil then --http://swfoo.com/?p=623 hay que hacerlo así
--			self.ActualEnemy = self.Enemy[i]
--			timerHurt = "Recibo disparo "..self.ActualEnemy:GetName()			
--			if self.ActualEnemy:GetLife() > 0 and not self.ActualEnemy.IsHurting and self.ActualEnemy.CurrentAnimation ~="hurt" then
--				self.ActualEnemy:Update()
--				
--			elseif self.ActualEnemy:GetLife() > 0 and self.ActualEnemy.IsHurting and self.ActualEnemy.CurrentAnimation ~= "hurt" then
--				self.ActualEnemy.IsHurting = false
--				self.ActualEnemy.BackAnimation = self.ActualEnemy.CurrentAnimation
--				self.ActualEnemy:ChangeAnimation("hurt", 0.2, 1.0)
--				if not countdowntimer_manager:ExistTimer(timerHurt) then
--					countdowntimer_manager:AddTimer(timerHurt, 1.0, false)
--				else
--					countdowntimer_manager:SetActive(timerHurt, true)
--				end
--			elseif self.ActualEnemy:GetLife() > 0 and countdowntimer_manager:ExistTimer(timerHurt) and countdowntimer_manager:IsActive(timerHurt) then
--				self.ActualEnemy.IsHurting = false
--				if countdowntimer_manager:isTimerFinish(timerHurt) then
--					self.ActualEnemy:ChangeAnimation(self.ActualEnemy.BackAnimation, 0.2, 1)
--					countdowntimer_manager:Reset(timerHurt, false)
--				end
--			else
--				if self.ActualEnemy:GetVelocity() == 5.0 then
--					self.ActualEnemy:SetVelocity(2.0)
--				end
--				self.ActualEnemy.IsHurting = false
--				
--				if not countdowntimer_manager:ExistTimer(self.ActualEnemy:GetName().."DeadTimer") then
--					AnimatedModel = self.ActualEnemy:GetAnimationModel()
--					AnimatedModel:ChangeAnimationAction( "morir", 0.2, 0.2 )
--					countdowntimer_manager:AddTimer(self.ActualEnemy:GetName().."DeadTimer", 10.0, false)
--				end
--			
--				if countdowntimer_manager:isTimerFinish(self.ActualEnemy:GetName().."DeadTimer") then				
--					countdowntimer_manager:Reset(self.ActualEnemy:GetName().."DeadTimer", false)
--					
--					self.ActualEnemy:Destroy()
--					self.Enemy[i] = nil
--					--table.remove(self.Enemy, i)
--					collectgarbage()
--				end
--			end
--		end
--	end
--	
--	for i in pairs (self.Enemy) do
--		if self.Enemy[i] ~= nil then --http://swfoo.com/?p=623 hay que hacerlo así
--			self.ActualEnemy = self.Enemy[i]
--			if self.ActualEnemy:GetActualState() == "idle" and self.RoomAlarm == true and self.Room == self.ActualEnemy:GetRoom() then
--				self.ActualEnemy:ChangeRoute(self.Routes[self.ActualEnemy.IdRouteAlarm])
--				self.ActualEnemy:ChangeState("activation")
--			elseif self.RoomAlarm == true and self.Room == self.ActualEnemy:GetRoom() then					
--				self.ActualEnemy:ChangeRoute(self.Routes[self.ActualEnemy.IdRouteAlarm])
--			end	
--		end
--	end
--	
--	if self.Boss ~= nil then
--		self.Boss:Update()
--	end
--	
--	self.RoomAlarm = false
--	
--	for k in pairs (self.Shoots) do
--		if self.Shoots[k]:GetImpacted() then
--			self.Shoots[k]:Destroy()
--			table.remove(self.Shoots, k)
--			collectgarbage()
--		end
--	end
--	
--	for k in pairs (self.Shoots) do
--		self.Shoots[k]:Update()
--	end
--end
--
--function CEnemyManagerLUA:AddNewCoreEnemy( Node )
--	CoreEnemy = CCoreEnemyLUA()
--    
--    CoreEnemy:SetEnemyType(Node:GetAttributeString( "type", "no_type" ))
--	CoreEnemy:SetLife(Node:GetAttributeFloat( "life", 0.0 ))
--	CoreEnemy:SetDamage(Node:GetAttributeFloat( "damage", 1.0 ))
--	CoreEnemy:SetRespawnTime(Node:GetAttributeFloat( "time_to_respawn", 0.0 ))
--	CoreEnemy:SetTimeToShoot(Node:GetAttributeFloat( "time_to_shoot", 0.0 ))
--	CoreEnemy:SetShootAccuracy(Node:GetAttributeFloat( "shoot_accuracy", 0.0 ))
--	CoreEnemy:SetStateMachineName(Node:GetAttributeString( "state_machine_name", "no_name" ))
--	CoreEnemy:SetStateMachineFileName(Node:GetAttributeString( "state_machine_file", "no_name" ))
--	
--	StateMachine = CStateMachineLUA(CoreEnemy:GetStateMachineFileName())
--	
--	if self.StatesMachine[CoreEnemy:GetStateMachineName()] == nil then
--		self.StatesMachine[CoreEnemy:GetStateMachineName()] = StateMachine
--	end	
--	if self.CoreEnemy[CoreEnemy:GetEnemyType()] == nil then
--		self.CoreEnemy[CoreEnemy:GetEnemyType()] = CoreEnemy
--	end
--end
--
--function CEnemyManagerLUA:AddNewRoute( Node )
--	l_Id = Node:GetAttributeInt( "id", -1 )
--    count = Node:GetNumChildren()
--    l_Route = {}
--    for i = 0, count-1 do
--		CurrentNode = Node:GetChildren(i)
--        l_Point = Vect3f(CurrentNode:GetAttributeVect3f( "value", Vect3f( 0.0, -99999999.0, 0.0 ) ))
--		--engine:Trace("Nodo ".. i)
--        if ( l_Point == Vect3f( 0.0, -99999999.0, 0.0 ) ) then        
--            --engine:Trace( "Point in the route '".. l_Id .."' not correctly loaded.")        
--        else     
--            table.insert(l_Route, l_Point)
--        end
--    end
--
--    self.Routes[l_Id] = l_Route
--end
--
--function CEnemyManagerLUA:AddNewEnemy( Node )
--	lType = Node:GetAttributeString( "type", "no_type" )
--    lCoreEnemy = self.CoreEnemy[lType]
--
--    if ( lCoreEnemy == nil) then
--        --engine:Trace("Core '".. lType .."' not found")
--    end
--
--    lStateMachine = self.StatesMachine[self.CoreEnemy[lType]:GetStateMachineName()]
--
--    if ( lStateMachine == nil) then
--        --engine:Trace("State machine for '".. lType .."' not found")
--    end
--	
--	lEnemy = nil
--	
--    if lType == "patroll" then
--		lEnemy = CPatrolEnemyLUA(Node, self.Routes[Node:GetAttributeInt("route", -1)], lStateMachine, lCoreEnemy)
--	elseif lType == "drone" then		
--		lEnemy = CDroneEnemyLUA(Node, self.Routes[Node:GetAttributeInt("route", -1)], lStateMachine, lCoreEnemy)	
--	elseif lType == "easy" then
--		lEnemy = CEasyEnemyLUA(Node, self.Routes[Node:GetAttributeInt("route", -1)], lStateMachine, lCoreEnemy)	
--	end
--	name = Node:GetAttributeString("name", "no_name")
--	--engine:Trace("Enemy: "..name)
--	self.Enemy[name] = lEnemy
--	self.ActualEnemy = lEnemy
--end
--
--function CEnemyManagerLUA:AddExtraEnemy( Node )
--	local lRoomName = Node:GetAttributeString("name", "no_name")
--	self.ExtraEnemy[lRoomName] = Node
--end
--
--function CEnemyManagerLUA:GetActualEnemy()
--	return self.ActualEnemy
--end
--
--function CEnemyManagerLUA:GetAStar( aRoomName )
--	return self.AStar[ aRoomName ]
--end
--
--function CEnemyManagerLUA:GetResource(name)
--	return self.Enemy[name]
--end
--
--function CEnemyManagerLUA:GetEnemy( aName )
--	return self.Enemy[aName]
--end
--
--function CEnemyManagerLUA:AddDamage(name)
--	lEnemy = self.Enemy[name]
--	lEnemy:AddDamage(25.0)
--end
--
--function CEnemyManagerLUA:GetCloseEnemy(aPos)
--	lDist = 999999.99
--	lActualDist = 0.0
--	lEnemy = nil
--	lActualEnemy = 0
--
--	for i in pairs (self.Enemy) do
--		lActualEnemy = self.Enemy[i]
--		lActualDist = lActualEnemy:GetPosition():Distance( aPos )
--
--		if ( lActualDist < lDist ) then
--		  lDist = lActualDist
--		  lEnemy = lActualEnemy
--		end
--	end
--
--	return lEnemy
--end
--
--function CEnemyManagerLUA:GetCloseEnemyNotSelf(aPos, aName)
--	engine:Trace("Getting close enemy, not self")
--	lDist = 999999.99
--	lActualDist = 0.0
--	lEnemy = nil
--	lActualEnemy = 0
--
--	for i in pairs (self.Enemy) do
--		lActualEnemy = self.Enemy[i]
--		if lActualEnemy:GetName() ~= aName then
--			lActualDist = lActualEnemy:GetPosition():Distance( aPos )
--
--			if ( lActualDist < lDist ) then
--				lDist = lActualDist
--				lEnemy = lActualEnemy
--			end
--		end
--	end
--
--	return lEnemy
--end
--
--function CEnemyManagerLUA:AddShoot(aShoot)
--	table.insert(self.Shoots, aShoot)
--end
--
--function CEnemyManagerLUA:GetEnemys()
--	return self.Enemy
--end
--
--function CEnemyManagerLUA:GetNumEnemys()
--	local count = 0
--	for i in pairs (self.Enemy) do
--		count = count + 1
--	end
--	return count
--end
--
--function CEnemyManagerLUA:SetAlarm(aAlarm)
--	self.Alarm = aAlarm
--end
--
--function CEnemyManagerLUA:GenerateEnemy()
--	local lRoom = "room2"--scene:GetActivateRoom()
--	--engine:Trace("Name room:"..lRoom)
--	local Node = self.ExtraEnemy[lRoom]
--	if not Node == nil then
--		self:AddNewEnemy( Node:GetChildren(self.ExtraEnemyCount) )
--		self.ExtraEnemyCount = self.ExtraEnemyCount + 1
--	end
--end
--
--function CEnemyManagerLUA:GetEnemiesAtDistance( aDist )
--	lEnemyList = {}
--	local PlayerPos = g_Player:GetPosition()
--	for i in pairs (self.Enemy) do
--		if self.Enemy[i] ~= nil then
--			local EnemyPos = self.Enemy[i]:GetPosition()
--			local Dist = PlayerPos:Distance(EnemyPos)
--			if Dist <= aDist then
--				table.insert(lEnemyList, self.Enemy[i])
--			end
--		end
--	end
--	return lEnemyList
--end
--
--function CEnemyManagerLUA:AlarmRoom(room)	
--	self.Room = room
--	self.RoomAlarm = true
--end
--
--function CEnemyManagerLUA:CreateBoss()
--	self.Boss = CBoss()
--end