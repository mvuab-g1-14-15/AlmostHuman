fileIn "GetAABB.ms"

global VERTEX_TYPE_GEOMETRY 	= 0x0001
global VERTEX_TYPE_NORMAL 		= 0x0002
global VERTEX_TYPE_TANGENT 	 	= 0x0004
global VERTEX_TYPE_BINORMAL	 	= 0x0008
global VERTEX_TYPE_TEXTURE1 	= 0x0010
global VERTEX_TYPE_TEXTURE2 	= 0x0020
global VERTEX_TYPE_DIFFUSE 		= 0x0040

clearListener()

struct STexture
(
	m_Name = "",
	m_Path = "",
	
	fn FromMaxTexture textureFileName =
	(
		m_Name = filenameFromPath textureFileName
		m_Path = getFilenamePath textureFileName
	)
)

struct SMaterial
(
	m_Type,
	m_Texture1 = STexture(),
	m_Texture2  = STexture(),
	m_TextureBump  = STexture(),
	
	fn FromMaxMaterial mat =
	(
		m_Type = (bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL)

		if material != undefined then
		(
			if mat.diffuseMapEnable == true AND classOf(mat.diffuseMap) == BitmapTexture then
			(
				m_Type = (bit.or type VERTEX_TYPE_TEXTURE1)
				-- Load the texture 1
				local l_Texture1FileName = mat.diffuseMap.filename
				m_Texture1.FromMaxTexture l_Texture1FileName

				if mat.selfIllumMapEnable == true AND classOf(mat.selfIllumMap) == BitmapTexture then
				(
					m_Type = (bit.or type VERTEX_TYPE_TEXTURE2)
					-- Load the texture 2
					local l_Texture2FileName = mat.selfIllumMap.filename
					m_Texture2.FromMaxTexture l_Texture2FileName
				)
				
				if mat.bumpMapEnable == true AND classOf(mat.bumpMap) == BitmapTexture then
				(
					m_Type = (bit.or type (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) )
					-- Load the texture of bump mapping
					local l_TextureBumpMappingFileName = mat.bumpMap.filenames
					m_TextureBump.FromMaxTexture l_TextureBumpMappingFileName
				)
			)
			else
			(
				m_Type = (bit.or m_Type VERTEX_TYPE_DIFFUSE)
			)
		)
	)
)

struct SVertex
(
	m_Position = point3 0 0 0,
	m_Normal = point3 0 0 0,
	m_UV = point2 0 0,
	m_UV2 = point2 0 0,
	m_Color = 0,
	
	fn ToStr = 
	(
		local l_Text = stringstream ""
		--convert position
		format "% % % " m_Position.x m_Position.y m_Position.z to:l_Text
		--convert normal
		format "% % % " m_Normal.x m_Normal.y m_Normal.z to:l_Text
		--convert UV
		format "% % " m_UV.x m_UV.y to:l_Text
		--convert UV2
		format "% % " m_UV2.x m_UV2.y to:l_Text
		--convert Color
		format "%" m_Color to:l_Text
		return (l_Text as string)
	),
	
	fn WriteToFile =
	(
		format "\tPos: (%, %, %)\n" m_Position.x m_Position.z m_Position.y
		WriteFloat file m_Position.x
		WriteFloat file m_Position.z
		WriteFloat file m_Position.y
		
		
		format "\tNormal: (%, %, %)\n" m_Normal.x m_Normal.z m_Normal.y
		WriteFloat file m_Normal.x
		WriteFloat file m_Normal.z
		WriteFloat file m_Normal.y
		
		format "\tUV1: (%, %, %)\n" m_UV.x m_UV.y
		WriteFloat file m_UV.x
		WriteFloat file m_UV.y
	
		format "\tUV2: (%, %)\n" m_UV2.x m_UV2.y
		WriteFloat file m_UV2.x
		WriteFloat file m_UV2.y
		
		format "\tColor: (%, %, %)\n" m_Color.r m_Color.g m_Color.b
		WriteFloat file m_Color.r
		WriteFloat file m_Color.g
		WriteFloat file m_Color.b
	)
)

struct SVertexBuffer
(
	m_VertexList = #(),
	m_VertexIdMap = #(dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"),
	
	fn AddVertex vertex =
	(
		local l_VertexStr = vertex.ToStr
		if (m_VertexIdMap.ContainsKey (l_VertexStr))==false then
		(
			append m_VertexList vertex
			local l_Idx = m_VertexList.count-1
			m_VertexIdMap.Add l_VtxString l_Idx
			return l_Idx
		)
		else
		(
			return m_VertexIdMap.Item[l_VertexStr]
		)
	),
	
	fn WriteToFile file =
	(
			local l_NumVertex = m_VertexList.count 
			WriteShort file num_vertex #unsigned
			for iVertex = 1 to l_NumVertex do
			(
				m_VertexList[iVertex].WriteToFile file
			)
	
	)
)

