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
	button CameraButon "Camera" pos:[21,215] width:57 height:17
	button StaticMeshManagerButton "Static Mesh Manager" pos:[19,240] width:124 height:17
	button SceneButton "Scene" pos:[18,263] width:124 height:17
	button SetProperties "Set Properties" pos:[21,301] width:98 height:19
	
	--Local vars
	local ostream
	
	
	on UABUtility open do
	(
		local levels=#("","level 1","level 2", "level 3","level 4", "main menu", "select player")
		LevelList.items=levels
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
		fileIn "ExportMesh.ms"
		fileIn "XMLExporter.ms"
		
		--export Mesh
		local MeshDir = PathLabel.text + "\meshes"		--poner parametrico
		local TextureDir = PathLabel.text + "\\textures"
		local filenameXML = MeshDir + "\static_meshes2.xml"
		
		--selec all objects
		if ExportMeshCheckBox.checked then
		(
			local filename = MeshDir + "\mesh.mesh"--poner parametrico
			ExportMesh $ filename
		)
		
		--selec all objects
		if CopyTexturesCheckBox.checked then
		(
			ExportTextureFile allObj TextureDir
		)

		--TODO Loop only geometry
		select $*
		local allObj =#()
		append allObj $
		
		--ostream = GetSaveFileStream filenameXML
		if filenameXML != undefined then
		(
			ExportMeshXMLFile allObj filenameXML
		)
	)
	on CameraButon pressed do
	(
		--export camera
	)
	on StaticMeshManagerButton pressed do
	(
		
	)
	on SceneButton pressed do
	(
		fileIn "XMLExporter.ms"
		
		local rootDir = PathLabel.text		--poner parametrico
		local fileSrcRenderableXML = rootDir + "\\renderable_objects.xml"
		local fileSrcMeshXML = rootDir + "\static_meshes2.xml"
		
		select $*
		local allObj =#()
		append allObj $
		
		--ostream = GetSaveFileStream filenameXML
		if fileSrcMeshXML != undefined then
		(
			ExportMeshXMLFile allObj fileSrcMeshXML
		)
		
		if fileSrcRenderableXML != undefined then
		(
			ExportRenderableObjectXMLFile allObj fileSrcRenderableXML
		)
	)
	
	on SetProperties pressed do
	(
		--open UABSetProperties
	)
	
)


