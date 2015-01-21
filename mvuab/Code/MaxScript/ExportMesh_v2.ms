fileIn "GetAABB.ms"
fileIn "VertexTypes.ms"
fileIn "CalculateNormals.ms"

clearListener()

struct STexture
(
	m_Name = "",
	m_Path = "",
	
	fn FromMaxTexture textureFileName =
	(
		m_Name = filenameFromPath textureFileName
		m_Path = getFilenamePath textureFileName
	),
	
	fn IsNotNull =
	(
		return m_Name.count > 0
	),
	
	fn WriteToFile file = 
	(
		if IsNotNull() then
		(
			WriteShort file m_Name.count #unsigned--Write Lenght Name
			WriteString file m_Name --Write Name
		)
		else
		(
			l_Name = "test.png"
			WriteShort file l_Name.count #unsigned--Write Lenght Name
			WriteString file l_Name --Write Name
		)
	)
)

struct SMaterial
(
	m_Type,
	m_Texture1 = STexture(),
	m_Texture2  = STexture(),
	m_TextureBump  = STexture(),
	m_Color = 0,
	
	fn FromMaxMaterial mat =
	(
		m_Type = (bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL)

		if material != undefined then
		(
			if mat.diffuseMapEnable == true AND classOf(mat.diffuseMap) == BitmapTexture then
			(
				m_Type = (bit.or m_Type VERTEX_TYPE_TEXTURE1)
				-- Load the texture 1
				local l_Texture1FileName = mat.diffuseMap.filename
				m_Texture1.FromMaxTexture l_Texture1FileName

				if mat.selfIllumMapEnable == true AND classOf(mat.selfIllumMap) == BitmapTexture then
				(
					m_Type = (bit.or m_Type VERTEX_TYPE_TEXTURE2)
					-- Load the texture 2
					local l_Texture2FileName = mat.selfIllumMap.filename
					m_Texture2.FromMaxTexture l_Texture2FileName
				)
				
				if mat.bumpMapEnable == true AND classOf(mat.bumpMap) == BitmapTexture then
				(
					m_Type = (bit.or m_Type (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) )
					-- Load the texture of bump mapping
					local l_TextureBumpMappingFileName = mat.bumpMap.filenames
					m_TextureBump.FromMaxTexture l_TextureBumpMappingFileName
				)
			)
			else
			(
				m_Type = (bit.or m_Type VERTEX_TYPE_DIFFUSE)
				m_Color = bit.shift mat.diffuse.r 24 + bit.shift mat.diffuse.g 16 + bit.shift mat.diffuse.b 8 + mat.diffuse.a
			)
		)
	),
	
	fn WriteToFile file =
	(
		format "Writing Material\n"
		
		WriteShort file m_Type #unsigned
		format "Type: %\n" m_Type
		
		local l_nTextures = 0
		if ((bit.and m_Type VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 ) then 
		(
			l_nTextures = l_nTextures + 1
		)
		if ((bit.and m_Type VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 ) then 
		(
			l_nTextures = l_nTextures + 1
		)
		if (bit.and m_Type (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) ) == (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) then
		(
			l_nTextures = l_nTextures + 1
		)
		
		WriteShort file l_nTextures #unsigned
		
		if ((bit.and m_Type VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 ) then 
		(
			m_Texture1.WriteToFile file
		)
		if ((bit.and m_Type VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 ) then 
		(
			m_Texture2.WriteToFile file
		)
		if (bit.and m_Type (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) ) == (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) then
		(
			m_TextureBump.WriteToFile file
		)
		format "Writing Material End\n"
	)
)

struct SVertexBuffer
(
	m_VertexList = #(),
	m_VertexIdMap = dotNetObject "System.Collections.Hashtable",
	
	fn AddVertex vertex =
	(
		local l_VertexStr = vertex.GetAsString()
		local l_VertexIdx = m_VertexIdMap.Item[l_VertexStr];
		if ( l_VertexIdx == undefinded ) then
		(
			append m_VertexList vertex
			l_VertexIdx = m_VertexList.count-1
			m_VertexIdMap.Add l_VertexStr l_VertexIdx
		)
		
		return l_VertexIdx
	),
	
	fn WriteToFile file =
	(
		format "Writing BV\n"
		local l_NumVertex = m_VertexList.count 
		WriteShort file l_NumVertex #unsigned
		for iVertex in m_VertexList do
		(
			iVertex.WriteToFile file
		)
		format "Writing BV end\n"
	)
)

struct SIndexBuffer
(
	m_IndexList = #(),
	
	fn AddIndex index =
	(
		append m_IndexList index
	),
	
	fn WriteToFile file =
	(
		format "Writing IB\n"
		local l_NumIds = m_IndexList.count
		WriteShort file l_NumIds #unsigned
		format "Idx count: %\n" l_NumIds
		for iIndex in m_IndexList do
		(
			WriteShort file iIndex #unsigned
			format "Idx actual id: %\n" iIndex
		)
		
		format "Writing IB end\n"
	)
)

struct SAABB
(
	m_PointMin = point3 0 0 0,
	m_PointMax = point3 0 0 0,
	
	fn WriteToFile file = 
	(
		WriteFloat file m_PointMin.x
		WriteFloat file m_PointMin.z
		WriteFloat file m_PointMin.y
		WriteFloat file m_PointMax.x
		WriteFloat file m_PointMax.z
		WriteFloat file m_PointMax.y
	),
	
	fn CalculateFromObject Obj =
	(
		local bb = GetAABB Obj
		m_PointMin = point3 bb[1].x bb[1].y bb[1].z
		m_PointMax = point3 bb[2].x bb[2].y bb[2].z
	)
)

struct SBndSphere
(
	m_Radious = 0,
	m_Center = point3 0 0 0
)

struct SSubMesh
(
	m_Material = SMaterial(),
	m_IB = SIndexBuffer(),
	m_VB = SVertexBuffer(),
	
	fn WriteToFile file =
	(
		m_Material.WriteToFile file
		m_VB.WriteToFile file
		m_IB.WriteToFile file
	)
)

struct SMesh
(
	m_SubMeshes = #(),
	m_AABB = SAABB(),
	m_BS = SBndSphere(),
    m_MaterialsHashTable = dotNetObject "System.Collections.Hashtable",
	
	fn CreateSubMeshes Obj =
	(
		format "Creating submeshes\n"
    	local l_Material = Obj.material
		if classof( l_Material ) == Multimaterial then
		(
			format "Saving multimaterial object...\n"
			local l_NumMaterials = getNumfaces l_Material
            
			for iMaterial=1 to l_NumMaterials do
			(
            	try
            	(
					local l_MaterialId = getFaceMatId Obj iMaterial
					
					local l_MaterialNameById = m_MaterialsHashTable.Item[l_MaterialId];
			
					format "Material Name: %\n" l_MaterialNameById
					
					-- Check if the material is not already added
        	        if l_MaterialNameById == undefined then
    	            (
						-- Obtain the submaterial from the main material
	                	local l_NewMaterial = l_Material[l_MaterialId]
						
						-- Create a new submehs
                    	local l_NewSubMesh = SSubMesh()
						
						-- Init the material of the new sub mesh with the max material
                    	l_NewSubMesh.m_Material.FromMaxMaterial l_NewMaterial
						
						--Finally add the new submesh to the submeshes in the object
						append m_SubMeshes l_NewSubMesh
						
						--Update the donet map in order have the same data as the submeshes
						m_MaterialsHashTable.Add l_MaterialId l_NewMaterial.name
					)
            	)
				catch
				(
					format "Error while exporting submaterial %.\n" l_MaterialId
				)
			)
            
			format "Num mats: % v %\n" materials.count materials
		)
		else
		(
			format "Single Material Object\n"
         	local l_NewSubMesh = SSubMesh()
            l_NewSubMesh.m_Material.FromMaxMaterial l_Material
			format "Material Name: %\n" l_Material.name
			append m_SubMeshes l_NewSubMesh
			format "Submesh: %\n" m_SubMeshes
			local l_MaterialId = 1
			local l_MaterialName = l_Material.name as string
			m_MaterialsHashTable.Add l_MaterialId l_MaterialName
		)
	),
	
	fn CreateBuffers Obj =
	(
		local l_nFaces = getNumFaces Obj
		
		local InvTransform=inverse Obj.transform
		for iFaces = 1 to l_nFaces do
		(
			format "Parsing face %...\n" iFaces
			
			local l_MaterialId
			if classof(Obj.material) == Multimaterial then
			(
				l_MaterialId=getFaceMatID Obj iFaces
			)
			else
			(
				l_MaterialId = 1
			)
		
			local l_MaterialNameById = m_MaterialsHashTable.Item[l_MaterialId];
			
			--format "Material Name: %\n" l_MaterialNameById
			
			-- Check if the material is found
			if ( l_MaterialNameById != undefined ) then
			(
				local l_Type = m_SubMeshes[l_MaterialId].m_Material.m_Type
				
				local l_NewVertex1 = SVertex()
				local l_NewVertex2 = SVertex()
				local l_NewVertex3 = SVertex()
				
				l_NewVertex1.m_Type = l_Type
				l_NewVertex2.m_Type = l_Type
				l_NewVertex3.m_Type = l_Type
              
				-- Para coger los indices de la primera cara
				local IdxsFace=getFace Obj iFaces
				
				format "Face: %\n" IdxsFace
				-- Para coger los vértices de la primera cara
				local Coord1=getVert Obj IdxsFace.x*InvTransform
				local Coord2=getVert Obj IdxsFace.y*InvTransform
				local Coord3=getVert Obj IdxsFace.z*InvTransform
              
              	l_NewVertex1.m_Position = Coord1
                l_NewVertex2.m_Position = Coord2
                l_NewVertex3.m_Position = Coord3
				
				UABCalcVertexsNormals Obj

				local l_FaceNormal = getFaceNormal Obj iFaces
				local l_SmoothValue = getFaceSmoothGroup Obj iFaces
				
				-- Para coger las normales de los vértices de la primera cara
				local Normal1 = UABGetVertexNormal Obj IdxsFace.x l_SmoothValue l_FaceNormal
				local Normal2 = UABGetVertexNormal Obj IdxsFace.y l_SmoothValue l_FaceNormal
				local Normal3 = UABGetVertexNormal Obj IdxsFace.z l_SmoothValue l_FaceNormal
              
                --igual con normales
                l_NewVertex1.m_Normal = Normal1
                l_NewVertex2.m_Normal = Normal2
                l_NewVertex3.m_Normal = Normal3

				if ((bit.and l_Type VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 ) then 
				(
					/* Para coger los índices de las coordenadas de textura de la primera cara, el 1 señala al primer
					conjunto de coordenadas aplicadas sobre la cara */
					local IdxsMap = meshop.getMapFace Obj 1 iFaces
					
					-- Para coger las coordenadas de los vértices de la primera cara
					local TUV1Map1= meshop.getMapVert Obj 1 IdxsMap.x
					local TUV1Map2= meshop.getMapVert Obj 1 IdxsMap.y
					local TUV1Map3= meshop.getMapVert Obj 1 IdxsMap.z
				
					l_NewVertex1.m_UV = point2 TUV1Map1.x 1.0 - TUV1Map1.y
					l_NewVertex2.m_UV = point2 TUV1Map2.x 1.0 - TUV1Map2.y
					l_NewVertex3.m_UV = point2 TUV1Map3.x 1.0 - TUV1Map3.y

				)
			
				if ((bit.and l_Type VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 ) then 
				(
					local IdxsMap = meshop.getMapFace Obj 2 iFaces
			  
					-- Para coger las coordenadas de los vértices de la primera cara
					local TUV2Map1= meshop.getMapVert Obj 2 IdxsMap.x
					local TUV2Map2= meshop.getMapVert Obj 2 IdxsMap.y
					local TUV2Map3= meshop.getMapVert Obj 2 IdxsMap.z
			  
					l_NewVertex1.m_UV2 = point2 TUV2Map1.x 1.0 - TUV2Map1.y
					l_NewVertex2.m_UV2 = point2 TUV2Map2.x 1.0 - TUV2Map2.y
					l_NewVertex3.m_UV2 = point2 TUV2Map3.x 1.0 - TUV2Map3.y
				)
			
				if (bit.and l_Type (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) ) == (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) then
				(
					l_Binormal1 = point3 0 0 0
					l_Binormal2 = point3 0 0 0
					l_Binormal3 = point3 0 0 0
					l_NewVertex1.m_Binormal = l_Binormal1
					l_NewVertex2.m_Binormal = l_Binormal2
					l_NewVertex3.m_Binormal = l_Binormal3
					
					l_Tan1 = point3 0 0 0
					l_Tan2 = point3 0 0 0
					l_Tan3 = point3 0 0 0
					l_NewVertex1.m_Tan = l_Tan1
					l_NewVertex2.m_Tan = l_Tan2
					l_NewVertex3.m_Tan = l_Tan3
				)
			
				if ((bit.and l_Type VERTEX_TYPE_DIFFUSE) == VERTEX_TYPE_DIFFUSE) then
				(
					l_Color = bit.shift Obj.material.diffuse.r 24 + bit.shift Obj.material.diffuse.g 16 + bit.shift Obj.material.diffuse.b 8 + Obj.material.diffuse.a
					l_NewVertex1.m_Color = l_Color
					l_NewVertex2.m_Color = l_Color
					l_NewVertex3.m_Color = l_Color
				)
		
				-- Add the new vertex
				local l_Id1 = m_SubMeshes[l_MaterialId].m_VB.AddVertex l_NewVertex1
				local l_Id2 = m_SubMeshes[l_MaterialId].m_VB.AddVertex l_NewVertex2
				local l_Id3 = m_SubMeshes[l_MaterialId].m_VB.AddVertex l_NewVertex3
				
				-- Add the new indexes
				m_SubMeshes[l_MaterialId].m_IB.AddIndex l_Id1
				m_SubMeshes[l_MaterialId].m_IB.AddIndex l_Id3
				m_SubMeshes[l_MaterialId].m_IB.AddIndex l_Id2
			)
		)	
		
		format "% submeshes found on the object\n" m_SubMeshes.count
	),
	
	fn InitFromMaxObject Obj =
	(
		maxOps.cloneNodes Obj cloneType:#instance newNodes:&newObj #nodialog
		convertToMesh newObj
		actualObj = newObj[1]
		CreateSubMeshes actualObj
		CreateBuffers actualObj
		
		delete newObj
	),
	

	fn WriteToFile file =
	(
		format "Write to file\n"
		WriteShort file 0x55ff #unsigned
		WriteShort file m_SubMeshes.count #unsigned--Write number of materials
	
		for iSubMesh in m_SubMeshes do
		(
			format "Current Submesh%\n" iSubMesh
			iSubMesh.WriteToFile file
		)
		
		m_AABB.WriteToFile file
	
		WriteShort file 0xff55 #unsigned
	)
)

