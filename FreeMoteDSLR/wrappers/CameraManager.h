#pragma once
#include "Camera.h"

// Manages communication with cameras
// Only one instance of this object must exist at a given time
class CameraManager
{
public:
	// Initializes the SDK environment
	CameraManager(void);

	~CameraManager(void);
	// Gets the camera curently connected to the computer
	// Returns NULL if none
	Camera* GetCamera();

private:
	// Number of instances of this class
	static int instances_count_;
};
