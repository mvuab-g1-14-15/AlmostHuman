function shoot()
	sound_manager:PlayEvent( "Shoot", "TestGameObject2d" )
end

function othersound()

sound_manager:PlayEvent("Shoot","TestGameObject3d")
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