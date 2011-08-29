#include "stdafx.h"
#include "CloseSessionTask.h"

CloseSessionTask::CloseSessionTask(Camera* camera)
	: CameraTask(camera)
{
}

void CloseSessionTask::Process()
{
	/*
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSetPropertyData(inner_camera_, kEdsPropID_ImageQuality, 0, sizeof(EdsUInt32), &formerImageQuality);
	}
	*/
	EdsCameraRef edsCamera = camera_->GetInnerCamera();

	EdsUInt32 saveTo = kEdsSaveTo_Camera;
	EdsError errorCode = EdsSetPropertyData(edsCamera, kEdsPropID_SaveTo, 0, sizeof(saveTo) , &saveTo);


	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSetObjectEventHandler(edsCamera, kEdsObjectEvent_DirItemRequestTransfer, NULL, NULL);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsCloseSession(edsCamera);
	}
	CoUninitialize();
}