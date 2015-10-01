fileIn "Utils.ms"
fileIn "XMLStruct.ms"

function GetMaterialCount aObj =
(
	local lMaterialCount = 0
	local lMaterial = aObj.material
	if classof lMaterial == Multimaterial then
	(
		lMaterialCount = lMaterial.count
	)
	else
	(
		lMaterialCount = 1
	)
	
	return lMaterialCount as string
)
	
--export de la class
fn ExportStaticMeshXMLFile filenameXML MeshDir aRoom =
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
		
		local NameVertexsMap = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int32]"
		for Obj in UniqueInstanceArray do
		(
				local ObjName = toLower Obj.name
				local ObjNameCoreAux = trimright ObjName "0123456789"
				
				local lNumVertices  = Obj.verts.count as string
				local lNumMaterials = GetMaterialCount Obj
				local ObjNameCore = ObjNameCoreAux + "v" + lNumVertices + "m" + lNumMaterials
			
			    if (NameVertexsMap.ContainsKey (ObjNameCore)) == false then
				(
					local l_NewStaticMeshNode = xDoc.getNewNode "static_mesh"

				    format "COre name %\n" ObjNameCore			
					-- append the new node static_mesh to the root node static_meshs
					l_RootNode.AppendNode l_NewStaticMeshNode 

					local l_Name = (ObjNameCore + "_" + aRoom) as String
					filename = "meshes\\" + ObjNameCore +".mesh"
			
					l_NewStaticMeshNode.AddAttributeAsString "name" l_Name
					l_NewStaticMeshNode.AddAttributeAsString "filename" filename
					NameVertexsMap.Add ObjNameCore 0
				)
		)
	)
	catch
	(
		format "Error generating mesh instance\n"
	)
	return xDoc.PrettySaveXML()
)

--export de la class
fn ExportRenderableObjectXMLFile AllObj filenameXML l_Layers l_Room=
(
	try
	(
			
		local unique_instances = GetUniqueInstancesScene()
		local m_LayersIndex = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
		
		local l_counter = 1
		
		for layer in l_Layers do
		(
			m_LayersIndex.Add layer l_counter
			l_counter = l_counter + 1
		)
		
		local m_LayersVector = #()
		while m_LayersIndex.count > m_LayersVector.count do
		(
			append m_LayersVector #()
		)
		
		if AllObj.count == 0 then 
		(
			format "Empty selection. Select any object \n"
			return -1
		)

		for Obj in AllObj do
		(
			
			local l_Layer = (getUserProp Obj "layer_render")
			
			if l_Layer == undefined then
			(
				l_Layer = "solid"
				setUserProp Obj "layer_render" "solid"
			)

			local l_index = m_LayersIndex.Item[l_Layer]
						
			append m_LayersVector[l_index] Obj
		)	
		
		for LayerItem in m_LayersVector do
		(
			local l_LayerFilename = (getUserProp LayerItem[1] "layer_render") 
			local l_XmlFilename = (filenameXML +"\\"+ l_LayerFilename + "_layer" + ".xml") as String						

			-- create the document and set the filename
			xDoc = XMLDocument l_XmlFilename
			
			--create the base node and append the node to the document
			local l_RootNode = xDoc.getNewNode "RenderableObjects"
			xDoc.AppendNode l_RootNode
				
			for Obj in LayerItem do
			(	
				local l_CoreName = GetObjectCore Obj unique_instances
				local lRotation = Obj.rotation 
				local l_Yaw = GetYaw lRotation as string
				local l_Pitch = GetPitch lRotation as string
				local l_Roll = GetRoll lRotation as string
				
				local ObjName = toLower Obj.name
				local ObjNameCoreAux = trimright ObjName "0123456789"
				
				local lNumVertices = Obj.verts.count as string
				local lNumMaterials = GetMaterialCount Obj
				local ObjNameCore = ObjNameCoreAux + "v" + lNumVertices + "m" + lNumMaterials
				
				local l_Name = (Obj.name + "_" + l_Room) as String
				local l_Core = (ObjNameCore + "_" + l_Room) as String
				
				l_Name = toLower l_Name
				
				local l_NewMeshInstanceNode = xDoc.getNewNode "MeshInstance"			
				-- append the new node MeshInstance to the root node RenderableObjects
				l_RootNode.AppendNode l_NewMeshInstanceNode 
				
				l_NewMeshInstanceNode.AddAttributeAsString "name" l_Name	
				l_NewMeshInstanceNode.AddAttributeAsString "core" l_Core
				l_NewMeshInstanceNode.AddAttributeCoordinate "pos" Obj.Pos
				
				l_NewMeshInstanceNode.AddAttributeAsString "yaw" l_Yaw
				l_NewMeshInstanceNode.AddAttributeAsString "pitch" l_Pitch
				l_NewMeshInstanceNode.AddAttributeAsString "roll" l_Roll
			
				l_NewMeshInstanceNode.AddAttributeCoordinate "scale" [Obj.Scale.z, Obj.Scale.x, Obj.Scale.y]
			)
			xDoc.PrettySaveXML()
		)	
		
	)
	catch
	(
		format "Error generating renderable object\n"
	)
)

