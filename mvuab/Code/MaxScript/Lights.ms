--includes
fileIn "XMLStruct.ms"
filein "Utils.ms"

fn ExportLights filenameXML aLights =
(
	clearListener()
	
	try
	(
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "lights"
		xDoc.AppendNode l_RootNode
		
		--ambient light
		local lAmbientLightNode = xDoc.getNewNode "ambient_light"
			
		-- append the new node light to the root node lights
		l_RootNode.AppendNode lAmbientLightNode
		
		local lAmbientColor = ambientColor
		lAmbientLightNode.AddAttributeRGBNormalized "color" lAmbientColor
		
		-- iterate for all the lights and exported the allowed types into the xml file
		local l_LightsCount = 0;
		for iLight in aLights do
		(
			try
			(
				l_IsLightExportable = IsLightExportable iLight
			 format "Light name % and IsExportable: % \n " iLight.name l_IsLightExportable
			if superclassof iLight == light then
			(	
				if classof iLight != Targetobject and classof iLight != freeDirect and classof iLight != freeSpot AND l_IsLightExportable == true then
				(
					local l_LightName = iLight.name
					--create the light node
					
					local l_NewLightNode;
					
					--add the specific data for each light
					if classof iLight == TargetDirectionallight then
					(
						l_NewLightNode = xDoc.getNewNode "directional_light"
						--format "direct\n"
					)
					else if classof iLight == targetSpot then
					(
						l_NewLightNode = xDoc.getNewNode "spot_light"
						--format "spot\n"
					)
					else
					(
						l_NewLightNode = xDoc.getNewNode "omni_light"
						--format "omni\n"
					)
					
					-- append the new node light to the root node lights
					l_RootNode.AppendNode l_NewLightNode
					
					/*
					<light name="mylight2" type="spot" pos="169 18 14" dir="0.39344111 0.0096235331 0.91929936" color="1.0 1.0 1.0" att_start_range="20" att_end_range="40" generate_shadow_map="true" 
					shadow_map_format_type="R32F" shadow_map_width="256" shadow_map_height="256" generate_static_shadow_map="true" static_shadow_map_format_type="R32F" static_shadow_map_width="256"
					static_shadow_map_height="256" angle="1.05" fall_off="1.05" shadow_texture_mask="./data/textures/shadow_mask.tga">
							<static renderable_objects_manager="solid"/>
							<dynamic renderable_objects_manager="solid"/>
					</light>
					*/
					
					--add the common attributes of the light
					local l_LightTypeStr = iLight.type as string
					l_NewLightNode.AddAttributeAsString "name" l_LightName
					l_NewLightNode.AddAttributeCoordinate "pos" iLight.pos
					l_NewLightNode.AddAttributeRGB "color" iLight.rgb
					l_NewLightNode.AddAttributeFloat "att_start_range" iLight.farAttenStart
					l_NewLightNode.AddAttributeFloat "att_end_range" iLight.farAttenEnd
					
					/*
					l_NewLightNode.AddAttributeFloat "generate_shadow_map" "true"
					l_NewLightNode.AddAttributeFloat "shadow_map_format_type" "R32F"
					l_NewLightNode.AddAttributeFloat "shadow_map_width" iLight.shadowgenerator.mapsize
					l_NewLightNode.AddAttributeFloat "shadow_map_height" iLight.shadowgenerator.mapsize
					l_NewLightNode.AddAttributeFloat "generate_static_shadow_map" "false"
					l_NewLightNode.AddAttributeFloat "static_shadow_map_format_type" "R32F"
					l_NewLightNode.AddAttributeFloat "static_shadow_map_width" iLight.shadowgenerator.mapsize
					l_NewLightNode.AddAttributeFloat "static_shadow_map_height" iLight.shadowgenerator.mapsize
					l_NewLightNode.AddAttributeFloat "shadow_texture_mask" iLight.shadowProjectorMap.filename
					*/
					
					--add the specific data for each light
					if classof iLight == TargetDirectionallight then
					(
						local lTarget = iLight.target.pos
						
						l_NewLightNode.AddAttributeCoordinate "look_at" lTarget
					)
					else if classof iLight == targetSpot then
					(
						local lTarget = iLight.target.pos
						
						l_NewLightNode.AddAttributeCoordinate "look_at" lTarget
						l_NewLightNode.AddAttributeFloat "angle" iLight.hotspot
						l_NewLightNode.AddAttributeFloat "fall_off" iLight.Falloff
						l_NewLightNode.AddAttributeBool "render_shadows" false
					)
					
					--<shadow_map generate="true" layer ="static" renderable_objects_manager="solid" format_type="R32F" size="1024 1024"/>
					
					l_LightsCount = l_LightsCount + 1
				)
			)
			)
			catch
			(
				format "Error Light % cannot be exported\n" l_LightName
			)
		)
		
		format "Exported in file: '%'\n - Ambient Color '%' and '%' Lights.\n" filenameXML lAmbientColor l_LightsCount
		return xDoc.PrettySaveXML()
	)
	catch
	(
		format "The File % has not been generated due to an unexpected error\n" filenameXML
		format "Check the path of the file\n"
		return false
	)
)