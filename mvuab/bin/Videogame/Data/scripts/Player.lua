
class "CPlayer"

function CPlayer:__init() 
    self.m_Life = 100.0
    self.m_RunSpeed = 15.0
    self.m_WalkSpeed = 10.0
    self.m_CrouchSpeed = 2.0
    self.m_RunCrouchSpeed = 5.0
        
    self.m_Height = 1

    self.m_Crouch = false
    self.m_Up = true
    
    -- Camera shake
    self.m_Amplitude = 0.05
    self.m_ElapsedTime = 0.0
    self.m_ModAmplitude = -0.01
    self.m_StartAmplitude = self.m_Amplitude

    -- Setting roooms
    self.rooms = {Vect3f( 0, 2, 1 ), Vect3f( 8, -10, 18 ), Vect3f( 40, -15, -8), Vect3f( 141, 35, -17 ), Vect3f( 104, 22, 198 )}
    self.g_Room = 1
    local position = self.rooms[self.g_Room]

    --Init Manager
    self:InitManagers()

	m_PhysicManager:AddController("Logan", 0.4, self.m_Height, 0.2, 0.5, 0.5, position, CollisionGroup.ECG_PLAYER.value, -10.0)

    --Init Controllers
    self:InitControllers()

	camera_manager:GetCurrentCamera():SetPos(Vect3f(position.x, position.y + (m_CharacterController:GetHeight()*2/3), position.z))
	
	self.m_pRenderableObject = CInstanceMesh("Logan", "Logan");
	
	m_ROM:AddResource("Logan", self.m_pRenderableObject)
	
	l_pos = m_CharacterController:GetPosition()

	l_pos.y = l_pos.y - 1.4
	self.m_pRenderableObject:SetPosition( l_pos );
	
	self.m_pRenderableObject:SetYaw( -m_CharacterController:GetYaw() -1.57 );
	
	self.m_pRenderableObject:SetScale( Vect3f(0.09) );
	
	self.m_pRenderableObject:MakeTransform();

	self.m_MoveDt = 0.0
	self.m_InMove = false
	self.m_MoveTo = Vect3f()
	self.m_LeanOut = false
	self.m_Side = 1
	self.m_LeanOutEnable = true
	
    math.randomseed(os.time())
    core:trace("Character Initialized")
end

function CPlayer:InitManagers()
    Singleton_Core.get_singleton():trace("Init Managers")
    m_PhysicManager = Singleton_Core.get_singleton():GetPhysicsManager()
    m_Process = Singleton_Engine.get_singleton():GetProcess()
    m_ActionManager = Singleton_Core.get_singleton():GetActionManager()
    m_CameraManager = Singleton_Core.get_singleton():GetCameraManager()
	l_ROLM = Singleton_Core.get_singleton():GetRenderableObjectsLayersManager()
	m_ROM = l_ROLM:GetResource("characters")
end

function CPlayer:InitControllers()
    Singleton_Core.get_singleton():trace("Init Controllers")
    m_CharacterController = m_PhysicManager:GetController("Logan")
    m_UserDataCharacterController = m_PhysicManager:GetUserData("Logan")
    m_CharacterController = m_UserDataCharacterController:GetController()
end

function CPlayer:ModifyLife(life)
    self.m_Life = self.m_Life + life
end

