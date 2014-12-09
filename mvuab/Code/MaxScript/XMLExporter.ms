	--export de la class
	fn ExportMeshXMLFile AllObj filenameXML =
	(
		local ostream = fopen filenameXML "wt"

		WriteString  ostream "<static_meshes>" 
		for Obj in AllObj do
		(
			
			if ( classOf(Obj) ) != Editable_mesh then
			(
				format "Only Editable Meshes allowed...\n"
				return -1
			)
			
			--WriteShort file 0x55ff
			filename = Obj.material.diffuseMap.filename
			tag1 = "\n<static_mesh name=\"" + Obj.name + "\" filename=\"" + filename + "\"/>"
			WriteString ostream tag1
			
		)
		--Format "</static_meshes>" to:ostream
		WriteString ostream "</static_meshes>"
		fclose ostream
	)
	
	--export de la class
	fn ExportRenderableObjectXMLFile AllObj filenameXML =
	(
		local ostream = fopen filenameXML "wt"

		WriteString  ostream "<RenderableObjects>" 
		for Obj in AllObj do
		(
			
			if ( classOf(Obj) ) != Editable_mesh then
			(
				format "Only Editable Meshes allowed...\n"
				return -1
			)
			
			YawAngle =GetYaw Obj.transform as string
			PitchAngle= GetPitch Obj.transform as string
			RollAngle= GetRoll Obj.transform as string
			PosX = Obj.Pos.x as string
			PosY = Obj.Pos.y as string
			PosZ = Obj.Pos.z as string
			
			
			Pos = stringStream ""
			format "%,%,%" PosX PosY PosZ to:Pos
			strPos = Pos as string
			
			ScaleStr = stringStream ""
			format "%,%,%" Obj.Scale.x Obj.Scale.y Obj.Scale.z to:ScaleStr
			strScale = ScaleStr as string
			
			tag1 = stringStream ""
			format "\n<MeshInstance name=\"%\" core=\"%\" pos=\"%\" yaw=\"%\"  pitch=\"%\" roll=\"%\" scale=\"%\"/>"  Obj.name Obj.name strPos YawAngle PitchAngle RollAngle strScale to:tag1
			strTag1 = tag1 as string
			
			WriteString ostream strTag1
			
		)
		WriteString ostream "</RenderableObjects>"
		fclose ostream
	)

	fn ExportTextureFile AllObj DestPath = 
	(
	
		for Obj in AllObj do
		(
			
			if ( classOf(Obj) ) != Editable_mesh then
			(
				format "Only Editable Meshes allowed...\n"
				return -1
			)
			
			filename = filenameFromPath Obj.material.diffuseMap.filename
			DestPath = DestPath + "/" + filename
			
			copyFile Obj.material.diffuseMap.filename DestPath
		)
		
		
	)
	
	--UTIL
	
	fn GetYaw transform =
	(
		return (transform as eulerangles).z
	)
	
	fn GetPitch transform =
	(
		return (transform as eulerangles).x
	)
	fn GetRoll transform =
	(
		return (transform as eulerangles).y
	)
	
	
	