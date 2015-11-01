--includes
fileIn "XMLStruct.ms"
filein "Utils.ms"

fn ExportLights filenameXML aLights aRoom =
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
					
					--add the common attributes of the light
					local l_LightTypeStr = iLight.type as string
					l_NewLightNode.AddAttributeAsString "name" l_LightName
					l_NewLightNode.AddAttributeAsString "room" aRoom
					l_NewLightNode.AddAttributeFloat "intensity" iLight.multiplier
					l_NewLightNode.AddAttributeFloat "specular_intensity" iLight.contrast
					l_NewLightNode.AddAttributeCoordinate "pos" iLight.pos
					l_NewLightNode.AddAttributeRGB "color" iLight.rgb
					l_NewLightNode.AddAttributeFloat "att_start_range" iLight.farAttenStart
					l_NewLightNode.AddAttributeFloat "att_end_range" iLight.farAttenEnd
					
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
						
						local lShadowNode = xDoc.getNewNode "shadow_map"
						l_NewLightNode.AppendNode lShadowNode
						
						lShadowNode.AddAttributeAsString "render_shadows" "true"
						lShadowNode.AddAttributeAsString "shadow_texture_mask" ""
						lShadowNode.AddAttributeAsString "room" aRoom
						
						--STATIC SHADOW MAP
						local lStaticShadowMap = xDoc.getNewNode "static_shadow_map"
						lShadowNode.AppendNode lStaticShadowMap
						lStaticShadowMap.AddAttributeAsString "generate" "true"
						lStaticShadowMap.AddAttributeAsString "layer" "static"
						lStaticShadowMap.AddAttributeAsString "renderable_objects_manager" "solid"
						lStaticShadowMap.AddAttributeAsString "format_type" "R32F"
						lStaticShadowMap.AddAttributeAsString "size" "1024 1024"
						lStaticShadowMap.AddAttributeAsString "stage" "5"
						
						--DYNAMIC SHADOW MAP
						local lDynamicShadowMap = xDoc.getNewNode "dynamic_shadow_map"
						lShadowNode.AppendNode lDynamicShadowMap
						lDynamicShadowMap.AddAttributeAsString "generate" "true"
						lDynamicShadowMap.AddAttributeAsString "layer" "dynamic"
						lDynamicShadowMap.AddAttributeAsString "renderable_objects_manager" "solid"
						lDynamicShadowMap.AddAttributeAsString "format_type" "R32F"
						lDynamicShadowMap.AddAttributeAsString "size" "1024 1024"
						lDynamicShadowMap.AddAttributeAsString "stage" "6"
					)
					
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