function shoot()
	sound_manager:PlayEvent( "Shoot", "TestGameObject2d" )
end

function CargarJuego()
engine:SetPlay(true)
engine:SetRunnigProcess("Start Game")
end

function CargarJuegoFinish()
gui_manager:ActiveWindows("HUD.xml")
gui_manager:SetRenderPointer(false)
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