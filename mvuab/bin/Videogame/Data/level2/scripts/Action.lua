class 'CActionLUA'

function CActionLUA:__init( Node )
	self.Function = Node:GetAttributeString( "function", "no_function" ).."()"
	
	engine:Trace("Loaded the action " .. self.Function)
end

function CActionLUA:Execute()
	engine:Trace("Estoy ejecutando la función ".. self.Function)
	script_manager:RunCode(self.Function)
end

function CActionLUA:GetLuaFunction()
	return self.Function
end

function CActionLUA:SetLuaFunction( LuaFunction )
	self.Function = LuaFunction
end

function CActionLUA:__tostring()
	return "Action: " .. self.Function
end