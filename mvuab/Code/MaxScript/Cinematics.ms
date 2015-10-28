--includes
fileIn "XMLStruct.ms"
fileIn "Utils.ms"

fn ExportKeyCamera nFrames isCycle isReverse filenameXML =
(
	try 
	(
		if cameras.count == 0 then
		(
			format "No cameras in the scene \n"
			return -1 
		)
		
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "camera"
		xDoc.AppendNode l_RootNode
				
		for EachCamera in cameras do
		(
			if(classof EachCamera == targetcamera or classof EachCamera == freecamera) then 
			(
				
				fov = EachCamera.Fov as string
				nearclip = EachCamera.nearclip as string
				farclip = EachCamera.farclip as string
				
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
					local l_NewCameraKeyControllerNode = xDoc.getNewNode "camera_key_controller"
			
					-- append the new node camera_key_controller to the root node camera
					l_RootNode.AppendNode l_NewCameraKeyControllerNode

					l_NewCameraKeyControllerNode.AddAttributeAsString "name" EachCamera.name
					l_NewCameraKeyControllerNode.AddAttributeAsString "cycle" isCycleStr
					l_NewCameraKeyControllerNode.AddAttributeAsString "reverse" isReverseStr
					l_NewCameraKeyControllerNode.AddAttributeAsString "total_time" totalTimeStr
					
					--sorting keys of controller
					sortkeys EachCamera.controller

					--loop each animation from 0f
					for t = animationRange.start to animationRange.end by nFrames do
					(
						animate on
						(
							at time t
							(
																			
									TimeInSeconds = stringStream ""
									format "%" (((t as float) / ticksPerFrame) / framerate) to:TimeInSeconds
									strTime = TimeInSeconds as string

									--<key time="0.0" pos="-26.8859 9.80159 -6.46455" look_at="-20.3781 3.19479 8.47801" fov="0.602417" near_plane="1.0" far_plane="1000.0"/>
									local l_NewCameraKeyTagNode = xDoc.getNewNode "key"
			
									-- append the new node key to node camera_key_controller
									l_NewCameraKeyControllerNode.AppendNode l_NewCameraKeyTagNode 

									l_NewCameraKeyTagNode.AddAttributeAsString "time" strTime
									l_NewCameraKeyTagNode.AddAttributeCoordinate "pos" EachCamera.Pos
									
									if (classof EachCamera == targetcamera) then
										l_NewCameraKeyTagNode.AddAttributeCoordinate "look_at" EachCamera.Target.Pos
									
									if (classof EachCamera == freecamera) then
										l_NewCameraKeyTagNode.AddAttributeCoordinate "look_at" (EachCamera.pos - EachCamera.dir)
									
									local lUpVector = EachCamera.transform[2]
									l_NewCameraKeyTagNode.AddAttributeCoordinate "up_vector" (lUpVector)
								
									l_NewCameraKeyTagNode.AddAttributeAsString "fov" fov
									l_NewCameraKeyTagNode.AddAttributeAsString "near_plane" nearclip
									l_NewCameraKeyTagNode.AddAttributeAsString "far_plane" farclip
							)
						)
					)	
				)
				else 
				(
					--<fixedcamera name="camera1" pos="-26.8859 9.80159 -6.46455" look_at="-20.3781 3.19479 8.47801" fov="0.602417" near_plane="1.0" far_plane="1000.0"/>
					local l_NewFixedCameraNode = xDoc.getNewNode "fixed_camera"
					
					-- append the new node fixedcamera to the root node camera
					l_RootNode.AppendNode l_NewFixedCameraNode 

					l_NewFixedCameraNode.AddAttributeAsString "name" EachCamera.name
					l_NewFixedCameraNode.AddAttributeCoordinate "pos" EachCamera.Pos
					
					if (classof EachCamera == targetcamera) then
						l_NewFixedCameraNode.AddAttributeCoordinate "look_at" EachCamera.Target.Pos
					
					if (classof EachCamera == freecamera) then
						l_NewFixedCameraNode.AddAttributeCoordinate "look_at" (EachCamera.pos - EachCamera.dir)
					
					l_NewFixedCameraNode.AddAttributeAsString "fov" fov
					l_NewFixedCameraNode.AddAttributeAsString "near_plane" nearclip
					l_NewFixedCameraNode.AddAttributeAsString "far_plane" farclip
				)			
			)
		)
	)
	catch
	(
		format "Error generating camera cinematic XML \n"
	)
	
	format "Camera XML created successfully: % \n" filenameXML
	return xDoc.PrettySaveXML()
)

