--includes
fileIn "XMLStruct.ms"

/*
	Export all the lights of the scenary into the format of the UABEngine
	Notes:
	The allowed types of lights in the exportation are only:
	- Omni
	- Target Spot
	- Target Direct
	The other, including the target objects must not be exported.
*/
fn ExportLights filenameXML =
(
	clearListener()
	
	try
	(
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "lights"
		xDoc.AppendNode l_RootNode
		
		-- iterate for all the lights and exported the allowed types into the xml file
		local l_LightsCount = 0;
		for iLight in lights do
		(
			try
			(
				if classof iLight != Targetobject and 
				   classof iLight != freeDirect and
				   classof iLight != freeSpot
				then
				(
					local l_LightName = iLight.name
					--create the light node
					local l_NewLightNode = xDoc.getNewNode "light"
			
					-- append the new node light to the root node lights
					l_RootNode.AppendNode l_NewLightNode
					
					--add the common attributes of the light
					local l_LightTypeStr = iLight.type as string
					l_NewLightNode.AddAttributeAsString "type" l_LightTypeStr
					l_NewLightNode.AddAttributeAsString "name" l_LightName
					l_NewLightNode.AddAttributeCoordinate "pos" iLight.pos
					l_NewLightNode.AddAttributeRGB "color" iLight.rgb
					l_NewLightNode.AddAttributeFloat "att_start_range" iLight.farAttenStart
					l_NewLightNode.AddAttributeFloat "att_end_range" iLight.farAttenEnd
					
					--add the specific data for each light
					if classof iLight == TargetDirectionallight then
					(
						local l_Direction = iLight.target.pos-iLight.pos
						
						l_NewLightNode.AddAttributeCoordinate "dir" l_Direction
					)
					else if classof iLight == targetSpot then
					(
						local l_Direction = iLight.target.pos-iLight.pos
						
						l_NewLightNode.AddAttributeCoordinate "dir" l_Direction
						l_NewLightNode.AddAttributeFloat "angle" iLight.hotspot
						l_NewLightNode.AddAttributeFloat "fall_off" iLight.Falloff
						l_NewLightNode.AddAttributeBool "render_shadows" false
					)
					l_LightsCount = l_LightsCount + 1
				)
			)
			catch
			(
				format "Error Light % would not be exported\n" l_LightName
			)
		)
		format "% Lights have been exported to %\n" l_LightsCount filenameXML
		return xDoc.PrettySaveXML()
	)
	catch
	(
		format "The File % has not been generated due to an unexpected error\n" filenameXML
		format "Check the path of the file\n"
		return false
	)
)

ExportLights "C:\Users\Alex\camera.xml"