struct SIndexBuffer
(
	m_IndexList = #(),
	
	fn AddIndex index =
	(
		append m_IndexList index
	),
	
	fn WriteToFile =
	(
		local l_NumIds = m_IndexList.count
		WriteShort file num_face #unsigned
		for iIndex = 1 to l_NumIds do
		(
			WriteShort file m_IndexList[iIndex] #unsigned
		)
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
	m_VB = SVertexBuffer()
)

struct SMesh
(
	m_SubMeshes = #(),
	m_AABB = SAABB(),
	m_BS = SBndSphere(),
    m_MaterialsMap = #(dotnetobject "System.Collections.Generic.Dictionary`2[System.Int16,System.Int16]"),
	
	fn InitFromMaxObject Obj =
	(
		CreateSubMeshes Obj
		CreateBuffers Obj
	),
    
    fn CreateSubMeshes Obj =
	(
    	local l_Material = Obj.material
		if classof( l_Material ) == Multimaterial then
		(
			--format "Saving multimaterial object...\n"
			local l_NumMaterials = getNumfaces l_Material
            
			for iMaterial=1 to l_NumMaterials do
			(
            	try
            	(
					local l_MaterialId = getFaceMatId Obj iMaterial
					
					-- Check if the material is not already added
        	        if (m_MaterialsMap.ContainsKey(l_MaterialId))==false then
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
						m_MaterialsMap.Add l_MaterialId l_MaterialId
					)
            	)
				catch
				(
					format "Error while exporting submaterial %.\n" l_MaterialId
				)
			)
            
			--format "Num mats: % v %\n" materials.count materials
		)
		else
		(
         	local l_NewSubMesh = SSubMesh()
            l_NewSubMesh.m_Material.FromMaxMaterial l_Material
			append m_SubMeshes l_NewSubMesh
		)
	),
	
	fn CreateBuffers Obj =
	(
		--format "Getting buffers...\n"
		local l_nFaces = getNumFaces Obj
		
		local InvTransform=inverse Obj.transform
		for iFaces = 1  to l_nFaces do
		(
			--format "Parsing face %...\n" iFaces
			
			local l_MaterialId
			if classof(Obj.material) == Multimaterial then
			(
				l_MaterialId=getFaceMatID Obj iFaces
			)
			else
			(
				l_MaterialId = 1
			)
		
			-- Check if the material is found
			if (m_MaterialsMap.ContainsKey(l_MaterialId))==true then
			(
				-- Para coger los indices de la primera cara
				local IdxsFace=getFace Obj iFaces
				-- Para coger los vértices de la primera cara
				local Coord1=getVert Obj IdxsFace.x*InvTransform
				local Coord2=getVert Obj IdxsFace.y*InvTransform
				local Coord3=getVert Obj IdxsFace.z*InvTransform
				-- Para coger las normales de los vértices de la primera cara
				local Normal1=getNormal Obj IdxsFace.x
				local Normal2=getNormal Obj IdxsFace.y
				local Normal3=getNormal Obj IdxsFace.z
				/* Para coger los índices de las coordenadas de textura de la primera cara, el 1 señala al primer
				conjunto de coordenadas aplicadas sobre la cara */
				local IdxsMap = meshop.getMapFace Obj 1 iFaces
				-- Para coger las coordenadas de los vértices de la primera cara
				local TUVMap1= meshop.getMapVert Obj 1 IdxsMap.x
				local TUVMap2= meshop.getMapVert Obj 1 IdxsMap.y
				local TUVMap3= meshop.getMapVert Obj 1 IdxsMap.z
				
				local l_NewVertex1 = SVertex()
				l_NewVertex1.m_Position = Coord1
				l_NewVertex1.m_Normal = Normal1
				l_NewVertex1.m_UV = point2 TUVMap1.x 1.0-TUVMap1.y
				--l_NewVertex1.m_UV2 = point2 0 0,
				--l_NewVertex1.m_Color = 0,
				
				local NewVertex2 = SVertex()
				NewVertex2.m_Position = Coord2
				NewVertex2.m_Normal = Normal2
				NewVertex2.m_UV = point2 TUVMap2.x 1.0-TUVMap2.y
				--NewVertex2.m_UV2 = point2 0 0,
				--NewVertex2.m_Color = 0,
				
				local NewVertex3 = SVertex()
				NewVertex3.m_Position = Coord3
				NewVertex3.m_Normal = Normal3
				NewVertex3.m_UV = point2 TUVMap3.x 1.0-TUVMap3.y
				--NewVertex3.m_UV2 = point2 0 0,
				--NewVertex3.m_Color = 0,
				
				-- Add the new vertex
				local l_Id1 = m_SubMeshes[l_MaterialId].m_VB.AddVertex l_NewVertex1
				local l_Id2 = m_SubMeshes[l_MaterialId].m_VB.AddVertex l_NewVertex2
				local l_Id3 = m_SubMeshes[l_MaterialId].m_VB.AddVertex l_NewVertex3
				
				-- Add the new indexes
				m_SubMeshes[l_MaterialId].m_VB.AddIndex l_Id1
				m_SubMeshes[l_MaterialId].m_VB.AddIndex l_Id2
				m_SubMeshes[l_MaterialId].m_VB.AddIndex l_Id3
			)
		)
	),
	
	fn WriteToFile file =
	(
	)
)

