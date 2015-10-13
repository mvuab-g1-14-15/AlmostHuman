function shoot()
	sound_manager:PlayEvent( "Shoot", "Logan" )
end

function CargarJuego()
engine:SetPlay(true)
sound_manager:PlayEvent("Play_Long_Ambient", "Ambient" )
gui_manager:ActiveWindows("HUD.xml")
gui_manager:SetRenderPointer(false)
cinematic_manager:StopCinematic("cinematica_inicial")
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