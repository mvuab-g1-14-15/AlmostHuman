--includes
fileIn "XMLStruct.ms"
fileIn "Utils.ms"

fn ExportKeyCamera nFrames isCycle isReverse filenameXML =
(
	try 
	(
		for EachCamera in cameras do
		(
			if(classof EachCamera == targetcamera) then 
			(
				local ostream=createfile filenameXML
				
				if (EachCamera.position.controller.keys.count > 0) then 
				(
					
					--If number of frames is empty, set to 1
					if nFrames == undefined then 
						nFrames = 1
					--If isCycle, set to 1 (true)
					if isCycle == undefined then 
						isCycle = 1
					--If isReverse, set to 0 (false)
					if isReverse == undefined then 
						isReverse = 0
					
					isCycleStr = isCycle as string
					isReverseStr = isReverse as string
					
					totalTimeStr = stringStream ""
					format "%" (((animationRange.end as float) / ticksPerFrame) / framerate) to:totalTimeStr
					strTotalTime = totalTimeStr as string
					
					--<camera_key_controller cycle="1" reverse="0" total_time="100.0">
					camera_key_controller_tag = "<camera_key_controller name=\"" + EachCamera.name + "\" cycle=\"" + isCycleStr + "\" reverse=\"" + isReverseStr + "\" total_time=\"" + totalTimeStr  + "\">\n"
					format camera_key_controller_tag to:ostream
					
					--sorting keys of controller
					sortkeys EachCamera.controller

					--loop each animation from 0f
					for t = animationRange.start to animationRange.end by nFrames do
					(
						animate on
						(
							at time t
							(
					
									fov = EachCamera.fov as string
									nearclip = EachCamera.nearclip as string
									farclip = EachCamera.farclip as string	
														
									TimeInSeconds = stringStream ""
									format "%" (((t as float) / ticksPerFrame) / framerate) to:TimeInSeconds
									strTime = TimeInSeconds as string
									
									Pos = stringStream ""
									format "% % %" EachCamera.Pos.x EachCamera.Pos.y EachCamera.Pos.z to:Pos
									strPos = Pos as string
				
									Look_at = stringStream ""
									format "% % %" EachCamera.Target.Pos.X EachCamera.Target.Pos.Y EachCamera.Target.Pos.Z to:Look_at
									strLookAt = Look_at as string

									--<key time="0.0" pos="-26.8859 9.80159 -6.46455" look_at="-20.3781 3.19479 8.47801" fov="0.602417" near_plane="1.0" far_plane="1000.0"/>
									key_tag = stringStream ""
									format "\t<key time=\"%\" pos=\"%\" look_at=\"%\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" strTime strPos strLookAt fov nearclip farclip to:key_tag
									strKeyTag = key_tag as string					
									
									format strKeyTag to:ostream		
								
							)
						)
					)	
					format "</camera_key_controller>\n" to:ostream
				)
				else 
				(
					--<fixedcamera name="camera1" pos="-26.8859 9.80159 -6.46455" look_at="-20.3781 3.19479 8.47801" fov="0.602417" near_plane="1.0" far_plane="1000.0"/>
					
					Pos = stringStream ""
					format "% % %" EachCamera.Pos.x EachCamera.Pos.y EachCamera.Pos.z to:Pos
					strPos = Pos as string

					Look_at = stringStream ""
					format "% % %" EachCamera.Target.Pos.X EachCamera.Target.Pos.Y EachCamera.Target.Pos.Z to:Look_at
					strLookAt = Look_at as string
				
					fov = EachCamera.Fov as string
					nearclip = EachCamera.nearclip as string
					farclip = EachCamera.farclip as string

					--<key time="0.0" pos="-26.8859 9.80159 -6.46455" look_at="-20.3781 3.19479 8.47801" fov="0.602417" near_plane="1.0" far_plane="1000.0"/>
					fixed_camera_tag = stringStream ""
					format  "<fixed_camera name=\"%\" pos=\"%\" look_at=\"%\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" EachCamera.name strPos strLookAt fov nearclip farclip to:fixed_camera_tag 
					strFixedCamera = fixed_camera_tag  as string
					format strFixedCamera to:ostream		
				)
				close ostream				
			)
		)
		format "Camera XML created successfully: % \n" filenameXML
	)
	catch
	(
		format "Error generating camera XML \n"
	)
	
)

fn ExportCinematicObjects nFrames filenameXML =
(
	try
	(
		totalTimeStr = stringStream ""
		format "%" (((animationRange.end as float) / ticksPerFrame) / framerate) to:totalTimeStr
		strTotalTime = totalTimeStr as string
			
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "cinematic"
		xDoc.AppendNode l_RootNode

		l_RootNode.AddAttributeAsString "name"		maxFileName
		l_RootNode.AddAttributeAsString "duration" 	totalTimeStr

		for Obj in geometry do
		(
			isCinematic = getUserProp Obj "export_to_cinematics"
			
			if (isCinematic == true) then 
			(
				
				local l_NewCinematicObjectNode = xDoc.getNewNode "cinematic_object"
			
				-- append the new node cinematic_object to the root node cinematic
				l_RootNode.AppendNode l_NewCinematicObjectNode 

				l_NewCinematicObjectNode.AddAttributeAsString "resource" Obj.name
				l_NewCinematicObjectNode.AddAttributeAsString "renderable_objects_manager" Obj.name
				
				
				for t = animationRange.start to animationRange.end by nFrames do
				(
					animate on
					(
						at time t
						(
							TimeInSeconds = stringStream ""
							format "%" (((t as float) / ticksPerFrame) / framerate) to:TimeInSeconds
							strTime = TimeInSeconds as string
								
							YawAngle =GetYaw Obj.transform
							PitchAngle= GetPitch Obj.transform
							RollAngle= GetRoll Obj.transform

							--<cinematic_object_key_frame time="0.0333333" pos="1.63108 0.564489 10.0025" yaw="-1.19209e-007" pitch="0.0" roll="0.0" scale="1.0 1.0 1.0"/>
							local l_NewCinematicObjectKeyNode = xDoc.getNewNode "cinematic_object_key_frame"
							
							-- append the new node cinematic_key_object to the cinematic_object
							l_NewCinematicObjectNode.AppendNode l_NewCinematicObjectKeyNode 
							
							l_NewCinematicObjectKeyNode.AddAttributeAsString "time" strTime
							l_NewCinematicObjectKeyNode.AddAttributeCoordinate "pos" Obj.Pos
							l_NewCinematicObjectKeyNode.AddAttributeFloat "yaw" YawAngle
							l_NewCinematicObjectKeyNode.AddAttributeFloat "pitch" PitchAngle
							l_NewCinematicObjectKeyNode.AddAttributeFloat "roll" RollAngle
							l_NewCinematicObjectKeyNode.AddAttributeCoordinate "scale" Obj.Scale						
							
						)
					)
				)
			)
		)
		format "Cinematics XML created successfully: % \n" filenameXML
		return xDoc.PrettySaveXML()
	)
	catch
	(
		format "Error generating cinematics XML \n"
	)
)