function CPlayer:Update()
	local l_Dt = Singleton_Core.get_singleton():GetTimer():GetElapsedTime()

	if self.m_LeanOut == true then
		self:LeanOut(self.m_Side, l_Dt)
		local diferencia = Vect3f(m_CameraManager:GetCurrentCamera():GetPos()) - Vect3f(m_CharacterController:GetPosition())
		if ((diferencia.x < 0.25 and diferencia.x > -0.25) and (diferencia.z < 0.25 and diferencia.z > -0.25)) then
			self.m_LeanOut = false
			self.m_MoveDt = 0.0
			self.m_InMove = false
			self.m_MoveTo = Vect3f()
		end
	else
		if m_ActionManager:DoAction("ASS_LEFT_SETZERO") then
			self.m_LeanOut = true
			self.m_Side = -1
			self.m_MoveDt = 0.0
			self.m_InMove = false
			self.m_MoveTo = Vect3f()
			self.m_LeanOutEnable = true
		elseif m_ActionManager:DoAction("ASS_RIGHT_SETZERO") then
			self.m_LeanOut = true
			self.m_Side = 1
			self.m_MoveDt = 0.0
			self.m_InMove = false
			self.m_MoveTo = Vect3f()
			self.m_LeanOutEnable = true
		else
			local l_Run = false     
			local l_Strafe = 0
			local l_Forward = 0
			
			if m_ActionManager:DoAction("Run") then
				l_Run = true
			end
			
			if m_ActionManager:DoAction("Crouch") then
				if self.m_Crouch == true then
					self.m_Crouch = false
				else
					self.m_Crouch = true
				end
				self:Crouch()
			end

			if m_ActionManager:DoAction("MoveLeft") then
				l_Strafe = 1
				self:Move(l_Run, self.m_Crouch, l_Forward, l_Strafe, l_Dt)
			elseif m_ActionManager:DoAction("MoveRight") then
				l_Strafe = -1
				self:Move(l_Run, self.m_Crouch, l_Forward, l_Strafe, l_Dt)
			elseif m_ActionManager:DoAction("MoveBackward") then
				l_Forward = -1
				self:Move(l_Run, self.m_Crouch, l_Forward, l_Strafe, l_Dt)
			elseif m_ActionManager:DoAction("MoveForward") then
				l_Forward = 1
				self:Move(l_Run, self.m_Crouch, l_Forward, l_Strafe, l_Dt)
			else
				self:Move(l_Run, self.m_Crouch, 0, 0, l_Dt)
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
			
			if action_manager:DoAction("Jump") then
				self:Jump(4)
			end

			if m_ActionManager:DoAction("ChangeRoom") then
				self:CambiarSala()
			end
			
			l_pos = m_CharacterController:GetPosition()
			l_pos.y = l_pos.y - 1.45
			self.m_pRenderableObject:SetPosition( l_pos )
			self.m_pRenderableObject:SetYaw( -m_CharacterController:GetYaw() -1.57 )

			self.m_pRenderableObject:MakeTransform()
			if self.m_LeanOutEnable == true then
				if m_ActionManager:DoAction("ASS_RIGHT") then
					self:LeanOut(-1, l_Dt)
				elseif m_ActionManager:DoAction("ASS_LEFT") then
					self:LeanOut(1, l_Dt)
				end
			end
		end
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
    
    if l_Run == true and self.m_Crouch == true then
        l_AddPos = l_AddPos * self.m_RunCrouchSpeed * l_Dt
    elseif l_Run == true then
        l_AddPos = l_AddPos * self.m_RunSpeed * l_Dt
    elseif self.m_Crouch == true then  
        l_AddPos = l_AddPos * self.m_CrouchSpeed * l_Dt
    else
        l_AddPos = l_AddPos * self.m_WalkSpeed * l_Dt
    end

    m_CharacterController:Move(l_AddPos, l_Dt)
    m_CharacterController:SetYaw(Yaw)

    local l_Position = Vect3f(m_CharacterController:GetPosition())
	l_Position.y = l_Position.y + (m_CharacterController:GetHeight()*2/3)
    
	local l_Yaw = m_CharacterController:GetYaw()
	local l_LookAt = Vect3f(math.cos(l_Yaw) + math.cos(l_Yaw + 3.141618 * 0.5), 0, math.sin(l_Yaw) + math.sin(l_Yaw + 3.141618 * 0.5))
	l_LookAt:Normalize()
    l_Position = self:CameraShake(l_Position, l_Dt)
	l_Camera:SetPos(l_Position + l_LookAt*0.1)
	
	
end

function CPlayer:CameraShake(l_Position, l_Dt)
    self.m_ElapsedTime = self.m_ElapsedTime + l_Dt;
    self.m_Amplitude = self.m_Amplitude + self.m_ModAmplitude * l_Dt;
    
    local l_Camera = Singleton_Core.get_singleton():GetCameraManager():GetCurrentCamera()
    local l_Pitch = l_Camera:GetPitch()
    local l_Yaw = l_Camera:GetYaw()
    
    l_Pitch = math.sin(self.m_ElapsedTime) * self.m_Amplitude
    l_Yaw = math.sin(self.m_ElapsedTime) * self.m_Amplitude
    
    l_Camera:AddPitch(l_Pitch)
    l_Camera:AddYaw(l_Yaw)
 
    --l_Position.x =  l_Position.x + math.sin(self.m_ElapsedTime) * self.m_Amplitude;
    --l_Position.y =  l_Position.y + math.sin(self.m_ElapsedTime) * self.m_Amplitude;
    --l_Position.z =  l_Position.z + math.sin(self.m_ElapsedTime) * self.m_Amplitude;
    
    if(self.m_Amplitude < 0.0 or self.m_Amplitude > self.m_StartAmplitude) then
        self.m_ModAmplitude = -1 * self.m_ModAmplitude
    end
    
    return l_Position
