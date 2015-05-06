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
		
		local UniqueInstanceArray = GetUniqueInstancesScene()
				
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
	try
	(
		local unique_instances = GetUniqueInstancesScene()
		
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
			
			local l_CoreName = GetObjectCore Obj unique_instances
			local lRotation = Obj.rotation 
			local l_Yaw = GetYaw lRotation as string
			local l_Pitch = GetPitch lRotation as string
			local l_Roll = GetRoll lRotation as string

			local l_NewMeshInstanceNode = xDoc.getNewNode "MeshInstance"			
			-- append the new node MeshInstance to the root node RenderableObjects
			l_RootNode.AppendNode l_NewMeshInstanceNode 

			l_NewMeshInstanceNode.AddAttributeAsString "name" Obj.name	
			l_NewMeshInstanceNode.AddAttributeAsString "core" l_CoreName
			l_NewMeshInstanceNode.AddAttributeCoordinate "pos" Obj.Pos
			
			l_NewMeshInstanceNode.AddAttributeAsString "yaw" l_Yaw
			l_NewMeshInstanceNode.AddAttributeAsString "pitch" l_Pitch
			l_NewMeshInstanceNode.AddAttributeAsString "roll" l_Roll
		
			l_NewMeshInstanceNode.AddAttributeCoordinate "scale" [Obj.Scale.z, Obj.Scale.x, Obj.Scale.y]
			
			if (getUserProp Obj "layer_render") == undefined then
			(
				l_NewMeshInstanceNode.AddAttributeAsString "layer" "solid"
			)
			else
			(
				l_NewMeshInstanceNode.AddAttributeAsString "layer" (getUserProp Obj "layer_render")
			)
		)	
		
	)
	catch
	(
		format "Error generating renderable object\n"
	)
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
			local l_MaterialTexture = ""
			local l_MaterialTextureFilename = ""
			local l_ActualMaterial = ""

			if Obj.material != undefined then
			(
				local l_Material = Obj.material
				if classof l_Material == Standardmaterial then
				(
					l_Material = #(l_Material)
					format "Object % Standard Material \n" l_Material
				)
				
				local l_NumMaterials = l_Material.count
				for iMaterial=1 to l_NumMaterials do
				(
					l_ActualMaterial = l_Material[iMaterial]
					format "ActualMaterial % \n" l_ActualMaterial.name
					
					if l_ActualMaterial.diffuseMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.diffuseMap.filename
						l_MaterialTexture = l_ActualMaterial.diffuseMap.filename
						format "DiffuseMap \n"
					)else if l_ActualMaterial.bumpMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.bumpMap.normal_map.filename
						l_MaterialTexture = l_ActualMaterial.bumpMap.normal_map.filename
						format "bumpMap \n"
					)			
					else if l_ActualMaterial.ambientMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.ambientMap.filename
						l_MaterialTexture = l_ActualMaterial.ambientMap.filename
						format "ambientMap \n"
					)
					else if  l_ActualMaterial.displacementMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.displacementMap.filename
						l_MaterialTexture = l_ActualMaterial.displacementMap.filename
						format "displacementMap \n"
					)
					else if  l_ActualMaterial.filterMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.filterMap.filename
						l_MaterialTexture = l_ActualMaterial.filterMap.filename
						format "filterMap \n"
					)
					else if l_ActualMaterial.glossinessMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.glossinessMap.filename
						l_MaterialTexture = l_ActualMaterial.glossinessMap.filename
						format "glossinessMap \n"
					)
					else if  l_ActualMaterial.opacityMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.opacityMap.filename
						l_MaterialTexture = l_ActualMaterial.opacityMap.filename
						format "opacityMap \n"
					)
					else if  l_ActualMaterial.reflectionMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.reflectionMap.filename
						l_MaterialTexture = l_ActualMaterial.reflectionMap.filename
						format "reflectionMap \n"
					)
					else if l_ActualMaterial.refractionMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.refractionMap.filename
						l_MaterialTexture = l_ActualMaterial.refractionMap.filename
						format "refractionMap \n"
					)
					else if  l_ActualMaterial.selfIllumMap != undefined then
					(
						-- TODO: RNM
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.selfIllumMap.filename
						l_MaterialTexture = l_ActualMaterial.selfIllumMap.filenameformat 
						format "selfIllumMap \n"
					)
					else if  l_ActualMaterial.specularMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.specularMap.filename
						l_MaterialTexture = l_ActualMaterial.specularMap.filename
						format "specularMap \n"
					)
					
					DestTexturePath = DestPath +"\\"+ l_MaterialTextureFilename
					if copyFile l_MaterialTexture DestTexturePath then
					(
						format "Texture % copied on: % \n" l_MaterialTexture DestTexturePath
					)
					else
					(
						format "Texture % FAILED TO COPY on: % \n" l_MaterialTexture DestTexturePath
					)
					
				)
			)
		)	
		catch
		(
			format "Error while exporting object\n"
		)	
	)
)

function GenerateMeshes aObjects MeshDir =
(
	for Obj in aObjects do
	(
		filename = MeshDir +"\\"+ Obj.name + ".mesh"--poner parametrico
		local file = fopen filename "wb"
		local l_ObjectMesh = SMesh()
		l_ObjectMesh.InitFromMaxObject Obj
		l_ObjectMesh.WriteToFile file
		fclose file
	)
	
	format "Meshes copied on: % \n" MeshDir	
)