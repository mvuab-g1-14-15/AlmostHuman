dofile("./data/level2/scripts/StateMachine.lua")
dofile("./data/level2/scripts/CoreEnemy.lua")
dofile("./data/level2/scripts/shoot.lua")

class 'CEnemyManagerLUA'

function CEnemyManagerLUA:__init()
	--Mapas
	self.StatesMachine = {}
	self.CoreEnemy = {}
	self.Enemy = {}
	self.ExtraEnemy = {}
	self.Routes = {}
	self.ActualEnemy = nil
	self.AStar = {}
	self.Shoots = {}
	self.Alarm = false
	self.RoomAlarm = false
	self.ExtraEnemyCount = 0
	self.ExtraEnemyCountMax = 3
	self.timerExtraEnemy = "Add Extra Enemy"
	
	self.Boss = nil
	
	self:Load("Data/enemies/enemies.xml")
end

function CEnemyManagerLUA:Load(filename)
	
	l_File = CXMLTreeNode()
	Node = CXMLTreeNode()
    if not l_File:LoadAndFindNode( filename, "enemies", Node ) then
        --engine:Trace( "File '"..filename.."' not correctly loaded" )
    end

    if Node:Exists() then
        count = Node:GetNumChildren();

        for i = 0, count do
			CurrentNode = Node:GetChildren(i)
            l_TagName = CurrentNode:GetName()

            if ( l_TagName == "enemy" ) then
                self:AddNewEnemy( CurrentNode )
            elseif ( l_TagName == "core_enemy" ) then           
                self:AddNewCoreEnemy( CurrentNode )                          
            elseif( l_TagName == "route" ) then           
                self:AddNewRoute( CurrentNode )
			elseif ( l_TagName == "extra_enemy" ) then
				self:AddExtraEnemy( CurrentNode )
            end

        end
    end
	
	self.AStar[ "sala2" ] = CAStar( "sala2" )
	self.AStar[ "sala3" ] = CAStar( "sala3" )
end

function CEnemyManagerLUA:Update()
	if self.Alarm then
		if ( self.ExtraEnemyCount < self.ExtraEnemyCountMax and ( not countdowntimer_manager:ExistTimer(self.timerExtraEnemy) or countdowntimer_manager:isTimerFinish(self.timerExtraEnemy) ) ) then			
			self:GenerateEnemy()
			countdowntimer_manager:AddTimer(self.timerExtraEnemy, 10.0, false)
			
		elseif countdowntimer_manager:isTimerFinish(self.timerExtraEnemy) then
			countdowntimer_manager:Reset(self.timerExtraEnemy, false)
		end
	end
	for i in pairs (self.Enemy) do
		if self.Enemy[i] ~= nil then --http://swfoo.com/?p=623 hay que hacerlo así
			self.ActualEnemy = self.Enemy[i]
			timerHurt = "Recibo disparo "..self.ActualEnemy:GetName()			
			if self.ActualEnemy:GetLife() > 0 and not self.ActualEnemy.IsHurting and self.ActualEnemy.CurrentAnimation ~="hurt" then
				self.ActualEnemy:Update()
				
			elseif self.ActualEnemy:GetLife() > 0 and self.ActualEnemy.IsHurting and self.ActualEnemy.CurrentAnimation ~= "hurt" then
				self.ActualEnemy.IsHurting = false
				self.ActualEnemy.BackAnimation = self.ActualEnemy.CurrentAnimation
				self.ActualEnemy:ChangeAnimation("hurt", 0.2, 1.0)
				if not countdowntimer_manager:ExistTimer(timerHurt) then
					countdowntimer_manager:AddTimer(timerHurt, 1.0, false)
				else
					countdowntimer_manager:SetActive(timerHurt, true)
				end
			elseif self.ActualEnemy:GetLife() > 0 and countdowntimer_manager:ExistTimer(timerHurt) and countdowntimer_manager:IsActive(timerHurt) then
				self.ActualEnemy.IsHurting = false
				if countdowntimer_manager:isTimerFinish(timerHurt) then
					self.ActualEnemy:ChangeAnimation(self.ActualEnemy.BackAnimation, 0.2, 1)
					countdowntimer_manager:Reset(timerHurt, false)
				end
			else
				if self.ActualEnemy:GetVelocity() == 5.0 then
					self.ActualEnemy:SetVelocity(2.0)
				end
				self.ActualEnemy.IsHurting = false
				
				if not countdowntimer_manager:ExistTimer(self.ActualEnemy:GetName().."DeadTimer") then
					AnimatedModel = self.ActualEnemy:GetAnimationModel()
					AnimatedModel:ChangeAnimationAction( "morir", 0.2, 0.2 )
					countdowntimer_manager:AddTimer(self.ActualEnemy:GetName().."DeadTimer", 10.0, false)
				end
			
				if countdowntimer_manager:isTimerFinish(self.ActualEnemy:GetName().."DeadTimer") then				
					countdowntimer_manager:Reset(self.ActualEnemy:GetName().."DeadTimer", false)
					
					self.ActualEnemy:Destroy()
					self.Enemy[i] = nil
					--table.remove(self.Enemy, i)
					collectgarbage()
				end
			end
		end
	end
	
	for i in pairs (self.Enemy) do
		if self.Enemy[i] ~= nil then --http://swfoo.com/?p=623 hay que hacerlo así
			self.ActualEnemy = self.Enemy[i]
			if self.ActualEnemy:GetActualState() == "idle" and self.RoomAlarm == true and self.Room == self.ActualEnemy:GetRoom() then
				self.ActualEnemy:ChangeRoute(self.Routes[self.ActualEnemy.IdRouteAlarm])
				self.ActualEnemy:ChangeState("activation")
			elseif self.RoomAlarm == true and self.Room == self.ActualEnemy:GetRoom() then					
				self.ActualEnemy:ChangeRoute(self.Routes[self.ActualEnemy.IdRouteAlarm])
			end	
		end
	end
	
	if self.Boss ~= nil then
		self.Boss:Update()
	end
	
	self.RoomAlarm = false
	
	for k in pairs (self.Shoots) do
		if self.Shoots[k]:GetImpacted() then
			self.Shoots[k]:Destroy()
			table.remove(self.Shoots, k)
			collectgarbage()
		end
	end
	
	for k in pairs (self.Shoots) do
		self.Shoots[k]:Update()
	end
