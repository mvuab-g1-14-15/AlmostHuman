--includes
fileIn "XMLStruct.ms"
fileIn "Utils.ms"

/*
	Export all Renderable objects to the engine
*/
fn ExportRenderableObjects filenameXML RenderableObjects =
(
	clearListener()
	
	try
	(
		
		if RenderableObjects.count == 0 then
		(
			format "Empty selection. Select an object \n"
			return false
		)
		
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "renderable_objects"
		xDoc.AppendNode l_RootNode
		
		local l_UniqueInstanceArray = GetUniqueInstances()
		
		-- iterate for all the objects
		local l_RenderableObjectsCount = 0;
		for iRenderableObject in RenderableObjects do
		(
			try
			(
				--create the light node
				local l_NewObjectNode = xDoc.getNewNode "mesh_instance"
			
				-- append the new node mesh_instance
				l_RootNode.AppendNode l_NewObjectNode

				local l_ObjectName = iRenderableObject.name
				
				-- Get the name of the core of this mesh
				local l_CoreName;
				if findItem l_UniqueInstanceArray iRenderableObject != 0 then
				(
					l_CoreName = iRenderableObject.name
				)
				else
				(
					local references = GetNodeReferences iRenderableObject
					if (references.count > 0) then
					(
						for ref_obj in references do
						(
							if findItem l_UniqueInstanceArray ref_obj != 0 then
							(
								l_CoreName = ref_obj.name
							)
						)
					)
					else
					(
						l_CoreName = iRenderableObject.name
					)
				)
				
				l_NewObjectNode.AddAttributeAsString "name" l_ObjectName
				l_NewObjectNode.AddAttributeAsString "core" l_ObjectName
				
				-- Save the position of the object in Right Handed reference system
				local l_Pos = iRenderableObject.pos
				l_Pos = point3 l_Pos.x l_Pos.z l_Pos.y
				l_NewLightNode.AddAttributeCoordinate "pos" l_Pos
				
				-- Get the angles of the object
				local l_Yaw = GetYaw iRenderableObject.transform
				l_NewLightNode.AddAttributeFloat "yaw" l_Yaw
				local l_Pitch = GetPitch iRenderableObject.transform
				l_NewLightNode.AddAttributeFloat "pitch" l_Pitch
				local l_Roll = GetRoll iRenderableObject.transform
				l_NewLightNode.AddAttributeFloat "roll" l_Roll
				
				-- Get the scale of the object
				local l_Scale = iRenderableObject.scale
				l_Scale = point3 l_Scale.x l_Scale.z l_Scale.y
				l_NewLightNode.AddAttributeCoordinate "scale" l_Scale

				l_RenderableObjectsCount = l_RenderableObjectsCount + 1
				)
			)
			catch
			(
				format "Error instance % would not be exported\n" l_ObjectName
			)
		)
		format "% Mesh instances have been exported to %\n" l_RenderableObjectsCount filenameXML
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