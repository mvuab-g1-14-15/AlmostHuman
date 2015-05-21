class 'CStateMachineLUA'

function CStateMachineLUA:__init( Name )
	Load( Name )
	
	self.States = {}
end

function CStateMachineLUA:Load( FileName )
	self.FileName = FileName

	File = CXMLTreeNode()
	if not File:LoadFile( FileName ) then
		--TODO: LOG ERROR
	end
	
	TreeNode = File:GetNode("state_machine")
	
	if TreeNode:Exists() then
		count = TreeNode:GetNumChildren()
		
		for i = 0, count do
			CurrentNode = TreeNode:GetChildren(i)
			TagName = CurrentNode:GetName()
			
			if TagName == "state" then
				Name = CurrentNode:GetPszProperty("name", "no_name")
				State = CStateLUA(Name)
				
				if not State:Load(CurrentNode) then
					--TODO: LOG ERROR
				end
				
				self.State[Name] = State
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