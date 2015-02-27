
	function SetRNMAxisMaterial material _color = 
	(
		material.bumpMap=Normal_Bump()
		material.bumpMapAmount=100
		material.bumpMapEnable=true
		material.bumpMap.method=3
		material.bumpMap.normal_map=Checker()
		material.bumpMap.normal_map.color1=_color
		material.bumpMap.normal_map.color2=_color
	)S

	function SetRNMAxis Obj color =
	(
		if (classof Obj.material)==Standardmaterial then
		(
			SetRNMAxisMaterial Obj.material color
		)
		else
		(
			for i=1 to Obj.material.count do
			(
				SetRNMAxisMaterial Obj.material[i] color
			)
		)
	)

	function setLightmapOn Label =
	(
		local file=getOpenFileName caption:"Open lightmap" types:"TGA(*.tga)|*.tga|JPG(*.jpg)|*.jpg"
		if file!=undefined then
			Label.text=file
	)

	function ApplyRNMToMaterial material lightmapXFilename lightmapYFilename lightmapZFilename= 
	(
		material.selfIllummap=Multi_Sub_Map()
		material.selfIllumMapEnable=true
		
		material.selfIllummap.id_0_color_connected=true
		material.selfIllummap.id_1_color_connected=true
		material.selfIllummap.id_2_color_connected=true
		
		material.selfIllummap.id_0_color_shader=BitmapTexture()
		material.selfIllummap.id_0_color_shader.bitmap=(openBitmap lightmapXFilename)
		material.selfIllummap.id_0_color_shader.coords.mapChannel=2
		
		material.selfIllummap.id_1_color_shader=BitmapTexture()
		material.selfIllummap.id_1_color_shader.bitmap=(openBitmap lightmapYFilename)
		material.selfIllummap.id_1_color_shader.coords.mapChannel=2
		
		material.selfIllummap.id_2_color_shader=BitmapTexture()
		material.selfIllummap.id_2_color_shader.bitmap=(openBitmap lightmapZFilename)
		material.selfIllummap.id_2_color_shader.coords.mapChannel=2
	)

	function ApplyRNMToObj  Obj = 
	(
		if (classof Obj.material)==standardMaterial then
		(
			ApplyRNMToMaterial Obj.material LightmapXLabel.text LightmapYLabel.text LightmapZLabel.text
		)
		else
		(
			for i=1 to Obj.material.count do
			(
				ApplyRNMToMaterial Obj.material[i] LightmapXLabel.text LightmapYLabel.text LightmapZLabel.text
			)
		)
	)

