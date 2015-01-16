fileIn "GetAABB.ms"

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
	m_Texture1 = Texture(),
	m_Texture2  = Texture(),
	m_TextureBump  = Texture(),
	
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
	m_BndSphere = SBndSphere(),
	
	fn InitFromMaxObject Obj =
	(
		
	),
	
	fn WriteToFile file =
	(
		
	)
)

