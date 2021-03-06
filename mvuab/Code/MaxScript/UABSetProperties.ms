rollout UABSetProperties "UABSetProperties" width:600 height:800
(
	GroupBox RenderGroup "Render" pos:[47,16] width:226 height:300
	checkbox ExportCheckRender "Export" pos:[50,37] width:61 height:17
	dropdownList ExportListRender "" pos:[141,35] width:120 height:21 enabled:false items:#("instance_mesh", "animated_mesh")
	checkbox VisibleCheckRender "Visible" pos:[50,63] width:51 height:17
	checkbox LayerCheckRender "Layer" pos:[50,86] width:51 height:17
	dropdownList LayerListRender "" pos:[141,84] width:120 height:21 enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles", "glow")
	label AnimModelRender "Anim. model" pos:[50,118] width:64 height:18
	edittext AnimModelEditText "" pos:[141,120] width:120 height:19
	
	GroupBox PhysicsGroup "Physics" pos:[340,90] width:226 height:72
	checkbox GenerateCheckPhysics "Generate" pos:[344,109] width:66 height:13
	checkbox LayerCheckPhysics "Layer" pos:[344,137] width:62 height:13
	dropdownList GenerateListPhysics "" pos:[429,105] width:123 height:21 enabled:false items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape")
	dropdownList LayerListPhysics "" pos:[429,137] width:123 height:21 enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
	
	GroupBox LogicGroup "Logic" pos:[340,16] width:223 height:69
	checkbox GenerateCheckLogic "Generate" pos:[343,34] width:66 height:13
	dropdownList GenerateListLogic "" pos:[429,30] width:123 height:21 enabled:false items:#("", "event_box", "event_sphere", "stairs", "grippable_edges", "edges_where_to_be_dropped")
	
	checkbox CameraVisibilityCheckBox "Don't test camera visibility" pos:[52,209] width:217 height:17
	checkbox WorldTransformCheckBox "Don't export world transform" pos:[52,227] width:217 height:17
	checkbox LinkedToBoneCheckBox "Linked to bone" pos:[50,184] width:91 height:17
	edittext LinkedToBoneEditText "" pos:[141,184] width:120 height:19
	checkbox ExportToCinematicsCheckbox "Export to cinematics" pos:[52,251] width:217 height:17
	label InitKeyframeEditText "Init Keyframe" pos:[54,289] width:75 height:18
	edittext InitKeyframeValue "" pos:[142,287] width:53 height:19
	label CinematicsCoreObjEditText "CinematicsCoreObj" pos:[53,269] width:95 height:18
	edittext CoreObjCinematicsValue "" pos:[148,266] width:124 height:19
	
	label lbl7 "Technique" pos:[50,152] width:64 height:18
	edittext LineEditTechnique "" pos:[141,152] width:120 height:19
	checkbox ShowNormalCheck "Show Normal" pos:[343,56] width:139 height:13
	GroupBox grp15 "Events" pos:[340,167] width:226 height:40
	checkbox GenerateEventsCheckBox "Generate" pos:[345,186] width:66 height:13
	dropdownList GenerateEventsComboBox "" pos:[429,182] width:123 height:21 enabled:false items:#("", "event_1", "event_2", "event_3", "event_4", "event_5")
	
	GroupBox GBTracks "Traks" pos:[339,210] width:226 height:40
	checkbox GenerateTracksCheckBox "Generate" pos:[346,227] width:66 height:13
	GroupBox GBCameras "Cameras" pos:[339,255] width:226 height:40
	checkbox CycleCameraCheckBox "Cycle" pos:[347,273] width:66 height:13
	checkbox ReverseCameraCheckBox "Reverse" pos:[404,269] width:68 height:21
	checkbox IsFixedCheckBox "Is Fixed" pos:[474,269] width:68 height:21
	
	--  LIGHTS	
	GroupBox GBLights "Lights" pos:[43,330] width:244 height:282
	
		--Shadow Map
	label lbl8 "ShadowMapFormatType" pos:[49,364] width:120 height:18
	edittext ShadowMapFormatType "" pos:[171,365] width:90 height:19
	label lbl9 "ShadowMapHeight" pos:[48,390] width:120 height:18
	edittext ShadowMapHeight "" pos:[170,390] width:90 height:19
	label lbl10 "ShadowMapWidth" pos:[48,417] width:120 height:18
	edittext ShadowMapWidth "" pos:[170,417] width:90 height:19
	checkbox ExportGenerateShadowMap "Generate Shadow Map" pos:[89,443] width:150 height:13

		--Static Shadow Map
	label lbl11 "ShadowMapWidth" pos:[48,470] width:120 height:18
	edittext StaticShadowMapFormatType "" pos:[170,470] width:90 height:19
	label lbl12 "StaticShadowMapHeight" pos:[48,496] width:120 height:18
	edittext StaticShadowMapHeight "" pos:[170,496] width:90 height:19
	label lbl13 "StaticShadowMapWidth" pos:[47,519] width:120 height:18
	edittext StaticShadowMapWidth "" pos:[169,519] width:90 height:19
	checkbox ExportGenerateStaticShadowMap "Generate Static Shadow Map" pos:[88,550] width:150 height:13
		
	checkbox ExportLightsCheckBox "no_export_lights" pos:[50,584] width:118 height:18
	
	--  TRIGGERS	
	GroupBox grp8 "Trigger" pos:[336,440] width:256 height:124
	edittext TriggerName "" pos:[435,457] width:120 height:19
	label lblTriggerName "Trigger_Name" pos:[363,460] width:68 height:18
	checkbox enter_event_chk "Enter Event" pos:[346,484] width:78 height:17
	edittext enter_event_func "" pos:[435,482] width:148 height:19
	checkbox stay_event_chk "Stay Event" pos:[346,509] width:91 height:17
	edittext stay_event_func "" pos:[436,509] width:145 height:19
	checkbox leave_event_chk "Leave Event" pos:[346,537] width:91 height:17
	edittext leave_event_func "" pos:[436,534] width:145 height:19
	
	-- COVERS
	GroupBox grp10 "Covers" pos:[337,306] width:226 height:40
	edittext CoverName "" pos:[429,322] width:120 height:19
	label lblCoverName "Cover_Name" pos:[347,324] width:64 height:18
	
	-- PATHS
	GroupBox grp11 "AI Paths" pos:[336,359] width:226 height:69
	edittext PathsName "" pos:[424,378] width:120 height:19
	label lblPathsName "Path_Name" pos:[346,377] width:64 height:18
	edittext PathsNumNode "" pos:[483,403] width:60 height:19
	label lbl28 "Node_Num (index of node)" pos:[346,404] width:129 height:18
	
	--Particles
	GroupBox grp12 "Particles" pos:[337,577] width:226 height:40
	edittext ParticleName "" pos:[428,593] width:120 height:19
	label lblParticle "Particle Core" pos:[346,595] width:64 height:18
	
	--Billlboards
	GroupBox grp13 "Billboards" pos:[337,630] width:226 height:40
	edittext BillboardName "" pos:[429,646] width:120 height:19
	label lblBillboard "Billboard Core" pos:[347,648] width:69 height:18
	
	
	button GetButton "Load" pos:[50,614] width:112 height:29
	button SetButton "Save" pos:[168,614] width:108 height:29
	
	--ENABLE OR DISABLE DROPLISTDOWN
	
	
	
	--ENABLE OR DISABLE DROPLISTDOWN END
	
	--GET PROPERTY AND UPDATE INTERFACE
	fn GetPropertyCheckList aProperty CheckBox List=
	(
		if aProperty!="" then
		(
			if aProperty!=undefined then
			(
				CheckBox.checked=true
				for i=1 to List.items.count do
				(
					if List.items[i]==aProperty then
					(
						List.selection=i
						List.enabled=true
					)
				)
			)
			else
			(
				CheckBox.checked=false
				List.selection=1
				List.enabled=false
			)
		)
		else
		(
			CheckBox.checked=false
			List.selection=1
			List.enabled=false
		)
	)
	fn GetPropertyCheckBox aProperty CheckBox=
	(
		if aProperty==true then
		(
			if aProperty!=undefined then
			(
				CheckBox.checked=true
			)
		)
		else
		(
			CheckBox.checked=false
		)
	)
	
	fn GetPropertyEditText aProperty EditText=
	(
		--format "AProperty: % \n" aProperty 
		if aProperty!="" then
		(
			if aProperty!=undefined then
			(
				EditText.text=aProperty as String
			)
		)
		else
		(
			EditText.text=""
		)
	)
	fn GetPropertyList aProperty List=
	(

		if aProperty!="" then
		(
			if aProperty!=undefined then
			(
				for i=1 to List.items.count do
				(
					if List.items[i]==aProperty then
					(
						List.selection=i
					)
				)
			)
			else
			(
				List.selection=1
			)
		)
		else
		(
			List.selection=1
		)
	)
	--GET PROPERTY AND UPDATE INTERFACE END
	--SET PROPERTY IN SELECTED ITEMS
	fn SetPropertyCheckList Obj Type Check List=
	(
		if Check.checked==true then
		(
			setUserProp Obj Type List.items[List.selection]
		)
		else
		(
			setUserProp Obj Type ""
		)
	)
	
	fn SetPropertyCheck Obj Type Check=
	(
		format "I'm obj: % type: % and check: % " Obj.name Type Check
		if Check.checked==true then
		(
			setUserProp Obj Type true
		)
		else
		(
			setUserProp Obj Type false
		)
	)
	
	fn SetPropertyEditText Obj Type EditText=
	(
		
		--format "Objeto % Type % EditText % \n" Obj Type EditText.text
		
		if EditText.text !="" then
		(
			setUserProp Obj Type EditText.text
		)
		else
		(
			setUserProp Obj Type ""
		)
	)
	
	fn SetPropertyList Obj Type List=
	(
		setUserProp Obj Type List.items[List.selection]
	)
	fn SetEachProperties Obj=
	(
		if ((classof  Obj)==ObjectSet)==false then
		(
			--------RENDER---------
			--ADD/UPDATE EXPORT CHECKBOX
			SetPropertyCheck Obj "export_check_render" ExportCheckRender
			--ADD/UPDATE PROPERTY EXPORT
			SetPropertyCheckList Obj "export_render" ExportCheckRender ExportListRender
			--ADD/UPDATE VISIBLE
			SetPropertyCheck Obj "visible_render" VisibleCheckRender
			--ADD/UPDATE LAYER CHECKBOX
			SetPropertyCheck Obj "layer_check_render" LayerCheckRender
			--ADD/UPDATE LAYER
			SetPropertyCheckList Obj "layer_render" LayerCheckRender LayerListRender
			--ADD/UPDATE ANIMATE MODEL
			SetPropertyEditText Obj "anim_model" AnimModelEditText
			--ADD/UPDATE TECHNIQUE
			SetPropertyEditText Obj "technique" LineEditTechnique
			--ADD/UPDATE LINKED CHECKBOX
			SetPropertyCheck Obj "linked_to_bone_check" LinkedToBoneCheckBox
			--ADD/UPDATE LINKED
			SetPropertyEditText Obj "linked_to_bone" LinkedToBoneEditText
			--ADD/UPDATE PROPERTY CAMERA VISIBILITY
			SetPropertyCheck Obj "camera_visibility" CameraVisibilityCheckBox
			--ADD/UPDATE PROPERTY WORLD TRANSFORM
			SetPropertyCheck Obj "world_transform" WorldTransformCheckBox
			--ADD/UPDATE EXPORT CINEMATICS CHECKBOX
			SetPropertyCheck Obj "export_to_cinematics" ExportToCinematicsCheckbox
			--------PHYSICS---------
			--ADD/UPDATE GENERATE CHECKBOX
			SetPropertyCheck Obj "generate_check_physics" GenerateCheckPhysics
			--ADD/UPDATE GENERATE
			SetPropertyCheckList Obj "generate_physics" GenerateCheckPhysics GenerateListPhysics
			--ADD/UPDATE LAYER CHECKBOX
			SetPropertyCheck Obj "layer_check_physics" LayerCheckPhysics
			--ADD/UPDATE LAYER
			SetPropertyCheckList Obj "layer_physics" LayerCheckPhysics LayerListPhysics
			--------LOGIC---------
			--ADD/UPDATE GENERATE CHECKBOX
			SetPropertyCheck Obj "generate_check_logic" GenerateCheckLogic
			--ADD/UPDATE GENERATE
			SetPropertyCheckList Obj "generate_logic" GenerateCheckLogic GenerateListLogic
			--ADD/UPDATE PROPERTY SHOW NORMAL
			SetPropertyCheck Obj "show_normal" ShowNormalCheck
			--------EVENTS---------
			--ADD/UPDATE PROPERTY GENERATE EVENTS CHECKBOX
			SetPropertyCheck Obj "generate_events_check" GenerateEventsCheckBox
			--ADD/UPDATE PROPERTY GENERATE EVENTS
			SetPropertyCheckList Obj "generate_events" GenerateEventsCheckBox GenerateEventsComboBox
			--------TRAKS---------
			--ADD/UPDATE PROPERTY GENERATE TRACKS
			SetPropertyCheck Obj "generate_tracks" GenerateTracksCheckBox
			--------CAMERAS---------
			--ADD/UPDATE PROPERTY CYCLE CAMERA
			SetPropertyCheck Obj "cycle_camera" CycleCameraCheckBox
			--ADD/UPDATE PROPERTY REVERSE CAMERA
			SetPropertyCheck Obj "reverse_camera" ReverseCameraCheckBox
			SetPropertyCheck Obj "is_fixed_camera" IsFixedCheckBox
			
			
			
			
			--------LIGHTS---------
				--Shadow Map SET 
				SetPropertyEditText Obj "Shadow_Map_Format_Type" ShadowMapFormatType
				SetPropertyEditText Obj "Shadow_Map_Height" ShadowMapHeight
				SetPropertyEditText Obj "Shadow_Map_Width" ShadowMapWidth
				SetPropertyCheck Obj "export_shadow_map" ExportGenerateShadowMap
				
				-- Static Shadow Map SET 
				SetPropertyEditText Obj "Static_Shadow_Map_Format_Type" StaticShadowMapFormatType
				SetPropertyEditText Obj "Static_Shadow_Map_Height" StaticShadowMapHeight
				SetPropertyEditText Obj "Static_Shadow_Map_Width" StaticShadowMapWidth
				SetPropertyCheck Obj "export_static_shadow_map" ExportGenerateStaticShadowMap
				
				--ADD/UPDATE PROPERTY EXPORT LIGHTS
				SetPropertyCheck Obj "no_export_lights" ExportLightsCheckBox
				
			SetPropertyEditText Obj "Trigger_Name" TriggerName
			SetPropertyCheck Obj "Enter_Event" enter_event_chk
			SetPropertyCheck Obj "Stay_Event" stay_event_chk
			SetPropertyCheck Obj "Leave_Event" leave_event_chk
			SetPropertyEditText Obj "Enter_Event_Function" enter_event_func
			SetPropertyEditText Obj "Stay_Event_Function" stay_event_func
			SetPropertyEditText Obj "Leave_Event_Function" leave_event_func

			SetPropertyEditText Obj "Cover_Name" CoverName
			SetPropertyEditText Obj "Path_Name" PathsName
			SetPropertyEditText Obj "Path_Num_Node" PathsNumNode
			
			SetPropertyEditText Obj "Particle_Core" ParticleName
			SetPropertyEditText Obj "Billboard_Core" BillboardName
			
			SetPropertyEditText Obj "Init_Keyframe" InitKeyframeValue
			SetPropertyEditText Obj "CoreObjCinematics" CoreObjCinematicsValue
			
			
			
				
			ObjAnterior=Obj
		)
		else
		(
			for i=1 to Obj.count do
			(
				--------RENDER---------
				--ADD/UPDATE EXPORT CHECKBOX
				SetPropertyCheck Obj[i] "export_check_render" ExportCheckRender
				--ADD/UPDATE PROPERTY EXPORT
				SetPropertyCheckList Obj[i] "export_render" ExportCheckRender ExportListRender
				--ADD/UPDATE VISIBLE
				SetPropertyCheck Obj[i] "visible_render" VisibleCheckRender
				--ADD/UPDATE LAYER CHECKBOX
				SetPropertyCheck Obj[i] "layer_check_render" LayerCheckRender
				--ADD/UPDATE LAYER
				SetPropertyCheckList Obj[i] "layer_render" LayerCheckRender LayerListRender
				--ADD/UPDATE ANIMATE MODEL
				SetPropertyEditText Obj[i] "anim_model" AnimModelEditText
				--ADD/UPDATE TECHNIQUE
				SetPropertyEditText Obj[i] "technique" LineEditTechnique
				--ADD/UPDATE LINKED CHECKBOX
				SetPropertyCheck Obj[i] "linked_to_bone_check" LinkedToBoneCheckBox
				--ADD/UPDATE LINKED
				SetPropertyEditText Obj[i] "linked_to_bone" LinkedToBoneEditText
				--ADD/UPDATE PROPERTY CAMERA VISIBILITY
				SetPropertyCheck Obj[i] "camera_visibility" CameraVisibilityCheckBox
				--ADD/UPDATE PROPERTY WORLD TRANSFORM
				SetPropertyCheck Obj[i] "world_transform" WorldTransformCheckBox
				--ADD/UPDATE EXPORT CINEMATICS CHECKBOX
				SetPropertyCheck Obj[i] "export_to_cinematics" ExportToCinematicsCheckbox
				--------PHYSICS---------
				--ADD/UPDATE GENERATE CHECKBOX
				SetPropertyCheck Obj[i] "generate_check_physics" GenerateCheckPhysics
				--ADD/UPDATE GENERATE
				SetPropertyCheckList Obj[i] "generate_physics" GenerateCheckPhysics GenerateListPhysics
				--ADD/UPDATE LAYER CHECKBOX
				SetPropertyCheck Obj[i] "layer_check_physics" LayerCheckPhysics
				--ADD/UPDATE LAYER
				SetPropertyCheckList Obj[i] "layer_physics" LayerCheckPhysics LayerListPhysics
				--------LOGIC---------
				--ADD/UPDATE GENERATE CHECKBOX
				SetPropertyCheck Obj[i] "generate_check_logic" GenerateCheckLogic
				--ADD/UPDATE GENERATE
				SetPropertyCheckList Obj[i] "generate_logic" GenerateCheckLogic GenerateListLogic
				--ADD/UPDATE PROPERTY SHOW NORMAL
				SetPropertyCheck Obj[i] "show_normal" ShowNormalCheck
				--------EVENTS---------
				--ADD/UPDATE PROPERTY GENERATE EVENTS CHECKBOX
				SetPropertyCheck Obj[i] "generate_events_check" GenerateEventsCheckBox
				--ADD/UPDATE PROPERTY GENERATE EVENTS
				SetPropertyCheckList Obj[i] "generate_events" GenerateEventsCheckBox GenerateEventsComboBox
				--------TRAKS---------
				--ADD/UPDATE PROPERTY GENERATE TRACKS
				SetPropertyCheck Obj[i] "generate_tracks" GenerateTracksCheckBox
				--------CAMERAS---------
				--ADD/UPDATE PROPERTY CYCLE CAMERA
				SetPropertyCheck Obj[i] "cycle_camera" CycleCameraCheckBox
				--ADD/UPDATE PROPERTY REVERSE CAMERA
				SetPropertyCheck Obj[i] "reverse_camera" ReverseCameraCheckBox
				SetPropertyCheck Obj[i] "is_fixed_camera" IsFixedCheckBox
				
				--------LIGHTS---------
					--Shadow Map SET 
					SetPropertyEditText Obj[i] "Shadow_Map_Format_Type" ShadowMapFormatType
					SetPropertyEditText Obj[i] "Shadow_Map_Height" ShadowMapHeight
					SetPropertyEditText Obj[i] "Shadow_Map_Width" ShadowMapWidth
					SetPropertyCheck Obj[i] "export_shadow_map" ExportGenerateShadowMap
					
					-- Static Shadow Map SET 
					SetPropertyEditText Obj[i] "Static_Shadow_Map_Format_Type" StaticShadowMapFormatType
					SetPropertyEditText Obj[i] "Static_Shadow_Map_Height" StaticShadowMapHeight
					SetPropertyEditText Obj[i] "Static_Shadow_Map_Width" StaticShadowMapWidth
					SetPropertyCheck Obj[i] "export_static_shadow_map" ExportGenerateStaticShadowMap
				
				--ADD/UPDATE PROPERTY EXPORT LIGHTS
				SetPropertyCheck Obj[i] "no_export_lights" ExportLightsCheckBox
			
				SetPropertyEditText Obj[i] "Trigger_Name" TriggerName
				SetPropertyCheck Obj[i] "Enter_Event" enter_event_chk
				SetPropertyCheck Obj[i] "Stay_Event" stay_event_chk
				SetPropertyCheck Obj[i] "Leave_Event" leave_event_chk
				SetPropertyEditText Obj[i] "Enter_Event_Function" enter_event_func
				SetPropertyEditText Obj[i] "Stay_Event_Function" stay_event_func
				SetPropertyEditText Obj[i] "Leave_Event_Function" leave_event_func

				SetPropertyEditText Obj[i] "Cover_Name" CoverName
				SetPropertyEditText Obj[i] "Path_Name" PathsName
				SetPropertyEditText Obj[i] "Path_Num_Node" PathsNumNode
				
				SetPropertyEditText Obj[i] "Particle_Core" ParticleName
				SetPropertyEditText Obj[i] "Billboard_Core" BillboardName
				
				SetPropertyEditText Obj[i] "Init_Keyframe" InitKeyframeValue
				SetPropertyEditText Obj[i] "CoreObjCinematics" CoreObjCinematicsValue
				
				ObjAnterior=Obj[i]
			)
		)
	)
	--SET PROPERTY IN SELECTED ITEMS END
	global ObjAnterior=undefined
	--GETBUTTON METHOD FOR PROPERTIES
	--GETBUTTON METHOD FOR PROPERTIES END
	
	--SETBUTTON METHOD TO UPDATE THE PROPERTIES
	---SETBUTTON METHOD TO UPDATE THE PROPERTIES END
	
	on ExportCheckRender changed theState do
	(
			if (ExportCheckRender.checked)==true then
			(
				ExportListRender.enabled=true
			)
			else
			(
				ExportListRender.enabled=false
			)
	)
	on LayerCheckRender changed theState do
	(
			if (LayerCheckRender.checked)==true then
			(
				LayerListRender.enabled=true
			)
			else
			(
				LayerListRender.enabled=false
			)
	)
	on GenerateCheckPhysics changed theState do
	(
			if (GenerateCheckPhysics.checked)==true then
			(
				GenerateListPhysics.enabled=true
			)
			else
			(
				GenerateListPhysics.enabled=false
			)
	)
	on LayerCheckPhysics changed theState do
	(
			if (LayerCheckPhysics.checked)==true then
			(				
				LayerListPhysics.enabled=true
			)
			else
			(
				LayerListPhysics.enabled=false
			)
	)
	on GenerateCheckLogic changed theState do
	(
			if (GenerateCheckLogic.checked)==true then
			(
				GenerateListLogic.enabled=true
			)
			else
			(
				GenerateListLogic.enabled=false
			)
	)
	on GenerateEventsCheckBox changed theState do
	(
			if (GenerateEventsCheckBox.checked)==true then
			(
				GenerateEventsComboBox.enabled=true
			)
			else
			(
				GenerateEventsComboBox.enabled=false
			)
	)
	on GetButton pressed do
	(
		local Property
		if ((classof $)==ObjectSet)==false then
		(
			--------RENDER---------
			--OBTAIN PROPERTY EXPORT CHECKBOX
			Property=getUserProp $ "export_check_render"
			GetPropertyCheckBox Property VisibleCheckRender
			--OBTAIN PROPERTY EXPORT
			Property=getUserProp $ "export_render"
			GetPropertyCheckList Property ExportCheckRender ExportListRender
			--OBTAIN PROPERTY VISIBLE
			Property=getUserProp $ "visible_render"
			GetPropertyCheckBox Property VisibleCheckRender
			--OBTAIN PROPERTY LAYER CHECKBOX
			Property=getUserProp $ "layer_check_render"
			GetPropertyCheckBox Property VisibleCheckRender
			--OBTAIN PROPERTY LAYER
			Property=getUserProp $ "layer_render"
			GetPropertyCheckList Property LayerCheckRender LayerListRender
			--OBTAIN PROPERTY ANIMATION MODEL
			Property=getUserProp $ "anim_model"
			GetPropertyEditText Property AnimModelEditText
			--OBTAIN LINE EDIT TECHNIQUE
			Property=getUserProp $ "technique"
			GetPropertyEditText Property LineEditTechnique
			--OBTAIN LINKED TO BONE
			Property=getUserProp $ "linked_to_bone_check"
			GetPropertyCheckBox Property LinkedToBoneCheckBox
			--OBTAIN edt4
			Property=getUserProp $ "linked_to_bone" 
			GetPropertyEditText Property LinkedToBoneEditText
			--OBTAIN CAMERA VISIBILITY
			Property=getUserProp $ "camera_visibility"
			GetPropertyCheckBox Property CameraVisibilityCheckBox
			--OBTAIN WORLD TRANSFORM
			Property=getUserProp $ "world_transform"
			GetPropertyCheckBox Property WorldTransformCheckBox
			--OBTAIN EXPORT TO CINEMATICS
			Property=getUserProp $ "export_to_cinematics"
			GetPropertyCheckBox Property ExportToCinematicsCheckbox
			--------PHYSICS---------
			--OBTAIN GENERATE CHECKBOX
			Property=getUserProp $ "generate_check_physics"
			GetPropertyCheckBox Property GenerateCheckPhysics
			--OBTAIN GENERATE
			Property=getUserProp $ "generate_physics"
			GetPropertyCheckList Property GenerateCheckPhysics GenerateListPhysics
			--OBTAIN GENERATE CHECKBOX
			Property=getUserProp $ "layer_check_physics"
			GetPropertyCheckBox Property LayerCheckPhysics
			--OBTAIN LAYER
			Property=getUserProp $ "layer_physics"
			GetPropertyCheckList Property LayerCheckPhysics LayerListPhysics
			--------LOGIC---------
			--OBTAIN GENERATE CHECKBOX
			Property=getUserProp $ "generate_check_logic"
			GetPropertyCheckBox Property GenerateCheckLogic
			--OBTAIN GENERATE
			Property=getUserProp $ "generate_logic"
			GetPropertyCheckList Property GenerateCheckLogic GenerateListLogic
			--OBTAIN SHOW NORMAL
			Property=getUserProp $ "show_normal"
			GetPropertyCheckBox Property ShowNormalCheck
			--------EVENTS---------
			--OBTAIN GENERATE EVENTS CHECKBOX
			Property=getUserProp $ "generate_events_check"
			GetPropertyCheckBox Property GenerateEventsCheckBox
			--OBTAIN GENERATE EVENTS
			Property=getUserProp $ "generate_events"
			GetPropertyCheckList Property GenerateEventsCheckBox GenerateEventsComboBox
			--------TRAKS---------
			--OBTAIN GENERATE TRACKS
			Property=getUserProp $ "generate_tracks"
			GetPropertyCheckBox Property GenerateTracksCheckBox
			--------CAMERAS---------
			--OBTAIN CYCLE CAMERA
			Property=getUserProp $ "cycle_camera"
			GetPropertyCheckBox Property CycleCameraCheckBox
			--OBTAIN REVERSE CAMERA
			Property=getUserProp $ "reverse_camera"
			GetPropertyCheckBox Property ReverseCameraCheckBox
			Property=getUserProp $ "is_fixed_camera"
			GetPropertyCheckBox Property IsFixedCheckBox
			
			--------LIGHTS---------
			--Shadow Map GET 
			Property=getUserProp $ "Shadow_Map_Format_Type" 
			GetPropertyEditText Property ShadowMapFormatType
			Property=getUserProp $ "Shadow_Map_Height" 
			GetPropertyEditText Property ShadowMapHeight
			Property=getUserProp $ "Shadow_Map_Width" 
			GetPropertyEditText Property ShadowMapWidth
			Property=getUserProp $ "export_shadow_map"
			GetPropertyCheckBox Property ExportGenerateShadowMap
			
			--Static Shadow Map GET 
			Property=getUserProp $ "Static_Shadow_Map_Format_Type" 
			GetPropertyEditText Property StaticShadowMapFormatType
			Property=getUserProp $ "Static_Shadow_Map_Height" 
			GetPropertyEditText Property StaticShadowMapHeight
			Property=getUserProp $ "Static_Shadow_Map_Width" 
			GetPropertyEditText Property StaticShadowMapWidth
			Property=getUserProp $ "export_static_shadow_map"
			GetPropertyCheckBox Property ExportGenerateStaticShadowMap
			
			--OBTAIN COVER NAME
			Property=getUserProp $ "no_export_lights"
			GetPropertyCheckBox Property ExportLightsCheckBox
						
			Property=getUserProp $ "Trigger_Name" 
			GetPropertyEditText Property TriggerName
			Property=getUserProp $ "Enter_Event"
			GetPropertyCheckBox Property enter_event_chk
			Property=getUserProp $ "Stay_Event"
			GetPropertyCheckBox Property stay_event_chk
			Property=getUserProp $ "Leave_Event"
			GetPropertyCheckBox Property leave_event_chk
			Property=getUserProp $ "Enter_Event_Function"
			GetPropertyEditText Property enter_event_func
			Property=getUserProp $ "Stay_Event_Function"
			GetPropertyEditText Property stay_event_func
			Property=getUserProp $ "Leave_Event_Function"
			GetPropertyEditText Property leave_event_func
	
			Property=getUserProp $ "Cover_Name" 
			GetPropertyEditText Property CoverName
			Property=getUserProp $ "Path_Name" 
			GetPropertyEditText Property PathsName
			Property=getUserProp $ "Path_Num_Node" 
			GetPropertyEditText Property PathsNumNode
			
			Property=getUserProp $ "Particle_Core" 
			GetPropertyEditText Property ParticleName
			Property=getUserProp $ "Billboard_Core" 	
			GetPropertyEditText Property BillboardName
	
			Property=getUserProp $ "Init_Keyframe" 	
			GetPropertyEditText Property InitKeyframeValue
			
			Property=getUserProp $ "CoreObjCinematics" 	
			GetPropertyEditText Property CoreObjCinematicsValue
				
			ObjAnterior=$
		)
		else
		(
			messageBox "You can only select an object"
		)
	)
	on SetButton pressed do
	(
		if ObjAnterior!=undefined then
		(
			if ObjAnterior!=$ then
			(
				if (queryBox "Are the properties are of another object, you are sure assign it to another?")==true then
				(
					SetEachProperties $
				)
			)
			else
			(
				SetEachProperties $
			)
		)
		else
		(
			SetEachProperties $
		)
		
	)
)

createDialog UABSetProperties