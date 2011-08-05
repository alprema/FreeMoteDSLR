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
	EdsError errorCode = EdsOpenSession(inner_camera_);
	if (EDS_ERR_OK != errorCode)
		return NULL;
	EdsChar* productName = new EdsChar[32];
	errorCode = EdsGetPropertyData(inner_camera_, kEdsPropID_ProductName, 0, 32, productName);
	EdsCloseSession(inner_camera_);
	return new CString(productName);
}

// TODO: This callback system should be replaced by a 
// worker thread that handles events because it's handle by
// the UI thread at the moment
EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context)
{
	EdsDirectoryItemRef directoryItem = (EdsDirectoryItemRef)object;
	EdsDirectoryItemInfo directoryItemInfo;
	EdsError errorCode = EdsGetDirectoryItemInfo(directoryItem, &directoryItemInfo);

	EdsStreamRef memStream = NULL;
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsCreateMemoryStream(directoryItemInfo.size, &memStream);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsDownload(directoryItem, directoryItemInfo.size, memStream);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsDownloadComplete(directoryItem);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSeek(memStream, 0, kEdsSeek_Begin);
	}

	if (object)
		EdsRelease(object);
	Image* newPicture = (Image*)context;
	EdsImageRef newImage;
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsCreateImageRef(memStream, &newImage);
	}
	if (EDS_ERR_OK == errorCode)
	{
		newPicture->SetPicture(newImage); // Put the handlers in the class
	}
	
	if (NULL != memStream)
	{
		EdsRelease(memStream);
		memStream = NULL;
	}

	return errorCode;

}

// Takes a picture with the current camera and returns it.
// It will set a callback method and then wait for it to be called
// in order to make the call synchronous.
// The caller is in charge of freeing the image object
Image* Camera::TakePicture()
{
	Image* newPicture = new Image();
	EdsError errorCode = EdsOpenSession(inner_camera_);

	// Locking the UI
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSendStatusCommand(inner_camera_, kEdsCameraStatusCommand_UILock, 0);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSetObjectEventHandler(inner_camera_, kEdsObjectEvent_DirItemRequestTransfer, handleObjectEvent, newPicture);
	}

	if (EDS_ERR_OK == errorCode)
	{
		EdsUInt32 saveTo = kEdsSaveTo_Host;
		errorCode = EdsSetPropertyData(inner_camera_, kEdsPropID_SaveTo, 0, sizeof(saveTo) , &saveTo);
	}
	
	if (EDS_ERR_OK == errorCode)
	{
		EdsCapacity capacity = {0x7FFFFFFF, 0x1000, 1};
		errorCode = EdsSetCapacity(inner_camera_, capacity);
	}

	EdsUInt32 formerImageQuality;
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsGetPropertyData(inner_camera_, kEdsPropID_ImageQuality, 0, sizeof(EdsUInt32), &formerImageQuality);
	}
	if (EDS_ERR_OK == errorCode)
	{
		EdsUInt32 tempImageQuality = EdsImageQuality_LR;
		errorCode = EdsSetPropertyData(inner_camera_, kEdsPropID_ImageQuality, 0, sizeof(EdsUInt32), &tempImageQuality);
	}

	// Shooting
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSendCommand(inner_camera_, kEdsCameraCommand_TakePicture, 0);
	}

	// Waiting
	if (EDS_ERR_OK == errorCode)
	{
		while (!newPicture->is_ready_)
			Sleep(3000);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSetPropertyData(inner_camera_, kEdsPropID_ImageQuality, 0, sizeof(EdsUInt32), &formerImageQuality);
	}

	if (EDS_ERR_OK == errorCode)
	{
		EdsUInt32 saveTo = kEdsSaveTo_Camera;
		errorCode = EdsSetPropertyData(inner_camera_, kEdsPropID_SaveTo, 0, sizeof(saveTo) , &saveTo);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSetObjectEventHandler(inner_camera_, kEdsObjectEvent_DirItemRequestTransfer, NULL, NULL);
	}

	// Unlocking UI
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSendStatusCommand(inner_camera_, kEdsCameraStatusCommand_UIUnLock, 0);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsCloseSession(inner_camera_);
	}

	if (EDS_ERR_OK != errorCode)
	{
		delete(newPicture);
		return NULL;
	}
	return newPicture;
}


