fileIn "ExportMesh.ms"
fileIn "XMLExporter.ms"
fileIn "Cinematics.ms"
filein "RNMUtils.ms"
		
utility UABUtility "UAB Utility" width:163 height:600
(
	button SetPathButton "Set Path" pos:[13,9] width:127 height:30
	label PathLabel "" pos:[15,44] width:132 height:29
	checkbox ExportMeshCheckBox "Export Mesh" pos:[17,84] width:79 height:25
	checkbox CopyTexturesCheckBox "Copy textures" pos:[17,110] width:94 height:25
	dropdownList LevelList "" pos:[13,135] width:135 height:21
	GroupBox ExportGroup "Export" pos:[11,156] width:141 height:112
	button LightsButton "Lights" pos:[21,171] width:57 height:17
	button MeshButton "Mesh" pos:[85,171] width:57 height:17
	button CinematicsButton "Cinematics" pos:[83,193] width:57 height:17
	button CameraButon "Camera" pos:[21,193] width:57 height:17
	button StaticMeshManagerButton "Static Mesh Manager" pos:[19,218] width:124 height:17
	button SceneButton "Scene" pos:[18,241] width:124 height:17
	button SetProperties "Set Properties" pos:[37,568] width:82 height:17
	
	GroupBox grp9 "Generate RNM" pos:[10,286] width:141 height:112
		button Xbtn "Generate X" pos:[15,310] width:124 height:17
		button Ybtn "Generate Y" pos:[15,340] width:124 height:17
		button Zbtn "Generate Z" pos:[15,370] width:124 height:17


	GroupBox grp10 "Set RNM Texture" pos:[4,411] width:145 height:150
		button LightmapXBtn "X" pos:[10,434] width:32 height:17
		label LightmapXLabel "" pos:[46,433] width:94 height:20
		button LightmapYBtn "Y" pos:[10,464] width:32 height:17
		label LightmapYLabel "" pos:[46,465] width:94 height:20
		button LightmapZBtn "Z" pos:[10,494] width:32 height:17
		label LightmapZLabel "" pos:[46,491] width:94 height:20
		button ApplyBtn "Apply" pos:[23,530] width:124 height:17

	on UABUtility open do
	(
		local levels=#("","level 1","level 2", "level 3","level 4", "main menu", "select player")
		LevelList.items=levels
		PathLabel.text = "D:\Master"
	)
	on SetPathButton pressed do
	(
		local data_path=getSavePath caption:"Data path" initialDir:PathLabel.text
		if data_path!=undefined then
			PathLabel.text=data_path
	)
	on LightsButton pressed do
	(
		--export lights
		local LightDir = PathLabel.text + "\\lights.xml"		--poner parametrico
		ExportLights LightDir
	)
	
	on MeshButton pressed do
	(		
		if PathLabel.text == "" then
		(
			format "Destination Path undefined, please choose a path \n"
			return -1 
		)
		
		--export Mesh
		local MeshDir = PathLabel.text + "\\meshes"		--poner parametrico
		local TexturesDir = PathLabel.text + "\\textures"		--poner parametrico
		--Check if exist dir, if not mkdir
		DoesFolderExist MeshDir
		
		local UniqueInstanceArray = GetUniqueInstances()
		
		format "UniqueInstancesArray % \n" UniqueInstanceArray 
		
		if UniqueInstanceArray.count == 0 then 
		(
			format "Empty selection. Select any object \n"
			return -1
		)
		
		--poner parametrico
		if selection.count > 0 then 
		(
							
			for Obj in UniqueInstanceArray do
			(
				filename = MeshDir +"\\"+ Obj.name + ".mesh"--poner parametrico
				local file = fopen filename "wb"
				local l_ObjectMesh = SMesh()
				l_ObjectMesh.InitFromMaxObject Obj
				l_ObjectMesh.WriteToFile file
				fclose file
			)
			
			if CopyTexturesCheckBox.checked then
				(
					DoesFolderExist TexturesDir
					ExportTextureFile selection TexturesDir
				)
		)
		else
		(
			format "Empty selection. Select any object \n"
			return -1
		)
	)
	on CinematicsButton pressed do
	(		
		if PathLabel.text == "" then
		(
			format "Destination Path undefined, please choose a path \n"
			return -1 
		)
		
		CinematicsDir = PathLabel.text + "\\cinematics.xml"
		nFrames = 5
		
		ExportCinematicObjects nFrames CinematicsDir
	)
	on CameraButon pressed do
	(		
		if PathLabel.text == "" then
		(
			format "Destination Path undefined, please choose a path \n"
			return -1 
		)
		
		cameraDir = PathLabel.text + "\\camera.xml"
		nFrames = 5
		isCycle = 1
		isReverse = 0
		
		ExportKeyCamera nFrames isCycle isReverse cameraDir
	)
	on StaticMeshManagerButton pressed do
	(		
		if PathLabel.text == "" then
		(
			format "Destination Path undefined, please choose a path \n"
			return -1 
		)
		
		local rootDir = PathLabel.text --poner parametrico
		local fileSrcMeshXML = rootDir + "\\static_meshes.xml"
		local MeshDir = PathLabel.text + "\\meshes\\"
		local TexturesDir = PathLabel.text + "\\textures\\"
		
		select $*
	
		if fileSrcMeshXML != undefined then
		(
			DoesFolderExist MeshDir
			ExportStaticMeshXMLFile fileSrcMeshXML MeshDir
			format "StaticMeshXML created on: % \n" fileSrcMeshXML
		)
	
		if CopyTexturesCheckBox.checked then
		(
			DoesFolderExist TexturesDir
			ExportTextureFile geometry TexturesDir
		)
	
		--selec all objects
		if ExportMeshCheckBox.checked then
		(
			local UniqueInstanceArray = GetUniqueInstances()
		
			if UniqueInstanceArray.count == 0 then 
			(
				format "Empty selection. Select any object \n"
				return -1
			)
			
			for Obj in UniqueInstanceArray do
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
	)
	on SceneButton pressed do
	(		
		if PathLabel.text == "" then
		(
			format "Destination Path undefined, please choose a path \n"
			return -1 
		)
		
		local rootDir = PathLabel.text		--poner parametrico
		local fileSrcRenderableXML = rootDir + "\\renderable_objects.xml"
		local fileSrcMeshXML = rootDir + "\\static_meshes.xml"
		local MeshDir = PathLabel.text + "\\meshes\\"
		local TexturesDir = PathLabel.text + "\\textures\\"
		
		select $*
		
		--Check if exist dir, if not mkdir
		--DoesFolderExist rootDir
		
		if fileSrcMeshXML != undefined then
		(
			DoesFolderExist MeshDir
			ExportStaticMeshXMLFile fileSrcMeshXML MeshDir
			format "StaticMeshXML created on: % \n" fileSrcMeshXML
		)
		
		if fileSrcRenderableXML != undefined then
		(
			ExportRenderableObjectXMLFile geometry fileSrcRenderableXML
			format "RenderableObjectXML created on: % \n" fileSrcRenderableXML
		)
		
		--selec all objects
		if CopyTexturesCheckBox.checked then
		(
			DoesFolderExist TexturesDir
			ExportTextureFile geometry TexturesDir
		)
	
		--selec all objects
		if ExportMeshCheckBox.checked then
		(
			local UniqueInstanceArray = GetUniqueInstances()
		
			if UniqueInstanceArray.count == 0 then 
			(
				format "Empty selection. Select any object \n"
				return -1
			)
			
			for Obj in UniqueInstanceArray do
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
		
	)
	
	on Xbtn pressed  do
	(
		SetRNMAxis $ (color 231 127 201)
	)
	
	on Ybtn pressed  do
	(
		SetRNMAxis $ (color 75 217 201)
	)
	
	on Zbtn pressed  do
	(
		SetRNMAxis $ (color 75 37 201)
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
	on ApplyBtn pressed do
	(
		ApplyRNMToObj $
	)
	
	on SetProperties pressed do
	(
		createDialog UABSetProperties
	)
)