fn ExportTextureFile AllObj DestPath = 
(

	if AllObj.count == 0 then 
	(
		format "Empty selection. Select any object \n"
		return -1
	)
		
	l_TextureUsed = DestPath + "TextureUsed.txt"
	file = createfile l_TextureUsed
	
	for Obj in AllObj do
	(
		try
		(
			local l_MaterialTexture = ""
			local l_MaterialTextureFilename = ""
			local l_ActualMaterial = ""
			local l_TexturesArray =#()

			if Obj.material != undefined then
			(
				local l_Material = Obj.material
				if classof l_Material == Standardmaterial then
				(
					l_Material = #(l_Material)
					--format "Object % Standard Material \n" l_Material
				)
				
				local l_NumMaterials = l_Material.count
				for iMaterial=1 to l_NumMaterials do
				(
					l_ActualMaterial = l_Material[iMaterial]
					--format "ActualMaterial % \n" l_ActualMaterial.name
					
					if l_ActualMaterial.diffuseMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.diffuseMap.filename
						l_MaterialTexture = l_ActualMaterial.diffuseMap.filename
						--format "DiffuseMap \n"
					)else if l_ActualMaterial.bumpMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.bumpMap.normal_map.filename
						l_MaterialTexture = l_ActualMaterial.bumpMap.normal_map.filename
						--format "bumpMap \n"
					)			
					else if l_ActualMaterial.ambientMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.ambientMap.filename
						l_MaterialTexture = l_ActualMaterial.ambientMap.filename
						--format "ambientMap \n"
					)
					else if  l_ActualMaterial.displacementMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.displacementMap.filename
						l_MaterialTexture = l_ActualMaterial.displacementMap.filename
						--format "displacementMap \n"
					)
					else if  l_ActualMaterial.filterMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.filterMap.filename
						l_MaterialTexture = l_ActualMaterial.filterMap.filename
						--format "filterMap \n"
					)
					else if l_ActualMaterial.glossinessMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.glossinessMap.filename
						l_MaterialTexture = l_ActualMaterial.glossinessMap.filename
						--format "glossinessMap \n"
					)
					else if  l_ActualMaterial.opacityMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.opacityMap.filename
						l_MaterialTexture = l_ActualMaterial.opacityMap.filename
						--format "opacityMap \n"
					)
					else if  l_ActualMaterial.reflectionMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.reflectionMap.filename
						l_MaterialTexture = l_ActualMaterial.reflectionMap.filename
						--format "reflectionMap \n"
					)
					else if l_ActualMaterial.refractionMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.refractionMap.filename
						l_MaterialTexture = l_ActualMaterial.refractionMap.filename
						--format "refractionMap \n"
					)
					else if  l_ActualMaterial.selfIllumMap != undefined then
					(
						-- TODO: RNM
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.selfIllumMap.filename
						l_MaterialTexture = l_ActualMaterial.selfIllumMap.filenameformat 
						--format "selfIllumMap \n"
					)
					else if  l_ActualMaterial.specularMap != undefined then
					(
						l_MaterialTextureFilename =filenameFromPath l_ActualMaterial.specularMap.filename
						l_MaterialTexture = l_ActualMaterial.specularMap.filename
						--format "specularMap \n"
					)
					else
					(
						l_MaterialTexture = "Material" + l_Material.name + " , texture " + l_ActualMaterial.name
					)
		
					format "% \n" (l_MaterialTexture as string) to:file
					
					DestTexturePath = DestPath + l_MaterialTextureFilename
					
					if doesFileExist DestTexturePath then
					(
						deleteFile DestTexturePath
					)
					
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
	close file
)

