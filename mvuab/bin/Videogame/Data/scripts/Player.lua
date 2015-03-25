
class "CPlayer"

function CPlayer:__init()
    self.m_Life = 100.0
    
    self.m_RunSpeed = 15.0
    self.m_WalkSpeed = 10.0
    self.m_CrouchSpeed = 4.0

    local l_Process = Singleton_Engine.get_singleton():GetProcess()
    local l_PhysicManager = Singleton_Core.get_singleton():GetPhysicsManager()
	
	self.rooms = {Vect3f( 0, 2, 1 ), Vect3f( 8, -10, 18 ), Vect3f( 40, -15, -8), Vect3f( 141, 35, -17 ), Vect3f( 104, 22, 198 )}
	self.g_Room = 1
	local position = self.rooms[self.g_Room]
	
	l_PhysicManager:AddController("CharacterController", 0.4, 2, 0.2, 0.5, 0.5, position, CollisionGroup.ECG_PLAYER.value, -10.0)

    self.m_CharacterController = l_PhysicManager:GetController("CharacterController")
	camera_manager:GetCurrentCamera():SetPos(Vect3f(position.x, position.y + (self.m_CharacterController:GetHeight()*2/3), position.z))
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
        self:Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
	elseif l_ActionManager:DoAction("MoveRight") then
        l_Strafe = -1
        self:Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
    end
    
	if l_ActionManager:DoAction("MoveBackward") then
        l_Forward = -1
        self:Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
	elseif l_ActionManager:DoAction("MoveForward") then
        l_Forward = 1
        self:Move(l_Run, l_Crouch, l_Forward, l_Strafe, l_Dt)
    end
	
	local l_ActionManagerLuaWrapper=CActionManagerLuaWrapper()
	local value=""
	local current_camera = camera_manager:GetCurrentCamera();
	if l_ActionManagerLuaWrapper:DoAction(action_manager, "MoveYaw") then
		current_camera:AddYaw( -l_ActionManagerLuaWrapper.amount * l_Dt * 100.0 );
	end
	local current_camera = camera_manager:GetCurrentCamera();
	if l_ActionManagerLuaWrapper:DoAction(action_manager, "MovePitch") then
		current_camera:AddPitch( -l_ActionManagerLuaWrapper.amount * l_Dt * 100.0 );
	end
	
	if l_ActionManager:DoAction("ChangeRoom") then
		self:CambiarSala()
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
	
    
    if l_Run == true then
        l_AddPos = l_AddPos * self.m_RunSpeed * l_Dt
    elseif l_Crouch == true then  
        l_AddPos = l_AddPos * self.m_CrouchSpeed * l_Dt
    else
        l_AddPos = l_AddPos * self.m_WalkSpeed * l_Dt
    end
	
    self.m_CharacterController:Move(l_AddPos, l_Dt)
    self.m_CharacterController:SetYaw(Yaw)
    
    local l_position = Vect3f(self.m_CharacterController:GetPosition())
	l_position.y = l_position.y + (self.m_CharacterController:GetHeight()*2/3)
	l_Camera:SetPos(l_position)
end

function CPlayer:CambiarSala()
	self.g_Room = self.g_Room + 1
	if(self.g_Room > 5) then
		self.g_Room = 1
	end
	local position = self.rooms[self.g_Room]
	self.m_CharacterController:SetPosition(position)
	camera_manager:GetCurrentCamera():SetPos(Vect3f(position.x, position.y + (self.m_CharacterController:GetHeight()/2), position.z))
end