fn ExportCinematicObjects nFrames filenameXML aRoom =
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
			local l_InitKeyframe = getUserProp Obj "Init_Keyframe" 
			
			if (l_InitKeyframe == undefined OR l_InitKeyframe == "") then
			(			
				l_InitKeyframe  = 0
			)

			--format "Obj name: % \n" Obj.name
			--format "InitKeyFrame % \n" l_InitKeyframe
			--format "InitKeyFrame to seconds % \n" strInitKeyFrame
			
			if (isCinematic == true) then 
			(
				
				InitKeyFrameStr = stringStream ""
				format "%" (l_InitKeyframe as float/ framerate) to:InitKeyFrameStr
				strInitKeyFrame = InitKeyFrameStr as string
				
				local l_NewCinematicObjectNode = xDoc.getNewNode "cinematic_object"
			
				-- append the new node cinematic_object to the root node cinematic
				l_RootNode.AppendNode l_NewCinematicObjectNode 
				lObjName = Obj.name +"_"+ aRoom; 
				
				l_NewCinematicObjectNode.AddAttributeAsString "resource" lObjName
				l_NewCinematicObjectNode.AddAttributeAsString "renderable_objects_manager" lObjName
				l_NewCinematicObjectNode.AddAttributeAsString "room" aRoom
				l_NewCinematicObjectNode.AddAttributeAsString "active" "false"
				l_NewCinematicObjectNode.AddAttributeAsString "init_keyframe" strInitKeyFrame
				
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

fn ExportCloneCinematicObjects nFrames filenameXML aRoom =
(
	--try
	--(
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
			local isCinematic = getUserProp Obj "export_to_cinematics"
			local l_InitKeyframe = getUserProp Obj "Init_Keyframe" 
						
			if (l_InitKeyframe == undefined OR l_InitKeyframe == "") then
			(			
				l_InitKeyframe  = 0
			)

			if (isCinematic == true) then 
			(
				
				InitKeyFrameStr = stringStream ""
				format "%" (l_InitKeyframe as float/ framerate) to:InitKeyFrameStr
				strInitKeyFrame = InitKeyFrameStr as string
				
				local l_CoreCinematics = getUserProp Obj "CoreObjCinematics"
				l_CorCinematicObj = execute ("$'"+l_CoreCinematics + "'")
				
				local l_NewCinematicObjectNode = xDoc.getNewNode "cinematic_object"
			
				-- append the new node cinematic_object to the root node cinematic
				l_RootNode.AppendNode l_NewCinematicObjectNode 
				lObjName = Obj.name +"_"+ aRoom; 
				
				l_NewCinematicObjectNode.AddAttributeAsString "resource" lObjName
				l_NewCinematicObjectNode.AddAttributeAsString "renderable_objects_manager" lObjName
				l_NewCinematicObjectNode.AddAttributeAsString "room" aRoom
				l_NewCinematicObjectNode.AddAttributeAsString "active" "false"
				l_NewCinematicObjectNode.AddAttributeAsString "init_keyframe" strInitKeyFrame
				
				for t = animationRange.start to animationRange.end by nFrames do
				(
					animate on
					(
						at time t
						(
							TimeInSeconds = stringStream ""
							format "%" (((t as float) / ticksPerFrame) / framerate) to:TimeInSeconds
							strTime = TimeInSeconds as string
								
							YawAngle =GetYaw l_CorCinematicObj.transform
							PitchAngle= GetPitch l_CorCinematicObj.transform
							RollAngle= GetRoll l_CorCinematicObj.transform

							--<cinematic_object_key_frame time="0.0333333" pos="1.63108 0.564489 10.0025" yaw="-1.19209e-007" pitch="0.0" roll="0.0" scale="1.0 1.0 1.0"/>
							local l_NewCinematicObjectKeyNode = xDoc.getNewNode "cinematic_object_key_frame"
							
							-- append the new node cinematic_key_object to the cinematic_object
							l_NewCinematicObjectNode.AppendNode l_NewCinematicObjectKeyNode 
							
							l_NewCinematicObjectKeyNode.AddAttributeAsString "time" strTime
							l_NewCinematicObjectKeyNode.AddAttributeCoordinate "pos" l_CorCinematicObj.Pos
							l_NewCinematicObjectKeyNode.AddAttributeFloat "yaw" YawAngle
							l_NewCinematicObjectKeyNode.AddAttributeFloat "pitch" PitchAngle
							l_NewCinematicObjectKeyNode.AddAttributeFloat "roll" RollAngle
							l_NewCinematicObjectKeyNode.AddAttributeCoordinate "scale" l_CorCinematicObj.Scale						
							
						)
					)
				)
			)

		)
		format "Cinematics clone XML created successfully: % \n" filenameXML
		return xDoc.PrettySaveXML()
	--)
	--catch
	--(
	--	format "Error generating clone cinematics XML \n"
--	)
)
