FREE_CAMERA = CameraType.Free.value;

function UpdateCameras()
	
	local lCameraType = camera_manager:GetCurrentCamera():GetCameraType();
	
	if( lCameraType == FREE_CAMERA ) then 
		UpdateFree()
	end
	
end