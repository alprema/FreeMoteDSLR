#include "stdafx.h"
#include "OpenSessionTask.h"
#include "../wrappers/CallbackHandler.h"
#include <EDSDK.h>

OpenSessionTask::OpenSessionTask(Camera* camera, CallbackHandler* callbackHandler)
	: CameraTask(camera), callback_handler_(callbackHandler)
{
}

void OpenSessionTask::Process()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	EdsCameraRef edsCamera = camera_->GetInnerCamera();

	EdsError errorCode = EdsOpenSession(edsCamera);
	
	if (EDS_ERR_OK == errorCode)
	{
		// TODO: Change the handler to handle all events (and send them all there)
		errorCode = EdsSetObjectEventHandler(edsCamera, kEdsObjectEvent_DirItemRequestTransfer, CallbackHandler::HandleObjectEvent, callback_handler_);
	}

	if (EDS_ERR_OK == errorCode)
	{
		EdsUInt32 saveTo = kEdsSaveTo_Host;
		errorCode = EdsSetPropertyData(edsCamera, kEdsPropID_SaveTo, 0, sizeof(saveTo) , &saveTo);
	}
	
	if (EDS_ERR_OK == errorCode)
	{
		EdsCapacity capacity = {0x7FFFFFFF, 0x1000, 1};
		errorCode = EdsSetCapacity(edsCamera, capacity);
	}
	
	// TODO: Restore former image quality on session close
	/*
	EdsUInt32 formerImageQuality;
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsGetPropertyData(edsCamera, kEdsPropID_ImageQuality, 0, sizeof(EdsUInt32), &formerImageQuality);
	}
	*/
	if (EDS_ERR_OK == errorCode)
	{
		EdsUInt32 tempImageQuality = EdsImageQuality_LR;
		errorCode = EdsSetPropertyData(edsCamera, kEdsPropID_ImageQuality, 0, sizeof(EdsUInt32), &tempImageQuality);
	}
}