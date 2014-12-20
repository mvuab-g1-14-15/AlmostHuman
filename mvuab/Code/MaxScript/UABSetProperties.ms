rollout UABSetProperties "UABSetProperties" width:509 height:351
(
	GroupBox RenderGroup "Render" pos:[16,16] width:226 height:280
	checkbox ExportCheckRender "Export" pos:[21,37] width:61 height:17
	dropdownList ExportListRender "" pos:[112,35] width:120 height:21 enabled:false items:#("instance_mesh", "animated_mesh")
	checkbox VisibleCheckRender "Visible" pos:[21,63] width:51 height:17
	checkbox LayerCheckRender "Layer" pos:[21,86] width:51 height:17
	dropdownList LayerListRender "" pos:[112,84] width:120 height:21 enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
	label AnimModelRender "Anim. model" pos:[21,120] width:64 height:18
	edittext AnimModelEditText "" pos:[112,120] width:120 height:19
	
	GroupBox PhysicsGroup "Physics" pos:[258,91] width:226 height:72
	checkbox GenerateCheckPhysics "Generate" pos:[262,110] width:66 height:13
	checkbox LayerCheckPhysics "Layer" pos:[262,138] width:62 height:13
	dropdownList GenerateListPhysics "" pos:[347,106] width:123 height:21 enabled:false items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape")
	dropdownList LayerListPhysics "" pos:[347,138] width:123 height:21 enabled:false
	
	GroupBox LogicGroup "Logic" pos:[258,17] width:223 height:69
	checkbox GenerateCheckLogic "Generate" pos:[261,35] width:66 height:13
	dropdownList GenerateListLogic "" pos:[347,31] width:123 height:21 enabled:false items:#("", "event_box", "event_sphere", "stairs", "grippable_edges", "edges_where_to_be_dropped")
	
	checkbox CameraVisibilityCheckBox "Don't test camera visibility" pos:[21,216] width:217 height:17
	checkbox WorldTransformCheckBox "Don't export world transform" pos:[21,240] width:217 height:17
	checkbox LinkedToBoneCheckBox "Linked to bone" pos:[21,184] width:91 height:17
	edittext edt4 "" pos:[112,184] width:120 height:19
	checkbox ExportToCinematicsChecbox "Export to cinematics" pos:[21,268] width:217 height:17
	label lbl7 "Technique" pos:[21,152] width:64 height:18
	edittext LineEditTechnique "" pos:[112,152] width:120 height:19
	checkbox chk42 "Show Normal" pos:[261,57] width:139 height:13
	GroupBox grp15 "Events" pos:[258,168] width:226 height:40
	checkbox GenerateEventsCheckBox "Generate" pos:[263,187] width:66 height:13
	dropdownList GenerateEventsComboBox "" pos:[347,183] width:123 height:21 enabled:false
	GroupBox GBTracks "Traks" pos:[257,211] width:226 height:40
	checkbox GenerateTracksCheckBox "Generate" pos:[264,228] width:66 height:13
	GroupBox GBCameras "Cameras" pos:[257,256] width:226 height:40
	checkbox CycleCameraCheckBox "Cycle" pos:[265,274] width:66 height:13
	checkbox ReverseCameraCheckBox "Reverse" pos:[350,270] width:123 height:21
	GroupBox GBLights "Lights" pos:[256,303] width:226 height:40
	checkbox ExportLightsCheckBox "Export" pos:[265,323] width:66 height:13
	
	button GetButton "Get" pos:[16,311] width:112 height:29
	button SetButton "Set" pos:[134,311] width:108 height:29
	--ENABLE OR DISABLE DROPLISTDOWN
	on ExportCheckRender changed theState do
	if (ExportCheckRender.checked)==true then
	(
		ExportListRender.enabled=true
	)
	else
	(
		ExportListRender.enabled=false
	)
	
	on LayerCheckRender changed theState do
	if (LayerCheckRender.checked)==true then
	(
		
		LayerListRender.enabled=true
	)
	else
	(
		LayerListRender.enabled=false
	)
	
	on GenerateCheckPhysics changed theState do
	if (GenerateCheckPhysics.checked)==true then
	(
		
		GenerateListPhysics.enabled=true
	)
	else
	(
		GenerateListPhysics.enabled=false
	)
	
	on LayerCheckPhysics changed theState do
	if (LayerCheckPhysics.checked)==true then
	(
		
		LayerListPhysics.enabled=true
	)
	else
	(
		LayerListPhysics.enabled=false
	)
	
	on GenerateCheckLogic changed theState do
	if (GenerateCheckLogic.checked)==true then
	(
		
		GenerateListLogic.enabled=true
	)
	else
	(
		GenerateListLogic.enabled=false
	)
	
	
	
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
		if aProperty!="" then
		(
			if aProperty!=undefined then
			(
				AnimModelEditText.text=EditText
			)
		)
		else
		(
			AnimModelEditText.text=""
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
		if EditText.text !="" then
		(
			setUserProp Obj Type EditText.text
		)
		else
		(
			setUserProp Obj Type ""
		)
	)
	
	fn SetProperties Obj=
	(
		if ((classof  Obj)==ObjectSet)==false then
		(
			--------RENDER---------
			--ADD/UPDATE PROPERTY EXPORT
			SetPropertyCheckList  Obj "export_render" ExportCheckRender ExportListRender
			--ADD/UPDATE PROPERTY VISIBLE
			SetPropertyCheck Obj "visible_render" VisibleCheckRender
			--ADD/UPDATE PROPERTY LAYER
			SetPropertyCheckList Obj "layer_render" LayerCheckRender LayerListRender
			--ADD/UPDATE PROPERTYANIMATION MODEL
			SetPropertyEditText Obj"anim_model" AnimModelEditText
			--------PHYSICS---------
			--ADD/UPDATE PROPERTY GENERATE
			SetPropertyCheckList Obj "generate_physics" GenerateCheckPhysics GenerateListPhysics
			--ADD/UPDATE PROPERTY LAYER
			SetPropertyCheckList Obj "layer_physics" LayerCheckPhysics LayerListPhysics
			--------LOGIC---------
			--ADD/UPDATE PROPERTY GENERATE
			SetPropertyCheckList Obj "generate_logic" GenerateCheckLogic GenerateListLogic
			ObjAnterior=Obj
		)
		else
		(
			for i=1 to Obj.count do
			(
				--------RENDER---------
				--ADD/UPDATE PROPERTY EXPORT
				SetPropertyCheckList Obj[i] "export_render" ExportCheckRender ExportListRender
				--ADD/UPDATE VISIBLE
				SetPropertyCheck Obj[i] "visible_render"
				--ADD/UPDATE LAYER
				SetPropertyCheckList Obj[i] "layer_render" LayerCheckRender LayerListRender
				--ADD/UPDATE ANIMATE MODEL
				SetPropertyEditText Obj[i] "anim_model" AnimModelEditText
				--------PHYSICS---------
				--ADD/UPDATE GENERATE
				SetPropertyCheckList Obj[i] "generate_physics" GenerateCheckPhysics GenerateListPhysics
				--ADD/UPDATE LAYER
				SetPropertyCheckList Obj[i] "layer_physics" LayerCheckPhysics LayerListPhysics
				--------LOGIC---------
				--ADD/UPDATE GENERATE
				SetPropertyCheckList Obj[i] "generate_logic" GenerateCheckLogic GenerateListLogic
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
	on CameraVisibilityCheckBox changed theState do
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
	on WorldTransformCheckBox changed theState do
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
	on LinkedToBoneCheckBox changed theState do
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
	on ExportToCinematicsChecbox changed theState do
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
	on chk42 changed theState do
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
			if (GenerateCheckPhysics.checked)==true then
			(
				
				GenerateListPhysics.enabled=true
			)
			else
			(
				GenerateListPhysics.enabled=false
			)
		)
	on GetButton pressed do
	(
		local Property
		if ((classof $)==ObjectSet)==false then
		(
			--------RENDER---------
			--OBTAIN PROPERTY EXPORT
			Property=getUserProp $ "export_render"
			GetPropertyCheckList Property ExportCheckRender ExportListRender
			--OBTAIN PROPERTY VISIBLE
			Property=getUserProp $ "visible_render"
			GetPropertyCheckBox Property VisibleCheckRender
			--OBTAIN PROPERTY LAYER
			Property=getUserProp $ "layer_render"
			GetPropertyCheckList Property LayerCheckRender LayerListRender
			--OBTAIN PROPERTY ANIMATION MODEL
			Property=getUserProp $ "anim_model"
			GetPropertyEditText Property AnimModelEditText
			--------PHYSICS---------
			--OBTAIN GENERATE
			Property=getUserProp $ "generate_physics"
			GetPropertyCheckList Property GenerateCheckPhysics GenerateListPhysics
			--OBTAIN LAYER
			Property=getUserProp $ "layer_physics"
			GetPropertyCheckList Property LayerCheckPhysics LayerListPhysics
			--------LOGIC---------
			--OBTAIN GENERATE
			Property=getUserProp $ "generate_logic"
			GetPropertyCheckList Property GenerateCheckLogic GenerateListLogic
			
			ObjAnterior=$
		)
		else
		(
			for i=1 to $.count do
			(
				--------RENDER---------
				--OBTAIN PROPERTY EXPORT
				Property=getUserProp $[i] "export_render"
				GetPropertyCheckList Property ExportCheckRender ExportListRender
				--OBTAIN VISIBLE
				Property=getUserProp $[i] "visible_render"
				GetPropertyCheckBox Property VisibleCheckRender
				--OBTAIN LAYER
				Property=getUserProp $[i] "layer_render"
				GetPropertyCheckList Property LayerCheckRender LayerListRender
				--OBTAIN ANIMATE MODEL
				Property=getUserProp $[i] "anim_model"
				GetPropertyEditText Property AnimModelEditText
				--OBTAIN GENERATE
				Property=getUserProp $[i] "generate_physics"
				GetPropertyCheckList Property GenerateCheckPhysics GenerateListPhysics
				--------PHYSICS---------
				--OBTAIN LAYER
				Property=getUserProp $[i] "layer_physics"
				GetPropertyCheckList Property LayerCheckPhysics LayerListPhysics
				--------LOGIC---------
				--OBTAIN GENERATE
				Property=getUserProp $[i] "generate_logic"
				GetPropertyCheckList Property GenerateCheckLogic GenerateListLogic
				ObjAnterior=$[i]
			)
		)
	)
	on SetButton pressed do
	(
		format "El objeto es %\n" ObjAnterior
		if ObjAnterior!=undefined then
		(
			if ObjAnterior!=$ then
			(
				if (queryBox "Are the properties are of another object, you are sure assign it to another?")==true then
				(
					SetProperties $
				)
			)
		)
		else
		(
			SetProperties $
		)
		
	)
)
createDialog UABSetProperties