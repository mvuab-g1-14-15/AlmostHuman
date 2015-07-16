class 'CEnemyManagerLUA'

function CEnemyManagerLUA:__Init(name)
	self.StatesMachine = {}
	self.CoreEnemy = {}
	self.Enemy = {}
	self.Routes = {}
	self:Load(name)
end

function CEnemyManagerLUA:Load(filename)
	
	l_File = CXMLTreeNode()
	Node = CXMLTreeNode()
    if not l_File:LoadFile( filename.c_str(), "enemies", Node ) then
        engine:Trace( "File '"..filename.."' not correctly loaded" )
    end

    if Node:Exists() then
        count = Node:GetNumChildren();

        for i = 0, count do
			CurrentNode = Node:GetChildren(i)
            l_TagName = CurrentNode.GetName();

            if ( l_TagName == "enemy" ) then
                self:AddNewEnemy( CurrentNode )
            end
			if ( l_TagName == "core_enemy" ) then           
                self:AddNewCoreEnemy( CurrentNode )                          
            end
			if ( l_TagName == "route" ) then           
                self:AddNewRoute( CurrentNode )
            end

        end
    end
end

function CEnemyManagerLUA:Update()

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
    count = Node:GetNumChildren();
    l_Route = {};

    for i = 0, count do
		CurrentNode = Node:GetChildren(i)
        l_Point = CurrentNode.GetAttributeVect3f( "value", Vect3f( 0.0, -99999999.0, 0.0 ) )

        if ( l_Point == Vect3f( 0.0, -99999999.0, 0.0 ) ) then        
            engine:Trace( "Point in the route '".. l_Id .."' not correctly loaded.")        
        else        
            table.insert(l_Route, l_Point)
        end
    end

    self.Routes[l_Id] = l_Route;
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
	
    if lType == "patrol" then
		lEnemy = CPatrolEnemyLUA(Node, self.Routes[Node:GetAttributeInt("route", -1)], lStateMachine, lCoreEnemy)
	end
	
	self.Enemy[Node:GetAttributeString("name", "no_name")] = lEnemy
end