fileIn "Utils.ms"
fileIn "XMLStruct.ms"

--export de la class
fn ExportStaticMeshXMLFile filenameXML MeshDir =
(	
	try
	(
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "static_meshes"
		xDoc.AppendNode l_RootNode
		
		local UniqueInstanceArray = GetUniqueInstances()
				
		if UniqueInstanceArray.count == 0 then 
		(
			format "Empty selection. Select any object \n"
			return -1
		)
		
		for Obj in UniqueInstanceArray do
		(
				filename = "\\meshes\\" + Obj.name +".mesh"
				
				local l_NewStaticMeshNode = xDoc.getNewNode "static_mesh"			
				-- append the new node static_mesh to the root node static_meshs
				l_RootNode.AppendNode l_NewStaticMeshNode 

				l_NewStaticMeshNode.AddAttributeAsString "name" Obj.name	
				l_NewStaticMeshNode.AddAttributeAsString "filename" filename

		)
	)
	catch
	(
		format "Error generating mesh instance\n"
	)
	return xDoc.PrettySaveXML()
)

--export de la class
fn ExportRenderableObjectXMLFile AllObj filenameXML =
(
	--try
	--(
		local unique_instances = GetUniqueInstances()
		
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "RenderableObjects"
		xDoc.AppendNode l_RootNode
		
		if AllObj.count == 0 then 
		(
			format "Empty selection. Select any object \n"
			return -1
		)
		
		--TODO: Petan los layers
		
		local layers=#()

		for Obj in AllObj do
		(
			if layers.count != 0 then
			(
				local Exists = false
				for i=1 to layers.count do
				(
					if layers[i] == (getUserProp Obj "layer_render") then
					(
						Exists = true	
					)
				)
				if Exists == false then
				(
					if (getUserProp Obj "layer_render") != undefined then
					(
						layers[(layers.count+1)] = getUserProp Obj "layer_render"
						local l_NewLayerNode = xDoc.getNewNode "layer"
						l_RootNode.AppendNode l_NewLayerNode
						l_NewLayerNode.AddAttributeAsString "name" layers[layers.count-1]
					)
				)
			)
			else
			(
				if (getUserProp Obj "layer_render") == undefined then
				(
					layers[1] = "solid"
				)
				else
				(
					layers[1] = getUserProp Obj "layer_render"
				)
				local l_NewLayerNode = xDoc.getNewNode "layer"
				l_RootNode.AppendNode l_NewLayerNode
				l_NewLayerNode.AddAttributeAsString "name" layers[1]
			)
		)
		
		for Obj in AllObj do
		(
			local instance_obj;
			if findItem unique_instances Obj != 0 then
				instance_obj = Obj
			else
			(
				local references = GetNodeReferences Obj
				if (references.count > 0) then
				(
					for ref_obj in references do
					(
						if findItem unique_instances ref_obj != 0 then
							instance_obj = ref_obj
					)
				)
				else
				(
					instance_obj = Obj
				)
			)
		
			local l_Yaw = GetYaw Obj.transform as string
			local l_Pitch = GetPitch Obj.transform as string
			local l_Roll = GetRoll Obj.transform as string
			
			local l_NewMeshInstanceNode = xDoc.getNewNode "MeshInstance"			
			-- append the new node MeshInstance to the root node RenderableObjects
			l_RootNode.AppendNode l_NewMeshInstanceNode 

			l_NewMeshInstanceNode.AddAttributeAsString "name" Obj.name	
			l_NewMeshInstanceNode.AddAttributeAsString "core" instance_obj.name
			l_NewMeshInstanceNode.AddAttributeCoordinate "pos" Obj.Pos
			l_NewMeshInstanceNode.AddAttributeAsString "yaw" l_Yaw
			l_NewMeshInstanceNode.AddAttributeAsString "pitch" l_Pitch
			l_NewMeshInstanceNode.AddAttributeAsString "roll" l_Roll
			l_NewMeshInstanceNode.AddAttributeCoordinate "scale" Obj.Scale
			--l_NewMeshInstanceNode.AddAttributeAsString "layer" "solid"
			if (getUserProp Obj "layer_render") == undefined then
			(
				l_NewMeshInstanceNode.AddAttributeAsString "layer" "solid"
			)
			else
			(
				l_NewMeshInstanceNode.AddAttributeAsString "layer" (getUserProp Obj "layer_render")
			)
			
			
		)
	--)
	--catch
	--(
		--format "Error generating renderable object\n"
	--)
	return xDoc.PrettySaveXML()
)

fn ExportTextureFile AllObj DestPath = 
(
	
	if AllObj.count == 0 then 
	(
		format "Empty selection. Select any object \n"
		return -1
	)

	for Obj in AllObj do
	(
		try
		(
			maxOps.cloneNodes Obj cloneType:#instance newNodes:&newObj #nodialog
			convertToMesh newObj
			actualObj = newObj[1]

			filename = filenameFromPath actualObj.material.diffuseMap.filename
			DestTexturePath = DestPath +"\\"+ filename
			
			format "Texture % copied on: % \n" Obj.material.diffuseMap.filename DestTexturePath
			copyFile Obj.material.diffuseMap.filename DestTexturePath
		)
		catch
		(
			format "Error while exporting object\n"
		)
		delete newObj
	)		
)