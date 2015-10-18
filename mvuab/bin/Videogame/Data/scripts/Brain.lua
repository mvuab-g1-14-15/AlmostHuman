class 'CBrain'

function CBrain:__init(initisl, state_machine)
	self.ActualState = initisl
	self.StateMachine = state_machine
	self.OnExit = false
	self.OnEnter = true
	self.NextState = initisl
end

function CBrain:Update()
	if not (self.ActualState == self.NextState) then
		self.OnExit = true
	end
	
	State = self.StateMachine:GetState(self.ActualState)
	Actions = nil
	
    if self.OnEnter == true then    
        Actions = State:GetOnEnterActions()
        self.OnEnter = false
    elseif self.OnExit == true then
        Actions = State:GetOnExitActions()
        self.OnEnter = true
        self.OnExit = false
        self.ActualState = self.NextState
    else
        Actions = State:GetUpdateActions()
	end

	count = #Actions
    for i = 1, count do
        Actions[i]:Execute()
	end
end

function CBrain:GetActualState()
	return self.ActualState
end

function CBrain:SetNextState(NextState)
	self.NextState = NextState
end