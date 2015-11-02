class 'CStateMachineLUA'

function CStateMachineLUA:__init( Name )
	self.States = {}
	self:Load( Name )
	
end

function CStateMachineLUA:Load( FileName )
	self.FileName = FileName
	
	----engine:Trace("Init the loading of the state machine " .. FileName)

	File = CXMLTreeNode()
	TreeNode = CXMLTreeNode()
	if not File:LoadAndFindNode( FileName, "state_machine", TreeNode) then
		--TODO: LOG ERROR
		----engine:Trace("Error loading XML in CStateMachineLUA")
	end
	
	if TreeNode:Exists() then
		count = TreeNode:GetNumChildren()
		
		----engine:Trace("State machine number of states is " .. count)
		
		for i = 0, count-1 do
			CurrentNode = TreeNode:GetChildren(i)
			TagName = CurrentNode:GetName()
			
			if TagName == "state" then
				Name = CurrentNode:GetAttributeString("name", "no_name")
				State = CStateLUA(Name)
				
				if not State:Load(CurrentNode) then
					--TODO: LOG ERROR
					----engine:Trace("Error loading the state " .. tostring(State))
				end
				----engine:Trace("Current state " .. Name .. " loaded:\n" .. tostring(State))
				self.States[Name] = State
			end
		end
	end
end

function CStateMachineLUA:ReLoad()
	self:CleanUp()
	self:Load(self.FileName)
end

function CStateMachineLUA:CleanUp()
	for k in pairs (self.States) do
		self.States[k] = nil
	end
end

function CStateMachineLUA:GetState(State)
	return self.States[State]
end