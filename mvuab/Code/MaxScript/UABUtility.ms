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
filein "ExportLightProbes.ms"
		
utility UABUtility "UAB Utility" width:170 height:761
(
	button SetPathButton "Select Path" pos:[13,9] width:127 height:30
	label PathLabel "" pos:[15,44] width:132 height:29
	checkbox ExportMeshCheckBox "Export Mesh" pos:[17,84] width:79 height:25
	checkbox CopyTexturesCheckBox "Copy textures" pos:[17,110] width:94 height:25
	dropdownList LevelList "" pos:[13,135] width:135 height:21
	GroupBox ExportGroup "Export" pos:[11,156] width:141 height:221
	button LightsButton "Lights" pos:[21,171] width:57 height:17
	button MeshButton "Mesh" pos:[85,171] width:57 height:17
	button CinematicsButton "Cinematics" pos:[85,193] width:57 height:17
	button CameraButon "Camera" pos:[21,192] width:57 height:17
	button GraphButton "Graph" pos:[21,213] width:57 height:17
	button ExportLayersButton "ExportLayers" pos:[85,213] width:57 height:17
	button StaticMeshManagerButton "Static Mesh Manager" pos:[20,324] width:124 height:17
	button SceneButton "Scene" pos:[19,347] width:124 height:17
	
	GroupBox grp9 "Generate RNM" pos:[20,410] width:133 height:108
	button Xbtn "Generate X" pos:[22,430] width:124 height:17
	button Ybtn "Generate Y" pos:[23,460] width:124 height:17
	button Zbtn "Generate Z" pos:[23,490] width:124 height:17
	
	GroupBox SetRNM "Set RNM Texture" pos:[12,530] width:141 height:76
	button LightmapXBtn "X" pos:[22,554] width:34 height:17
	label LightmapXLabel "" pos:[48,553] width:94 height:20
	button LightmapYBtn "Y" pos:[67,554] width:34 height:17
	label LightmapYLabel "" pos:[48,585] width:94 height:20
	button LightmapZBtn "Z" pos:[107,554] width:34 height:17
	label LightmapZLabel "" pos:[48,611] width:94 height:20
	button ApplyBtn "Apply" pos:[22,580] width:120 height:17
	
	button SetProperties "Set Properties" pos:[16,381] width:141 height:17
	
	button TriggerButton "Triggers" pos:[20,237] width:57 height:17
	button ParticlesButton "Particles" pos:[84,237] width:57 height:17
	button PathButton "Paths" pos:[20,260] width:57 height:17
	button BillboardButton "Billboards" pos:[84,260] width:57 height:17
	button ExportEnemyPos "Enemies" pos:[24,283] width:57 height:17
	button LightprobeButton "Lightprobe" pos:[84,284] width:57 height:17
	
	/*on RouteButton pressed do
	(
		format " PathLabel % \n" PathLabel.text
		IsEmptyDestPath PathLabel.text
		
		RouteDir = PathLabel.text + "\\route.xml"
		
		local graphExporter = RouteAndGraph()
		graphExporter.Calculate()
		graphExporter.ExportRouteXML RouteDir
	)*/

	on UABUtility open do
	(
		local levels=#("","sala1","sala2","pasillo","sala3","elevator","sala4","space")
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
		ExportLights LightDir selection LevelList.selected
	)
	on LightprobeButton pressed do
	(
		--export lights
		local LightDir = PathLabel.text + "\\" +LevelList.selected + "\\light_probe.xml"
		lTest = ExportLightProbes $
		lTest.WriteToFile LightDir
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
		IsEmptyLevelList LevelList.selected
		
		local rootDir = PathLabel.text
		local l_DirToXML = rootDir + "\\" +LevelList.selected
		
		DoesFolderExist l_DirToXML
		
		CinematicsDir = l_DirToXML + "\\cinematics.xml"
		nFrames = 1
		
		ExportCinematicObjects nFrames CinematicsDir LevelList.selected
	)
	on CameraButon pressed do
	(		
		IsEmptyDestPath PathLabel.text
		IsEmptyLevelList LevelList.selected
		
		local rootDir = PathLabel.text
		local l_DirToXML = rootDir + "\\" +LevelList.selected
		
		cameraDir = l_DirToXML + "\\camera.xml"
		nFrames = 1
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
	on PathButton pressed do
	(
		format " PathLabel % \n" PathLabel.text
		IsEmptyDestPath PathLabel.text
		
		PathDir = PathLabel.text + "\\paths.xml"
		
		ExportPaths PathDir		
	)
	on BillboardButton pressed do
	(
		IsEmptyDestPath PathLabel.text
		PathDir = PathLabel.text + "\\billboards.xml"
		
		ExportBillboards PathDir		
	)
	on ExportEnemyPos pressed do
	(
		format " PathLabel % \n" PathLabel.text
		IsEmptyDestPath PathLabel.text
			
		ExportEnemyPositionToXML PathLabel.text		
	)
	on LightprobeBtn pressed do
	(
		format " PathLabel % \n" PathLabel.text
		IsEmptyDestPath PathLabel.text
			
		ExportEnemyPositionToXML PathLabel.text		
	)
)
