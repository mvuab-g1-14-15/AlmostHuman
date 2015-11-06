function shoot()
	sound_manager:PlayEvent( "Shoot", "Logan" )
end

function CargarJuego()
	engine:SetPlay(true)
	lCamera = camera_manager:GetCurrentCamera()
	lCamera:SetEnable( false );
	--g_Player:UpdatePlayer(Vect3f( lCamera:GetPosition() ), lCamera:GetYaw(), lCamera:GetPitch() );
	-- camera_manager:SetCurrentCamera( "Main" );
	-- camera_manager:GetCurrentCamera():SetEnable( true );
	sound_manager:PlayEvent("Play_Long_Ambient", "Ambient" )
	gui_manager:ActiveWindows("HUD.xml")
	gui_manager:SetRenderPointer(false)
	cinematic_manager:Execute("inicial")
	--sound_manager:PlayEvent("Play_Ayer_Tuve_Una", "Logan")
	cinematic_manager:StopCinematic("cinematica_inicial")
	cinematic_manager:SetCinematicActive(true)
	sound_manager:PlayParticlesSFX()
	g_GameIsOn = true;
	----engine:Trace("CargarJuego OK")
	--gui_manager:ShowDialogTex("Test")
end

function FunctionGameOver()
	gui_manager:ActiveWindows("GameOver.xml")
	gui_manager:SetRenderPointer(true)
end

function PuntoControl()
	engine:SetPlay(true)
	sound_manager:PlayEvent("Play_Long_Ambient", "Ambient" )
	gui_manager:ActiveWindows("HUD.xml")
	gui_manager:SetRenderPointer(false)
	sound_manager:PlayParticlesSFX()
end

function IrASetup()
	gui_manager:ActiveWindows("Setup.xml")
end

function IrAMain()
	gui_manager:ActiveWindows("Main.xml")
	sound_manager:PlayEvent("Play_Back","GUI")
end

function MostrarMensaje()
	gui_manager:SetMessageBox("¿Desea salir?")
end

function ReloadGUI()
	g_HUD = CHUD()
	gui_manager:SetRenderPointer(true)
end

function SalirJuego()
	engine:QuitGame()
end

function SoundGUI()
	sound_manager:PlayEvent("Play_Gui_Over","GUI")
end

function End()
	gui_manager:ActiveWindows("Credits.xml")
	engine_manager:SetbGamePause(true)
end
