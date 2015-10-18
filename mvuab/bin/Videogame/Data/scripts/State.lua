dofile("./data/scripts/Action.lua")

class 'CStateLUA'

function CStateLUA:__init( name )
	self.Name = name
	
	self.OnEnterActions = {}
	self.OnExitActions = {}
	self.UpdateActions = {}
end

function CStateLUA:Load( Node )
	local count = Node:GetNumChildren()
	
	for i = 0, count-1 do
		CurrentNode = Node:GetChildren(i)
		TagName = CurrentNode:GetName()
		
		if TagName == "on_enter" then
			for j = 0, CurrentNode:GetNumChildren()-1 do
				CurrentSubNode = CurrentNode:GetChildren(j)
				ActualTagName = CurrentSubNode:GetName()
        
				if ActualTagName == "action" then
					Action = CActionLUA(CurrentSubNode)
					table.insert(self.OnEnterActions, Action)
				end
			end
		elseif TagName == "on_exit" then
			for j = 0, CurrentNode:GetNumChildren()-1 do
				CurrentSubNode = CurrentNode:GetChildren(j)
				ActualTagName = CurrentSubNode:GetName()
        
				if ActualTagName == "action" then
					Action = CActionLUA(CurrentSubNode)
					table.insert(self.OnExitActions, Action)					
				end
			end
		elseif TagName == "update" then
			for j = 0, CurrentNode:GetNumChildren()-1 do
				CurrentSubNode = CurrentNode:GetChildren(j)
				ActualTagName = CurrentSubNode:GetName()
        
				if ActualTagName == "action" then
					Action = CActionLUA(CurrentSubNode)
					table.insert(self.UpdateActions, Action)
				end
			end
		end
	end
	
	return true
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
	return self.UpdateActions
end

function CStateLUA:SetUpdateActions( Actions )
	self.UpdateActions = Actions
end

function CStateLUA:__tostring()
	on_enter = "On enter actions: \n"
	for i = 1, #self.OnEnterActions do
		on_enter = on_enter .. "\t" .. tostring(self.OnEnterActions[i]) .. "\n"
	end
	on_exit = "On exit actions: \n"
	for i = 1, #self.OnExitActions do
		on_exit = on_exit .. "\t" .. tostring(self.OnExitActions[i]) .. "\n"
	end
	on_update = "On enter actions: \n"
	for i = 1, #self.UpdateActions do
		on_update = on_update .. "\t" .. tostring(self.UpdateActions[i]) .. "\n"
	end
	return on_enter .. on_exit .. on_update
end