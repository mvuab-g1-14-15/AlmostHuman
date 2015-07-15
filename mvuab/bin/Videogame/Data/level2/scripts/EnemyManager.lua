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
        int count = Node:GetNumChildren();

        for i = 0, count do
			CurrentNode = Node:GetChildren(i)
            l_TagName = CurrentNode.GetName();

            if ( l_TagName == "enemy" ) then
                self:AddNewEnemy( CurrentNode )
            else if ( l_TagName == "core_enemy" ) then           
                self:AddNewCoreEnemy( CurrentNode )                          
            else if ( l_TagName == "route" ) then           
                self:AddNewRoute( CurrentNode )
            end

        end
    end
end

function CEnemyManagerLUA:Update()

end

function CEnemyManagerLUA:AddNewCoreEnemy( Node )

end

function CEnemyManagerLUA:AddNewEnemy( Node )

end

function CEnemyManagerLUA:AddNewRoute( Node )

end