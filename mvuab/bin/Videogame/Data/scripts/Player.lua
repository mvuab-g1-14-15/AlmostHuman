
class 'CPlayer'

function CPlayer:__init()
    self.m_Life = 100.0
    
    self.m_RunSpeed = 15.0
    self.m_WalkSpeed = 10.0
    self.m_CrouchSpeed = 4.0

    local l_Process = Singleton_Engine.get_singleton():GetProcess()
    local l_PhysicManager = Singleton_Core.get_singleton():GetPhysicsManager()
	l_PhysicManager:AddController("CharacterController", 0.4, 2, 0.2, 0.5, 0.5, Vect3f(0, 2, 1), CollisionGroup.ECG_PLAYER.value, -10.0)

    self.m_CharacterController = l_PhysicManager:GetUserData("CharacterController"):GetController()
    Singleton_Core.get_singleton():trace("Hello CPlayer INIT")
end

function CPlayer:ModifyLife(life)
    self.m_Life = self.m_Life + life
end

function CPlayer:Update()
	local l_ActionManager = Singleton_Core.get_singleton():GetActionManager()
    local l_Dt = Singleton_Core.get_singleton():GetTimer():GetElapsedTime()
    
    local l_Run = false
    local l_Crouch = false
    
    local l_Strafe = 0
    local l_Forward = 0
    
    
    if l_ActionManager:DoAction("Run") then
		l_Run = true
	end
    
    if l_ActionManager:DoAction("Crouch") then
		l_Crouch = true
	end
    
    if l_ActionManager:DoAction("MoveLeft") then
        l_Strafe = 1
        CPlayer.Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
	elseif l_ActionManager:DoAction("MoveRight") then
        l_Strafe = -1
        CPlayer.Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
    end
    
	if l_ActionManager:DoAction("MoveBackward") then
        l_Forward = 1
        CPlayer.Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
	elseif l_ActionManager:DoAction("MoveForward") then
        l_Forward = -1
        CPlayer.Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
    end
end

function CPlayer:Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
    --local l_Camera = Singleton_Core.get_singleton():GetCameraManager():GetCamera("<!-- Poner_Camara_Player -->")
    local l_Camera = Singleton_Core.get_singleton():GetCameraManager():GetCurrentCamera()
    local Yaw = l_Camera:GetYaw()
    
    local l_AddPos = Vect3f(0, 0, 0)    
	l_AddPos.x =  l_Forward * math.cos(Yaw) + l_Strafe * math.cos(Yaw + 3.141618 * 0.5)
	l_AddPos.z =  l_Forward * math.sin(Yaw) + l_Strafe * math.sin(Yaw + 3.141618 * 0.5)
    
	if (not l_AddPos.x == 0 or not l_AddPos.z == 0) then l_AddPos:Normalize() end
    --l_AddPos = if l_Run then self.m_RunSpeed elseif l_Crouch then self.m_CrouchSpeed else self.m_WalkSpeed end

    l_AddPos = l_AddPos * self.m_WalkSpeed
    --if l_Run then
    --    l_AddPos = l_AddPos * self.m_RunSpeed
    --elseif l_Crouch then  
    --    l_AddPos = l_AddPos * self.m_CrouchSpeed
    --else
    --    l_AddPos = l_AddPos * self.m_WalkSpeed
    --end

    self.m_CharacterController:Move(l_AddPos, l_dt)
    self.m_CharacterController:SetYaw(Yaw)
    
    local l_position = Vect3f(self.m_CharacterController:GetPosition())
	l_position.y = l_position.y + (self.m_CharacterController:GetHeight() / 2)
	l_Camera:SetPos(l_position)
end
