dofile("./data/level2/scripts/StateMachine.lua")
dofile("./data/level2/scripts/CoreEnemy.lua")
dofile("./data/level2/scripts/shoot.lua")

class 'CEnemyManagerLUA'

function CEnemyManagerLUA:__init()
	--Mapas
	self.StatesMachine = {}
	self.CoreEnemy = {}
	self.Enemy = {}
	self.Routes = {}
	self.ActualEnemy = nil
	self.AStar = CAStar()
	self.Shoots = {}
	self:Load("Data/enemies/enemies.xml")
	
end

function CEnemyManagerLUA:Load(filename)
	
	l_File = CXMLTreeNode()
	Node = CXMLTreeNode()
    if not l_File:LoadAndFindNode( filename, "enemies", Node ) then
        engine:Trace( "File '"..filename.."' not correctly loaded" )
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
            end

        end
    end
	
	self.AStar:Init()
end

function CEnemyManagerLUA:Update()
	for i in pairs (self.Enemy) do
		self.ActualEnemy = self.Enemy[i]
		if self.ActualEnemy:GetLife() > 0 then
			self.ActualEnemy:Update()
		else
			AnimatedModel = self.ActualEnemy:GetAnimationModel()
			AnimatedModel:ChangeAnimationAction( "morir", 0.2, 0.2 )
			if AnimatedModel:IsActionAnimationActive( "morir" ) then
				self.ActualEnemy:Destroy()
				table.remove(self.Enemy, i)
				collectgarbage()
			end
		end
	end
	
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
		engine:Trace("Nodo ".. i)
        if ( l_Point == Vect3f( 0.0, -99999999.0, 0.0 ) ) then        
            engine:Trace( "Point in the route '".. l_Id .."' not correctly loaded.")        
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
        engine:Trace("Core '".. lType .."' not found")
    end

    lStateMachine = self.StatesMachine[self.CoreEnemy[lType]:GetStateMachineName()]

    if ( lStateMachine == nil) then
        engine:Trace("State machine for '".. lType .."' not found")
    end
	
	lEnemy = nil
	
    if lType == "patroll" then
		engine:Trace("He entrado en el tipo: "..lType)
		lEnemy = CPatrolEnemyLUA(Node, self.Routes[Node:GetAttributeInt("route", -1)], lStateMachine, lCoreEnemy)
	end
	name = Node:GetAttributeString("name", "no_name")
	engine:Trace("Enemy: "..name)
	self.Enemy[name] = lEnemy
	self.ActualEnemy = lEnemy
end

function CEnemyManagerLUA:GetActualEnemy()
	return self.ActualEnemy
end

function CEnemyManagerLUA:GetAStar()
	return self.AStar
end

function CEnemyManagerLUA:GetResource(name)
	return self.Enemy[name]
end

function CEnemyManagerLUA:AddDamage(name)
	lEnemy = self.Enemy[name]
	lEnemy:AddDamage(25.0)
end

function CEnemyManagerLUA:GetCloseEnemy(aPos)
	lDist = 999999.99
	lActualDist = 0.0
	lEnemy = 0
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

function CEnemyManagerLUA:AddShoot(aShoot)
	table.insert(self.Shoots, aShoot)
end

function CEnemyManagerLUA:GetEnemys()
	return self.Enemy
end