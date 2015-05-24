dofile("./data/level2/scripts/Action.lua")

class 'CStateLUA'

function CStateLUA:__init( name )
	self.Name = name
	
	self.OnEnterActions = {}
	self.OnExitActions = {}
	self.UpdateActions = {}
end

function Load( Node )
	local count = Node:GetNumChildren()
	
	for i = 0, count do
		CurrentNode = Node:GetChildren(i)
		TagName = CurrentNode:GetName()
		
		if TagName == "on_enter" then
			for j = 0, CurrentNode:GetNumChildren() do
				CurrentSubNode = CurrentNode:GetChildren(j)
				ActualTagName = CurrentSubNode:GetName()
        
				if ActualTagName == "action" then
					Action = CActionLUA(CurrentSubNode)
					table.insert(self.OnEnterActions, Action)
				end
			end
		elseif TagName == "on_exit" then
			for j = 0, CurrentNode:GetNumChildren() do
				CurrentSubNode = CurrentNode:GetChildren(j)
				ActualTagName = CurrentSubNode:GetName()
        
				if ActualTagName == "action" then
					Action = CActionLUA(CurrentSubNode)
					table.insert(self.OnExitActions, Action)
				end
			end
		elseif TagName == "update" then
			for j = 0, CurrentNode:GetNumChildren() do
				CurrentSubNode = CurrentNode:GetChildren(j)
				ActualTagName = CurrentSubNode:GetName()
        
				if ActualTagName == "action" then
					Action = CActionLUA(CurrentSubNode)
					table.insert(self.UpdateActions, Action)
				end
			end
		end
	end
end

function CStateLUA:GetOnEnterActions()
	return self.OnEnterActions
end

function CStateLUA:SetOnEnterActions( Actions )
	self.OnEnterActions = Actions
end

function CStateLUA:GetOnExitActions()
	return self.OnExitActions
end

function CStateLUA:SetOnExitActions( Actions )
	self.OnExitActions = Actions
end

function CStateLUA:GetUpdateActions()
	return self.OnEnterActions
end

function CStateLUA:SetUpdateActions( Actions )
	self.UpdateActions = Actions
end