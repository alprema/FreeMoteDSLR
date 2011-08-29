#include "stdafx.h"
#include "Camera.h"
#include "CameraManager.h"
#include "../exceptions/InvalidOperationException.h"
#include <EDSDK.h>

bool CameraManager::initialized_ = FALSE;

// Goes through the list of cameras connected to the computer
// and returns the first one of the list in a wrapper, and
// NULL if none is connected
Camera* CameraManager::GetCamera()
{
	EdsCameraListRef list = NULL;
	EdsError errorCode = EdsGetCameraList(&list);
	EdsUInt32 cameraCount = 0;
	if (EDS_ERR_OK == errorCode) 
	{
		errorCode = EdsGetChildCount(list, &cameraCount);	
	}
	EdsCameraRef camera;

	if (EDS_ERR_OK == errorCode && 0 != cameraCount)
	{
		errorCode = EdsGetChildAtIndex(list, 0, &camera);
	}
	if (NULL != list)
	{
		EdsRelease(list);
		list = NULL;
	}
	return EDS_ERR_OK == errorCode && 0 != cameraCount ? new Camera(camera) : NULL;
}

// Just calls the SDK inner methods for initialization
CameraManager::CameraManager(void)
{
	if (CameraManager::initialized_)
		throw new InvalidOperationException("Only one class of the manager can exist at a given time");
	CameraManager::initialized_ = TRUE;
	EdsInitializeSDK();
}

// Calls the SDK's termination methods
CameraManager::~CameraManager(void)
{
	EdsTerminateSDK();
	CameraManager::initialized_ = FALSE;
}