end

function CPlayer:CambiarSala()
	self.g_Room = self.g_Room + 1
	if(self.g_Room > 5) then
		self.g_Room = 1
	end
	local position = self.rooms[self.g_Room]
	m_CharacterController:SetPosition(position)
	camera_manager:GetCurrentCamera():SetPos(Vect3f(position.x, position.y + (m_CharacterController:GetHeight()/2), position.z))
end

function CPlayer:Jump(amount)
    --Singleton_Core.get_singleton():trace("Jump")
    m_CharacterController:Jump(amount, m_CharacterController:GetPosition().y)
end

function CPlayer:Crouch()
    --Singleton_Core.get_singleton():trace("Crouch" .. (self.m_Crouch and 1 or 0))
    if self.m_Crouch == true then
        m_CharacterController:UpdateCharacterExtents(false, self.m_Height/2)
    elseif self.m_Crouch == false then
        m_CharacterController:UpdateCharacterExtents(true, self.m_Height/2)
    end

end

function CPlayer:LeanOut(direction, l_Dt)
	current_camera = m_CameraManager:GetCurrentCamera()
	local l_Pos = current_camera:GetPos()
	
	local l_Dir     = current_camera:GetDirection()
	local l_Up      = current_camera:GetVecUp()
			
	l_BallPos = l_Dir ^ l_Up
	l_BallPos:Normalize()
	l_BallPos = l_BallPos/10
	l_BallPos = direction * l_BallPos + l_Pos - current_camera:GetDirection()/5

	local l_PUD = ListaPUD(m_PhysicManager:OverlapSphere(1, l_BallPos))
	
	if l_PUD:size() == 0 then 
	
		
		if self.m_InMove == false then
			self.m_MoveTo = l_Dir ^ l_Up
			self.m_MoveTo = l_Pos + direction * self.m_MoveTo
			self.m_InMove = true
		end
		
		if self.m_InMove then
			if self.m_MoveDt >= 1.0 then
				self.m_MoveDt = 1.0
			end
			local l_PosRO = l_Pos + (self.m_MoveTo - l_Pos) * (self.m_MoveDt*0.8)
			l_Pos = l_Pos + (self.m_MoveTo - l_Pos) * self.m_MoveDt
			self.m_MoveDt = self.m_MoveDt + l_Dt
			
			current_camera:SetPos(l_Pos)
			
			l_PosRO.y = l_PosRO.y - 1.45 - (m_CharacterController:GetHeight()*2/3)
			self.m_pRenderableObject:SetPosition( l_PosRO )
			self.m_pRenderableObject:MakeTransform()
			
		end
	elseif (self.m_MoveDt > 0) == true then
		Singleton_Core.get_singleton():trace("he entrado en tu ojete")
		--self.m_InMove = false
		self.m_MoveDt = self.m_MoveDt - l_Dt
		--self.m_MoveTo = Vect3f()
		if self.m_InMove == false then

				self.m_MoveTo = l_Dir ^ l_Up
				self.m_MoveTo = l_Pos + 1.5*direction * self.m_MoveTo * (-1)
				self.m_InMove = true
		end
			
		if self.m_InMove then
				if self.m_MoveDt >= 1.0 then
					self.m_MoveDt = 1.0
				end
				local l_PosRO = l_Pos + (self.m_MoveTo - l_Pos) * (self.m_MoveDt*0.8)
				l_Pos = l_Pos + (self.m_MoveTo - l_Pos) * self.m_MoveDt
				self.m_MoveDt = self.m_MoveDt - l_Dt
				
				current_camera:SetPos(l_Pos)
				
				l_PosRO.y = l_PosRO.y - 1.45 - (m_CharacterController:GetHeight()*2/3)
				self.m_pRenderableObject:SetPosition( l_PosRO )
				self.m_pRenderableObject:MakeTransform()
				
		end
		self.m_LeanOutEnable = false
	end
	
end