function GenerateMeshes aObjects MeshDir =
(
		-- create the document and set the filename
		local    filenameXML = MeshDir +"\\materials.xml"
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "materials"
		xDoc.AppendNode l_RootNode
	
		local NameVertexsMap       = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int32]"
		local ObjectsProcessed     = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
		for Obj in aObjects do
		(
			--try
			--(
				local ObjName = toLower Obj.name
				local ObjNameCoreAux = trimright ObjName "0123456789"
				
				local lNumVertices  = Obj.verts.count as string
				local lNumMaterials = GetMaterialCount Obj
				local ObjNameCore = ObjNameCoreAux + "v" + lNumVertices + "m" + lNumMaterials
				
				format "ObjectName % CoreName %\n" ObjName ObjNameCore
				
				local l_ObjectMaterialNode = xDoc.getNewNode "object_material"
				l_ObjectMaterialNode.AddAttributeAsString "name" ObjName
				
				--------------------------------------------------------------------------------
				if (NameVertexsMap.ContainsKey (ObjNameCore)) == true then
				(
					format "Core exist %\n" ObjNameCore
					local index = NameVertexsMap.Item[ObjNameCore] + 1
					NameVertexsMap.Remove ObjNameCore
					NameVertexsMap.Add ObjNameCore index
					
					-- Export materials
					local l_ObjectMesh = SMesh()
					l_ObjectMesh.InitFromMaxObject Obj
					l_ObjectMesh.WriteToXML xDoc l_ObjectMaterialNode
					fclose file
				)
				else
				(
					format "Adding core %\n" ObjNameCore
					
					NameVertexsMap.Add ObjNameCore 0
					
					-- Export mesh and materials
					filename = MeshDir +"\\"+ ObjNameCore + ".mesh"--poner parametrico
					local file = fopen filename "wb"
					local l_ObjectMesh = SMesh()
					l_ObjectMesh.InitFromMaxObject Obj
					l_ObjectMesh.WriteToFile file xDoc l_ObjectMaterialNode
					fclose file
				)
				l_RootNode.AppendNode l_ObjectMaterialNode
				--------------------------------------------------------------------------------
			--)
			--catch
			--(
				--format "Something was wrong with object: % \n" Obj
			--)
		)
		xDoc.PrettySaveXML()
		format "Meshes copied on: % \n" MeshDir	
)

function ExportLayerFile AllObj aDirToXML aRoom =
(
	try
	(
		local unique_instances = GetUniqueInstancesScene()
		local l_OutXmlFilenameToWrite = (aDirToXML +"\\"+ aRoom + ".xml")
		local l_OutXmlFilenameToXML = ("Data\\"+ aRoom)

		-- create the document and set the filename
		xDoc = XMLDocument l_OutXmlFilenameToWrite
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "room"
		l_RootNode.AddAttributeAsString "path" l_OutXmlFilenameToXML
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
							append layers (getUserProp Obj "layer_render")
							
							local l_LayerName =  layers[layers.count]
							
							local l_NewLayerNode = xDoc.getNewNode "layer"
							l_RootNode.AppendNode l_NewLayerNode
							l_NewLayerNode.AddAttributeAsString "name" l_LayerName
							l_NewLayerNode.AddAttributeAsString "file" (l_LayerName + "_layer.xml")
							format "LayerXML created on: % \n" (aDirToXML + "\\" + l_LayerName + "_layer.xml")
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
					l_NewLayerNode.AddAttributeAsString "file" (layers[1] + "_layer.xml")
					format "LayerXML created on: % \n"  (aDirToXML + "\\" + layers[1] + "_layer.xml")
				)
		)
		
		xDoc.PrettySaveXML()
		
		return layers
	)
	catch
	(
		format "Error generating layer XML file\n"
	)
)