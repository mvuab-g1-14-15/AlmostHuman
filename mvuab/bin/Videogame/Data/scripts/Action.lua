class 'CActionLUA'

function CActionLUA:__init( Node )
	self.Function = Node:GetPszProperty( "function", "no_function" )
end

function CActionLUA:Execute()
	script_manager:RunCode(self.Function)
end

function CActionLUA:GetLuaFunction()
	return self.Function
end

function CActionLUA:SetLuaFunction( LuaFunction )
	self.Function = LuaFunction
end