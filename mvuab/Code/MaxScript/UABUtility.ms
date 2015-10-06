fileIn "ExportMesh.ms"
fileIn "XMLExporter.ms"
fileIn "Cinematics.ms"
filein "RNMUtils.ms"
filein "ExportGraph.ms"
filein "Utils.ms"
filein "Triggers.ms"
filein "Paths.ms"
filein "Particles.ms"
filein "Billboards.ms"
		
utility UABUtility "UAB Utility" width:170 height:761
(
	button SetPathButton "Select Path" pos:[13,9] width:127 height:30
	label PathLabel "" pos:[15,44] width:132 height:29
	checkbox ExportMeshCheckBox "Export Mesh" pos:[17,84] width:79 height:25
	checkbox CopyTexturesCheckBox "Copy textures" pos:[17,110] width:94 height:25
	dropdownList LevelList "" pos:[13,135] width:135 height:21
	GroupBox ExportGroup "Export" pos:[11,156] width:141 height:171
	button LightsButton "Lights" pos:[21,171] width:57 height:17
	button MeshButton "Mesh" pos:[85,171] width:57 height:17
	button CinematicsButton "Cinematics" pos:[85,193] width:57 height:17
	button CameraButon "Camera" pos:[21,192] width:57 height:17
	button GraphButton "Graph" pos:[21,213] width:57 height:17
	button ExportLayersButton "ExportLayers" pos:[85,213] width:57 height:17
	button StaticMeshManagerButton "Static Mesh Manager" pos:[19,281] width:124 height:17
	button SceneButton "Scene" pos:[18,304] width:124 height:17
	
	GroupBox grp9 "Generate RNM" pos:[12,379] width:141 height:112
	button Xbtn "Generate X" pos:[17,403] width:124 height:17
	button Ybtn "Generate Y" pos:[17,433] width:124 height:17
	button Zbtn "Generate Z" pos:[17,463] width:124 height:17
	
	GroupBox SetRNM "Set RNM Texture" pos:[12,503] width:141 height:76
	button LightmapXBtn "X" pos:[22,527] width:34 height:17
	label LightmapXLabel "" pos:[48,526] width:94 height:20
	button LightmapYBtn "Y" pos:[67,527] width:34 height:17
	label LightmapYLabel "" pos:[48,558] width:94 height:20
	button LightmapZBtn "Z" pos:[107,527] width:34 height:17
	label LightmapZLabel "" pos:[48,584] width:94 height:20
	button ApplyBtn "Apply" pos:[22,553] width:120 height:17
	
	button SetProperties "Set Properties" pos:[13,342] width:141 height:17
	
	button TriggerButton "Triggers" pos:[20,237] width:57 height:17
	button ParticlesButton "Particles" pos:[84,237] width:57 height:17
	button PathButton "Paths" pos:[20,260] width:57 height:17
	button BillboardButton "Billboards" pos:[84,260] width:57 height:17
	
	
	on UABUtility open do
	(
		local levels=#("","sala1","sala2","pasillo","sala3","sala4")
		LevelList.items=levels
		PathLabel.text = "C:\Users\Galix\Desktop"
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
		local LightDir = PathLabel.text + "\\" +LevelList.selected + "\\lights.xml"
		ExportLights LightDir selection
	)
	on MeshButton pressed do
	(		
		IsEmptyDestPath PathLabel.text
	
		local l_DirToXML = PathLabel.text + "\\" +LevelList.selected
		--export Mesh
		local MeshDir = l_DirToXML + "\\meshes"
		local TexturesDir = PathLabel.text + "\\textures"		
	
		DoesFolderExist MeshDir
		
		local UniqueInstanceArray = GetUniqueInstances()
		IsEmptyInstanceArray UniqueInstanceArray
		
		if selection.count > 0 then 
		(
			GenerateMeshes UniqueInstanceArray MeshDir
			
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
		IsEmptyDestPath PathLabel.text
		
		CinematicsDir = PathLabel.text + "\\cinematics.xml"
		nFrames = 5
		
		ExportCinematicObjects nFrames CinematicsDir
	)
	on CameraButon pressed do
	(		
		IsEmptyDestPath PathLabel.text
		
		cameraDir = PathLabel.text + "\\camera.xml"
		nFrames = 5
		isCycle = 1
		isReverse = 0
		
		ExportKeyCamera nFrames isCycle isReverse cameraDir
	)
	on GraphButton pressed do
	(
		format " PathLabel % \n" PathLabel.text
		IsEmptyDestPath PathLabel.text
		
		GraphDir = PathLabel.text + "\\graph.xml"
		
		local graphExporter = RouteAndGraph()
		graphExporter.Calculate()
		graphExporter.ExportGraphXML GraphDir
	)
	/*on RouteButton pressed do
	(
		format " PathLabel % \n" PathLabel.text
		IsEmptyDestPath PathLabel.text
		
		RouteDir = PathLabel.text + "\\route.xml"
		
		local graphExporter = RouteAndGraph()
		graphExporter.Calculate()
		graphExporter.ExportRouteXML RouteDir
	)*/
	on StaticMeshManagerButton pressed do
	(		
		IsEmptyDestPath PathLabel.text
		IsEmptyLevelList LevelList.selected
		
		local rootDir = PathLabel.text --poner parametrico
		local l_DirToXML = rootDir + "\\" +LevelList.selected
		
		DoesFolderExist l_DirToXML
		
		local fileSrcMeshXML = l_DirToXML + "\\static_meshes.xml"
		local MeshDir = l_DirToXML + "\\meshes\\"
		local TexturesDir = PathLabel.text + "\\textures\\"
		local l_ExportableObjects = GetAllExportableObjects()
		
		select $*
	
		if fileSrcMeshXML != undefined then
		(
			DoesFolderExist MeshDir
			ExportStaticMeshXMLFile fileSrcMeshXML MeshDir LevelList.selected
			format "StaticMeshXML created on: % \n" fileSrcMeshXML
		)
	
		if CopyTexturesCheckBox.checked then
		(
			DoesFolderExist TexturesDir
			ExportTextureFile l_ExportableObjects TexturesDir
		)
	
		--selec all objects
		if ExportMeshCheckBox.checked then
		(
			local UniqueInstanceArray = GetUniqueInstancesScene()
			IsEmptyInstanceArray UniqueInstanceArray
			GenerateMeshes UniqueInstanceArray MeshDir
		)
	)
	
	on ExportLayersButton pressed do
	(
	
		IsEmptyDestPath PathLabel.text
		IsEmptyLevelList LevelList.selected
		
		local rootDir = PathLabel.text
		local l_DirToXML = rootDir + "\\" +LevelList.selected
		
		DoesFolderExist l_DirToXML
		
		
		local fileSrcMeshXML = l_DirToXML + "\\static_meshes.xml"
		local MeshDir = l_DirToXML + "\\meshes\\"
		local TexturesDir = PathLabel.text + "\\textures\\"
		
		local l_ExportableObjects = GetAllExportableObjects()
		--Export Renderable Objects
		local l_Layers = ExportLayerFile l_ExportableObjects l_DirToXML LevelList.selected
		ExportRenderableObjectXMLFile l_ExportableObjects l_DirToXML l_Layers LevelList.selected
		-- End Export Renderable Objects
	)
		
	on SceneButton pressed do
	(		
		IsEmptyDestPath PathLabel.text
		IsEmptyLevelList LevelList.selected
		
		local rootDir = PathLabel.text		--poner parametrico
		local l_DirToXML = rootDir + "\\" +LevelList.selected
		
		DoesFolderExist l_DirToXML
		
		local fileSrcMeshXML = l_DirToXML + "\\static_meshes.xml"
		local MeshDir = l_DirToXML + "\\meshes\\"
		local TexturesDir = PathLabel.text + "\\textures\\"
		
		local l_ExportableObjects = GetAllExportableObjects()
	
		select $*
	
		--Export Static Meshes
			DoesFolderExist MeshDir
			ExportStaticMeshXMLFile fileSrcMeshXML MeshDir LevelList.selected
			format "StaticMeshXML created on: % \n" fileSrcMeshXML
		-- End Export Static Meshes
		
		--Export Renderable Objects
			local l_Layers = ExportLayerFile l_ExportableObjects l_DirToXML LevelList.selected
			ExportRenderableObjectXMLFile l_ExportableObjects l_DirToXML l_Layers LevelList.selected
		-- End Export Renderable Objects
		
		if CopyTexturesCheckBox.checked then
		(
			DoesFolderExist TexturesDir
			ExportTextureFile l_ExportableObjects TexturesDir
		)
		
		if ExportMeshCheckBox.checked then
		(
			local UniqueInstanceArray = GetUniqueInstancesScene()
			IsEmptyInstanceArray UniqueInstanceArray
			
			GenerateMeshes UniqueInstanceArray MeshDir
		)
		
	)
	on Xbtn pressed do
	(
		SetRNMAxis $ (color 231 127 201)
	)
	on Ybtn pressed do
	(
		SetRNMAxis $ (color 75 217 201)
	)
	on Zbtn pressed do
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
		filein "UABSetProperties.ms"
	)
	on TriggerButton pressed do
	(
		format " PathLabel % \n" PathLabel.text
		IsEmptyDestPath PathLabel.text
		
		TriggerDir = PathLabel.text + "\\triggers.xml"
		
		ExportTriggers TriggerDir		
	)
	on ParticlesButton pressed do
	(
		IsEmptyDestPath PathLabel.text
		PathDir = PathLabel.text + "\\particules.xml"
		
		ExportParticles PathDir
	)
	
	on BillboardButton pressed do
	(
		IsEmptyDestPath PathLabel.text
		PathDir = PathLabel.text + "\\billboards.xml"
		
		ExportBillboards PathDir		
	)
	
	on PathButton pressed do
	(
		format " PathLabel % \n" PathLabel.text
		IsEmptyDestPath PathLabel.text
		
		PathDir = PathLabel.text + "\\paths.xml"
		
		ExportPaths PathDir		
	)

)
