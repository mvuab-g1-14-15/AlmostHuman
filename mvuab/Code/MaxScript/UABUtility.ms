--fileIn "ExportMesh.ms"
fileIn "ExportMesh_v2.ms"
fileIn "XMLExporter.ms"
fileIn "Cinematics.ms"
		
utility UABUtility "UAB Utility" width:162 height:325
(
	button SetPathButton "Set Path" pos:[13,9] width:127 height:30
	label PathLabel "" pos:[15,49] width:132 height:52
	checkbox ExportMeshCheckBox "Export Mesh" pos:[17,106] width:79 height:25
	checkbox CopyTexturesCheckBox "Copy textures" pos:[17,132] width:94 height:25
	dropdownList LevelList "" pos:[13,157] width:135 height:21
	GroupBox ExportGroup "Export" pos:[11,178] width:141 height:112
	button LightsButton "Lights" pos:[21,193] width:57 height:17
	button MeshButton "Mesh" pos:[85,193] width:57 height:17
	button CinematicsButton "Cinematics" pos:[83,215] width:57 height:17
	button CameraButon "Camera" pos:[21,215] width:57 height:17
	button StaticMeshManagerButton "Static Mesh Manager" pos:[19,240] width:124 height:17
	button SceneButton "Scene" pos:[18,263] width:124 height:17
	button SetProperties "Set Properties" pos:[21,301] width:98 height:19

	on UABUtility open do
	(
		local levels=#("","level 1","level 2", "level 3","level 4", "main menu", "select player")
		LevelList.items=levels
		PathLabel.text = "C:\Users\Public\Documents\Dev\GameEngineUAB\mvuab\bin\Videogame\Data"
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
		--Check if exist dir, if not mkdir
		DoesFolderExist MeshDir
		
		--poner parametrico
		if selection.count > 0 then 
		(
			for Obj in selection do
			(
				filename = MeshDir +"\\"+ Obj.name + ".mesh"--poner parametrico
				local file = fopen filename "wb"
				local l_ObjectMesh = SMesh()
				l_ObjectMesh.InitFromMaxObject Obj
				l_ObjectMesh.WriteToFile file
				fclose file
			)
		)
		else
		(
			format "Empty selection. Select any object \n"
			return -1
		)
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
			format "Textures copied on: % \n" TexturesDir
		)
	
		--selec all objects
		if ExportMeshCheckBox.checked then
		(
			for Obj in geometry do
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
			format "Textures copied on: % \n" TexturesDir
		)
	
		--selec all objects
		if ExportMeshCheckBox.checked then
		(
			--poner parametrico
			for Obj in geometry do
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
	on SetProperties pressed do
	(
		fileIn "UABSetProperties.ms"
	)
	on btn8 pressed do
	(		
		if PathLabel.text == "" then
		(
			format "Destination Path undefined, please choose a path \n"
			return -1 
		)
		
		--export Mesh
		local MeshDir = PathLabel.text + "\\meshes"		--poner parametrico
		--Check if exist dir, if not mkdir
		DoesFolderExist MeshDir
		
		--poner parametrico
		if selection.count > 0 then 
		(
			for Obj in selection do
			(
				filename = MeshDir +"\\"+ Obj.name + ".mesh"--poner parametrico
				ExportMesh Obj filename
			)
		)
		else
		(
			format "Empty selection. Select any object \n"
			return -1
		)
	)
)