end

function CEnemyManagerLUA:AddNewCoreEnemy( Node )
	CoreEnemy = CCoreEnemyLUA()
    
    CoreEnemy:SetEnemyType(Node:GetAttributeString( "type", "no_type" ))
	CoreEnemy:SetLife(Node:GetAttributeFloat( "life", 0.0 ))
	CoreEnemy:SetDamage(Node:GetAttributeFloat( "damage", 1.0 ))
	CoreEnemy:SetRespawnTime(Node:GetAttributeFloat( "time_to_respawn", 0.0 ))
	CoreEnemy:SetTimeToShoot(Node:GetAttributeFloat( "time_to_shoot", 0.0 ))
	CoreEnemy:SetShootAccuracy(Node:GetAttributeFloat( "shoot_accuracy", 0.0 ))
	CoreEnemy:SetStateMachineName(Node:GetAttributeString( "state_machine_name", "no_name" ))
	CoreEnemy:SetStateMachineFileName(Node:GetAttributeString( "state_machine_file", "no_name" ))
	
	StateMachine = CStateMachineLUA(CoreEnemy:GetStateMachineFileName())
	
	if self.StatesMachine[CoreEnemy:GetStateMachineName()] == nil then
		self.StatesMachine[CoreEnemy:GetStateMachineName()] = StateMachine
	end	
	if self.CoreEnemy[CoreEnemy:GetEnemyType()] == nil then
		self.CoreEnemy[CoreEnemy:GetEnemyType()] = CoreEnemy
	end
end

function CEnemyManagerLUA:AddNewRoute( Node )
	l_Id = Node:GetAttributeInt( "id", -1 )
    count = Node:GetNumChildren()
    l_Route = {}
    for i = 0, count-1 do
		CurrentNode = Node:GetChildren(i)
        l_Point = Vect3f(CurrentNode:GetAttributeVect3f( "value", Vect3f( 0.0, -99999999.0, 0.0 ) ))
		--engine:Trace("Nodo ".. i)
        if ( l_Point == Vect3f( 0.0, -99999999.0, 0.0 ) ) then        
            --engine:Trace( "Point in the route '".. l_Id .."' not correctly loaded.")        
        else     
            table.insert(l_Route, l_Point)
        end
    end

    self.Routes[l_Id] = l_Route
end

