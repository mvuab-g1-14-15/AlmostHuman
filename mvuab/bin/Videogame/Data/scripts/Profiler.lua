class 'CProfiler'

function CProfiler:__init()
	self.Times = {}
	self.File = io.open ("profile.prfl", "a")
	self.File:write("<<< New execution starts >>>\n")
end

function CProfiler:AddEntry( aName )
	self.Times[aName] = {}
	self.Times[aName].Init = 0.0
	self.Times[aName].End = 0.0
	self.Times[aName].Time = 0.0
end

function CProfiler:AddInit( aName )
	self.Times[aName].Init = os.clock()
end

function CProfiler:AddEnd( aName )
	self.Times[aName].End = os.clock()
	self.Times[aName].Time = self.Times[aName].Time + self.Times[aName].End - self.Times[aName].Init
end

function CProfiler:Tick()
	for _, lTime in pairs(self.Times) do
		lTime.Time = 0.0
	end
end

function CProfiler:Trace()
	self.File:write( "----------\n" )
	for lName, lTime in pairs(self.Times) do
		self.File:write( lName..": "..lTime.Time.."\n" )
	end
	self.File:write( "----------\n" )
end