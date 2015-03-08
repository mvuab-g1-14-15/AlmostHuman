rollout SetRNMRollout "Set RNM" width:320 height:152
(
	button LightmapXBtn "X" pos:[16,16] width:24 height:24
	label LightmapXLabel "" pos:[48,15] width:248 height:24
	
	button LightmapYBtn "Y" pos:[16,49] width:24 height:24	
	label LightmapYLabel "" pos:[48,48] width:248 height:24
	
	button LightmapZBtn  "Z" pos:[16,81] width:24 height:24
	label LightmapZLabel "" pos:[48,80] width:248 height:24
	
	button ApplyBtn "Apply" pos:[96,112] width:104 height:24
	function setLightmapOn Label =
	(
		local file=getOpenFileName caption:"Open lightmap" types:"TGA(*.tga)|*.tga|JPG(*.jpg)|*.jpg"
		if file!=undefined then
			Label.text=file
	)
	on LightmapXBtn pressed do
	(
		setLightmapOn LightmapXLabel
	)
	on LightmapYBtn pressed do
	(
		setLightmapOn LightmapYLabel
	)
	on LightmapZBtn pressed do
	(
		setLightmapOn LightmapZLabel
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
	on ApplyBtn pressed do
	(
		ApplyRNMToObj $
	)
)
--createDialog SetRNMRollout