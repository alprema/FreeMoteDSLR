#pragma once
#include <EDSDK.h>
#include <atlmisc.h>
#include "Image.h"

// Wrapper class for SDK camera reference
class Camera
{
public:
	Camera(EdsCameraRef innerCamera);
	~Camera(void);
	// Gets the name of the camera
	CString* GetProductName();
	EdsCameraRef GetInnerCamera() { return inner_camera_; };
private:
	EdsCameraRef inner_camera_;
};