function CEnemyManagerLUA:AddNewEnemy( Node )
	lType = Node:GetAttributeString( "type", "no_type" )
    lCoreEnemy = self.CoreEnemy[lType]

    if ( lCoreEnemy == nil) then
        --engine:Trace("Core '".. lType .."' not found")
    end

    lStateMachine = self.StatesMachine[self.CoreEnemy[lType]:GetStateMachineName()]

    if ( lStateMachine == nil) then
        --engine:Trace("State machine for '".. lType .."' not found")
    end
	
	lEnemy = nil
	
    if lType == "patroll" then
		lEnemy = CPatrolEnemyLUA(Node, self.Routes[Node:GetAttributeInt("route", -1)], lStateMachine, lCoreEnemy)
	elseif lType == "drone" then		
		lEnemy = CDroneEnemyLUA(Node, self.Routes[Node:GetAttributeInt("route", -1)], lStateMachine, lCoreEnemy)	
	elseif lType == "easy" then
		lEnemy = CEasyEnemyLUA(Node, self.Routes[Node:GetAttributeInt("route", -1)], lStateMachine, lCoreEnemy)	
	end
	name = Node:GetAttributeString("name", "no_name")
	--engine:Trace("Enemy: "..name)
	self.Enemy[name] = lEnemy
	self.ActualEnemy = lEnemy
end

function CEnemyManagerLUA:AddExtraEnemy( Node )
	local lRoomName = Node:GetAttributeString("name", "no_name")
	self.ExtraEnemy[lRoomName] = Node
end

function CEnemyManagerLUA:GetActualEnemy()
	return self.ActualEnemy
end

function CEnemyManagerLUA:GetAStar( aRoomName )
	return self.AStar[ aRoomName ]
end

function CEnemyManagerLUA:GetResource(name)
	return self.Enemy[name]
end

function CEnemyManagerLUA:GetEnemy( aName )
	return self.Enemy[aName]
end

function CEnemyManagerLUA:AddDamage(name)
	lEnemy = self.Enemy[name]
	lEnemy:AddDamage(25.0)
end

function CEnemyManagerLUA:GetCloseEnemy(aPos)
	lDist = 999999.99
	lActualDist = 0.0
	lEnemy = nil
	lActualEnemy = 0

	for i in pairs (self.Enemy) do
		lActualEnemy = self.Enemy[i]
		lActualDist = lActualEnemy:GetPosition():Distance( aPos )

		if ( lActualDist < lDist ) then
		  lDist = lActualDist
		  lEnemy = lActualEnemy
		end
	end

	return lEnemy
end

function CEnemyManagerLUA:GetCloseEnemyNotSelf(aPos, aName)
	engine:Trace("Getting close enemy, not self")
	lDist = 999999.99
	lActualDist = 0.0
	lEnemy = nil
	lActualEnemy = 0

	for i in pairs (self.Enemy) do
		lActualEnemy = self.Enemy[i]
		if lActualEnemy:GetName() ~= aName then
			lActualDist = lActualEnemy:GetPosition():Distance( aPos )

			if ( lActualDist < lDist ) then
				lDist = lActualDist
				lEnemy = lActualEnemy
			end
		end
	end

	return lEnemy
end

function CEnemyManagerLUA:AddShoot(aShoot)
	table.insert(self.Shoots, aShoot)
end

function CEnemyManagerLUA:GetEnemys()
	return self.Enemy
end

function CEnemyManagerLUA:GetNumEnemys()
	local count = 0
	for i in pairs (self.Enemy) do
		count = count + 1
	end
	return count
end

function CEnemyManagerLUA:SetAlarm(aAlarm)
	self.Alarm = aAlarm
end

function CEnemyManagerLUA:GenerateEnemy()
	local lRoom = "room2"--scene:GetActivateRoom()
	--engine:Trace("Name room:"..lRoom)
	local Node = self.ExtraEnemy[lRoom]
	if not Node == nil then
		self:AddNewEnemy( Node:GetChildren(self.ExtraEnemyCount) )
		self.ExtraEnemyCount = self.ExtraEnemyCount + 1
	end
end

function CEnemyManagerLUA:GetEnemiesAtDistance( aDist )
	lEnemyList = {}
	local PlayerPos = g_Player:GetPosition()
	for i in pairs (self.Enemy) do
		if self.Enemy[i] ~= nil then
			local EnemyPos = self.Enemy[i]:GetPosition()
			local Dist = PlayerPos:Distance(EnemyPos)
			if Dist <= aDist then
				table.insert(lEnemyList, self.Enemy[i])
			end
		end
	end
	return lEnemyList
end

function CEnemyManagerLUA:AlarmRoom(room)	
	self.Room = room
	self.RoomAlarm = true
end

function CEnemyManagerLUA:CreateBoss()
	self.Boss = CBoss()
	self.Boss:SetPosition( Vect3f(-114.0, 31.0, 61.0) )
end