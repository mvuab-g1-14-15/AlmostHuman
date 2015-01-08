--export_lights
fn ExportLights filenameXML =
(
	try 
	(
		local ostream=createfile filenameXML
		format "<lights>\n" to:ostream
		for EachLight in lights do
		(
			
			Pos = stringStream ""
			format "% % %" EachLight.Pos.x EachLight.Pos.z EachLight.Pos.y to:Pos
			strPos = Pos as string
			
			Colors = stringStream ""
			format "% % %" EachLight.rgb.red EachLight.rgb.green EachLight.rgb.blue to:Colors
			strColor = Colors as string
			
			if(classof EachLight == Omnilight) then 
			(
							
				/* <light name=" Light03" type="omni" pos="51.2215 0.0 38.0957" color="1.0 1.0 1.0" att_start_range="80.0" att_end_range="200.0"/>*/
				
				omni_tag = stringStream ""
				format "\t<light name=\"%\" type=\"%\" pos=\"%\" color=\"%\" att_start_range=\"%\" att_end_range=\"%\"/>\n" EachLight.name "omni" strPos strColor EachLight.farAttenStart EachLight.farAttenEnd to:omni_tag 
				str_light_tag = omni_tag  as string	
				
			)
			else if(classof EachLight == targetSpot) then
			(
				/*<light name="Light02" type="directional" pos="-31.3958 35.6993 -74.0128" dir="0.555733 -0.350478 0.753874" color="1.0 1.0 1.0" render_shadows="true" att_start_range="8000.0" att_end_range="20000.0"/>
				*/
				
				Direct=(EachLight.target.pos-EachLight.pos)
				Direction = stringStream ""
				format "% % %" EachLight.Direct.x EachLight.Direct.z EachLight.Direct.y to:Direction
				strDirection = Direction as string
				
				target_spot_tag = stringStream ""
				format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\"  color=\"%\" render_shadows=\"%\" att_start_range=\"%\" att_end_range=\"%\"/>\n" EachLight.name "spot" strPos strDirection strColor true EachLight.farAttenStart EachLight.farAttenEnd to:target_spot_tag 
				str_light_tag = target_spot_tag as string	
				
			)
			else if (classof EachLight == TargetDirectionallight) then
			(
			
				/*<light name="Light01" type="spot" pos="58.335 6.15266 85.1666" dir="-0.758065 -0.193092 -0.622939" color="0.223529 0.788235 1.0" angle="0.226893" fall_off="0.977384" att_start_range="28.0" att_end_range="40.0"/>*/
				
				Direct=(EachLight.target.pos-EachLight.pos)
				Direction = stringStream ""
				format "% % %" EachLight.Direct.x EachLight.Direct.z EachLight.Direct.y to:Direction
				strDirection = Direction as string
				
				target_dir_tag = stringStream ""
				format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\"  color=\"%\" angle=\"%\" fall_off=\"%\" att_start_range=\"%\" att_end_range=\"%\"/>\n" EachLight.name "directional" strPos strDirection strColor EachLight.hotspot EachLight.Falloff EachLight.farAttenStart EachLight.farAttenEnd to:target_dir_tag 
				str_light_tag = target_dir_tag as string	
			)
			
			format str_light_tag to:ostream
		)
			format "</lights>\n" to:ostream
			close ostream				
		
		format "Light XML created successfully: % \n" filenameXML
		)
	catch
	(
		format "Error generating light XML \n"
	)
	
)

ExportLights "D:\lights.xml"