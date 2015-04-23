function CheckLevel()
	if action_manager:DoAction("ChangeVolumetria") then
		physic_manager:ReleasePhysicActor(physic_manager:GetActor("NameEffect"))
		physic_manager:RemoveUserData(physic_manager:GetUserData("NameEffect"))
		physic_manager:AddMesh("Data/a.ase", "Escenario")
		camera_manager:SetCurrentCamera("TestProcessCam")
	elseif action_manager:DoAction("ChangeEffects") then
		physic_manager:ReleasePhysicActor(physic_manager:GetActor("Escenario"))
		physic_manager:RemoveUserData(physic_manager:GetUserData("Escenario"))
		physic_manager:AddMesh("Data/NameEffect.ase", "NameEffect")
		camera_manager:SetCurrentCamera("FreeCam")
	end
end