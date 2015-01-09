fileIn "XMLStruct.ms"
fn ExportStaticMeshes filenameXML=
(
	--clearListener()
	try
	(
		local l_UniqueInstanceArray = GetUniqueInstances()
		if l_UniqueInstanceArray.count == 0 then 
		(
			format "Empty selection. Select any object \n"
			return false
		)
		
		-- Create the document and set the filename
		xDoc = XMLDocument()
		xDoc.file = 	filenameXML
		xDoc.loaded = true
		
		--create the base node
		l_RootNode = xDoc.getNewNode "static_meshes"
		xDoc.AppendNode l_RootNode
		l_RootNode = xDoc.getRootNode()
		xDoc.SaveXML()
		
		for iStaticMesh in l_UniqueInstanceArray do
		(
			try
			(
				--create the light node
				l_NewObjNode = xDoc.getNewNode "static_mesh"
			
				-- append the new node light to the root node lights
				l_RootNode.AppendNode l_NewLightNode
			
				l_ObjectName = Obj.name
				l_Filename = "\\meshes\\" + l_ObjectName +".mesh"
				l_NewObjNode.AddAttributeAsString "name" l_ObjectName
				l_NewObjNode.AddAttributeAsString "filename" l_Filename
			)
			catch
			(
				format "Error Object % would not be exported\n" l_ObjectName
			)
		)
		
		xDoc.SaveXML()
		return xDoc.PrettySaveXML()
	)
	catch
	(
		format "The File % has not been generated due to an unexpected error\n" filenameXML
		return false
	)
)