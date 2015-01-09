fileIn "XMLStruct.ms"
fn ExportLights filenameXML =
(
	--clearListener()
	
	try
	(
		format "Exporting lights in=>%\n" filenameXML
		-- Create the document and set the filename
		xDoc = XMLDocument()
		xDoc.file = 	filenameXML
		xDoc.loaded = true
		
		--create the base node
		l_RootNode = xDoc.getNewNode "lights"
		xDoc.AppendNode l_RootNode
		l_RootNode = xDoc.getRootNode()
		xDoc.SaveXML()
		
		count = 0;
		for iLight in lights do
		(
			try
			(
				-- We need to check if the class of the object is a target, because the lights
				-- of max script also contain their targets
				if classof iLight != Targetobject then
				(
					l_LightName = iLight.name
					--create the light node
					l_NewLightNode = xDoc.getNewNode "light"
			
					-- append the new node light to the root node lights
					l_RootNode.AppendNode l_NewLightNode
			
					--add the common attributes of the light
					l_NewLightNode.AddAttributeAsString "name" l_LightName
					l_NewLightNode.AddAttributeCoordinate "pos" iLight.pos
					l_NewLightNode.AddAttributeRGB "color" iLight.rgb
					l_NewLightNode.AddAttributeFloat "att_start_range" iLight.farAttenStart
					l_NewLightNode.AddAttributeFloat "att_end_range" iLight.farAttenEnd
					
					--add the specific data for each light
					if classof iLight == Omnilight then
					(
						-- Nothing to export now for omni lights
					)
					else if classof iLight == TargetDirectionallight then
					(
						l_Direction = iLight.target.pos-iLight.pos
						l_NewLightNode.AddAttributeCoordinate "dir" l_Direction
						l_NewLightNode.AddAttributeFloat "angle" iLight.hotspot
						l_NewLightNode.AddAttributeFloat "fall_off" iLight.Falloff
					)
					else if classof iLight == targetSpot then
					(
						l_NewLightNode.AddAttributeBool "render_shadows" false
					)
					count = count + 1
				)
			)
			catch
			(
				format "Error Light % would not be exported\n" l_LightName
			)
		)
		format "Congratulations! :D You have exported % lights in the file %\n" count filenameXML
		xDoc.SaveXML()
		return xDoc.PrettySaveXML()
	)
	catch
	(
		format "The File % has not been generated due to an unexpected error\n" filenameXML
		return false
	)
)

ExportLights "C:\\Temp.xml"