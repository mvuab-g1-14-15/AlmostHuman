global VERTEX_TYPE_GEOMETRY 	= 0x0001
global VERTEX_TYPE_NORMAL 		= 0x0002
global VERTEX_TYPE_TANGENT 	 	= 0x0004
global VERTEX_TYPE_BINORMAL	 	= 0x0008
global VERTEX_TYPE_TEXTURE1 	= 0x0010
global VERTEX_TYPE_TEXTURE2 	= 0x0020
global VERTEX_TYPE_DIFFUSE 		= 0x0040
global VERTEX_TYPE_REFLECTION	= 0x0200
global VERTEX_TYPE_SKYBOX       = 0x1000
global VERTEX_TYPE_RNM       		= 0x2000

struct SVertex
(
	m_Type,
	m_Position = point3 0 0 0,
	m_Normal = point3 0 0 0,
	m_UV = point2 0 0,
	m_UV2 = point2 0 0,
	m_Binormal = point3 0 0 0,
	m_Tan = point3 0 0 0,
	m_Color = 0,
	
	fn GetAsString = 
	(
		local l_Text = stringstream ""
		--convert position
		format "% % % " m_Position.x m_Position.y m_Position.z to:l_Text
		--convert normal
		format "% % % " m_Normal.x m_Normal.y m_Normal.z to:l_Text
		--convert UV
		format "% % " m_UV.x m_UV.y to:l_Text
		--convert Color
		
		--format "% % % %" bit.and m_Color 0xFF000000 bit.and m_Color 0x00FF0000 bit.and m_Color 0x0000FF00 bit.and m_Color 0x000000FF 
		
		format "%" m_Color #unsigned to:l_Text
		format "%" (formattedprint m_Color format:"x") to:l_Text  
		return (l_Text as string)
	),
	
	fn WriteToFile file =
	(
		if ((bit.and m_Type VERTEX_TYPE_SKYBOX) == VERTEX_TYPE_SKYBOX) then
		(
			WriteFloat file m_Position.x
			WriteFloat file m_Position.z
			WriteFloat file m_Position.y
		)
		else
		(
			--format "Vertex info:\n"
			--format "\tType % \n" m_Type
			--format "\tPos: (%, %, %)\n" m_Position.x m_Position.z m_Position.y
			WriteFloat file m_Position.x
			WriteFloat file m_Position.z
			WriteFloat file m_Position.y
			
			--format "\tNormal: (%, %, %)\n" m_Normal.x m_Normal.z m_Normal.y
			WriteFloat file m_Normal.x
			WriteFloat file m_Normal.z
			WriteFloat file m_Normal.y
			
			
			if (bit.and m_Type (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) ) == (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) then
			(
				WriteFloat file 0
				
				--format "\tTangencial: (%, %, %)\n" m_Tan.x m_Tan.y m_Tan.z
				WriteFloat file m_Tan.x
				WriteFloat file m_Tan.y
				WriteFloat file m_Tan.z
				WriteFloat file 0
				
				--format "\tBinormal: (%, %, %)\n" m_Binormal.x m_Binormal.y m_Binormal.z
				WriteFloat file m_Binormal.x
				WriteFloat file m_Binormal.y
				WriteFloat file m_Binormal.z
				WriteFloat file 0
			)
			
			if ((bit.and m_Type VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 ) then 
			(
				--format "\tUV1: (%, %)\n" m_UV.x m_UV.y
				WriteFloat file m_UV.x
				WriteFloat file m_UV.y
			)
			
			if ((bit.and m_Type VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 ) then 
			(
				--format "\tUV2: (%, %)\n" m_UV2.x m_UV2.y
				WriteFloat file m_UV2.x
				WriteFloat file m_UV2.y
			)
			
			if ((bit.and m_Type VERTEX_TYPE_DIFFUSE) == VERTEX_TYPE_DIFFUSE) then
			(
				--format "\tColor (%) \n" (formattedprint m_Color format:"x")
				WriteLong file m_Color #unsigned
			)
		)
	)
)