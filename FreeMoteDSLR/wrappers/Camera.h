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
	// Takes a picture with the current camera and
	// returns an in-memory picture
	Image* TakePicture();
private:
	EdsCameraRef inner_camera_;
	//EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context);
};
