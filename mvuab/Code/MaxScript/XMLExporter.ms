fileIn "Utils.ms"
--export de la class
fn ExportStaticMeshXMLFile filenameXML MeshDir =
(	
	local ostream=createfile filenameXML
	local UniqueInstanceArray = GetUniqueInstances()
	
	if UniqueInstanceArray.count == 0 then 
	(
		format "Empty selection. Select any object \n"
		return -1
	)

	format "<static_meshes>\n" to:ostream

	for Obj in UniqueInstanceArray do
	(
			filename = "\\meshes\\" + Obj.name +".mesh"
			tag1 = "\t<static_mesh name=\"" + Obj.name + "\" filename=\"" + filename + "\"/>\n"
			format tag1 to:ostream
	)
	
	format "</static_meshes>\n" to:ostream
	close ostream
)

--export de la class
fn ExportRenderableObjectXMLFile AllObj filenameXML =
(
	try
	(
		local ostream=createfile filenameXML
		local unique_instances = GetUniqueInstances()
			
		if AllObj.count == 0 then 
		(
			format "Empty selection. Select any object \n"
			return -1
		)
		
		format "<RenderableObjects>\n" to:ostream
		
		for Obj in AllObj do
		(
			local instance_obj;
			if findItem unique_instances Obj != 0 then
				instance_obj = Obj
			else
			(
				local references = GetNodeReferences Obj
				if (references.count > 0) then
				(
					for ref_obj in references do
					(
						if findItem unique_instances ref_obj != 0 then
							instance_obj = ref_obj
					)
				)
				else
				(
					instance_obj = Obj
				)
			)

			ObjectName = Obj.name as string
		
			YawAngle =GetYaw Obj.transform as string
			PitchAngle= GetPitch Obj.transform as string
			RollAngle= GetRoll Obj.transform as string
			
			PosX = Obj.Pos.x as string
			PosY = Obj.Pos.z as string
			PosZ = Obj.Pos.y as string
			
			Pos = stringStream ""
			format "% % %" PosX PosY PosZ to:Pos
			strPos = Pos as string
			
			ScaleStr = stringStream ""
			format "% % %" Obj.Scale.x Obj.Scale.y Obj.Scale.z to:ScaleStr
			strScale = ScaleStr as string
			
			tag1 = stringStream ""
			format "\t<MeshInstance name=\"%\" core=\"%\" pos=\"%\" yaw=\"%\"  pitch=\"%\" roll=\"%\" scale=\"%\"/>\n" ObjectName instance_obj.name strPos YawAngle PitchAngle RollAngle strScale to:tag1
			strTag1 = tag1 as string
			
			format strTag1 to:ostream		
		)
		
		format "</RenderableObjects>\n" to:ostream
		close ostream
	)
	catch
	(
		format "Error generating renderable object\n"
	)	
)

fn ExportTextureFile AllObj DestPath = 
(

	if AllObj.count == 0 then 
	(
		format "Empty selection. Select any object \n"
		return -1
	)

	for Obj in AllObj do
	(
		try
		(
			maxOps.cloneNodes Obj cloneType:#instance newNodes:&newObj #nodialog
			convertToMesh newObj
			actualObj = newObj[1]

			filename = filenameFromPath actualObj.material.diffuseMap.filename
			DestPath = DestPath + "/" + filename
			
			copyFile Obj.material.diffuseMap.filename DestPath
		)
		catch
		(
			format "Error while exporting object\n"
		)
		delete newObj
	)		
)