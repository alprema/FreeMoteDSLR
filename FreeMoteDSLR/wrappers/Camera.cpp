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

// Gets the name of the camera, returns CString or NULL
// if the name can't be retrieved
// The caller is in charge of freeing the CString
CString* Camera::GetProductName()
{
	// TODO: Replace the name retrieval method by a Task
	EdsError errorCode = EdsOpenSession(inner_camera_);
	if (EDS_ERR_OK != errorCode)
		return NULL;
	EdsChar* productName = new EdsChar[32];
	errorCode = EdsGetPropertyData(inner_camera_, kEdsPropID_ProductName, 0, 32, productName);
	EdsCloseSession(inner_camera_);
	return new CString(productName);
}