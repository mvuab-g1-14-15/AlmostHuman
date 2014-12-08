rollout UABSetProperties "UABSetProperties" width:264 height:406
(
	GroupBox RenderGroup "Render" pos:[19,19] width:227 height:112
	checkbox ExportCheckRender "Export" pos:[26,36] width:61 height:17
	dropdownList ExportListRender "" pos:[108,33] width:123 height:21 enabled:false items:#("instance_mesh", "animated_mesh")
	checkbox VisibleCheckRender "Visible" pos:[26,56] width:51 height:17
	checkbox LayerCheckRender "Layer" pos:[27,75] width:51 height:17
	dropdownList LayerListRender "" pos:[108,71] width:123 height:21 enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
	label AnimModelRender "Anim. model" pos:[32,104] width:64 height:18
	edittext AnimModelEditText "" pos:[108,101] width:120 height:19
	
	GroupBox PhysicsGroup "Physics" pos:[18,152] width:226 height:67
	checkbox GenerateCheckPhysics "Generate" pos:[29,167] width:66 height:13
	checkbox LayerCheckPhysics "Layer" pos:[30,194] width:62 height:13
	dropdownList GenerateListPhysics "" pos:[109,162] width:123 height:21 enabled:false items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape")
	dropdownList LayerListPhysics "" pos:[109,189] width:123 height:21 enabled:false
	
	GroupBox LogicGroup "Logic" pos:[20,230] width:223 height:69
	checkbox GenerateCheckLogic "Generate" pos:[29,248] width:66 height:13
	dropdownList GenerateListLogic "" pos:[109,244] width:123 height:21 enabled:false items:#("", "event_box", "event_sphere", "stairs", "grippable_edges", "edges_where_to_be_dropped")
	
	button GetButton "Get" pos:[14,369] width:77 height:29
	button SetButton "Set" pos:[171,369] width:77 height:29
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
	global ObjAnterior
	--GETBUTTON METHOD FOR PROPERTIES
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
	--GETBUTTON METHOD FOR PROPERTIES END
	
	--SETBUTTON METHOD TO UPDATE THE PROPERTIES
	on SetButton pressed do
	(
		if ObjAnterior!=$ then
		(
			if (queryBox "Are the properties are of another object, you are sure assign it to another?")==true then
			(
				SetProperties $
			)
		)
		else
		(
			SetProperties $
		)
		
	)
	---SETBUTTON METHOD TO UPDATE THE PROPERTIES END
)
createDialog UABSetProperties