class 'CBrain'

function CBrain:__init(initisl, state_machine)
	self.ActualState = initisl
	self.StateMachine = state_machine_manager:GetStateMachine(state_machine)
end