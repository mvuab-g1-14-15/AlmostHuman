class 'CEnemyManagerLUA'

function CEnemyManagerLUA:__Init(name)
	self.StatesMachine = {}
	self.CoreEnemy = {}
	self.Enemy = {}
	self.Routes = {}
	self:Load(name)
end

function CEnemyManagerLUA:Load(filename)