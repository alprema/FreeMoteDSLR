#include "stdafx.h"
#include "Camera.h"
#include <EDSDK.h>
#include <sstream>
#include <atlmisc.h>
Camera::Camera(EdsCameraRef innerCamera)
	:inner_camera_(innerCamera)
{
}

// Releases inner SDK references using SDK ref counting
Camera::~Camera(void)
{
	EdsRelease(inner_camera_);
	inner_camera_ = NULL;
}