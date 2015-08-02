function get_player_position()
	g_HUD:UpdatePositionPlayer()
end

function get_player_orientation_on_map()
	g_HUD:UpdateYawPlayer()
end

function get_enemy_position(name)
	g_HUD:UpdatePositionEnemy(name)
end

function get_enemy_orientation(name)
	g_HUD:UpdateYawEnemy(name)
end

function AddEnemy(aTexture, aPosInMap3d, aWidth, aHeight, aYaw, aPositionScript, aOrientationScript, aName)
	
	  if g_HUD == nil then
		g_HUD = CHUD()
	  end
      lposInMap = g_HUD:NormalizePlayerPos( aPosInMap3d.x, aPosInMap3d.z )
      PositionScriptComplete 	= ""..aPositionScript.."('"..aName.."')"
	  engine:Trace("El script es: "..PositionScriptComplete)	
      OrientationScriptComplete = ""..aOrientationScript  .."('" .. aName.."')"
      engine:Trace("El script es: "..OrientationScriptComplete)
	  g_HUD:AddEnemy( aName, texture_manager:GetTexture( aTexture ), aPosInMap3d, lposInMap, aWidth, aHeight, aYaw, PositionScriptComplete, OrientationScriptComplete )
	  engine:Trace("He llegado despues de petar")      
end