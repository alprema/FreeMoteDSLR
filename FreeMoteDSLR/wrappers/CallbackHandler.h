#pragma once
#include "../scheduling/TaskRunner.h"
#include "Camera.h"
#include <EDSDK.h>

class CallbackHandler
{
public:
	CallbackHandler(Camera* camera, TaskRunner* taskRunner, HWND completeMessageDestination);
	~CallbackHandler();
	// Sets the new download target (set to NULL if the pictures should not be saved on disk)
	// The CallbackHandler will free this string when it is done using it
	void SetDownloadTarget(CString* downloadTarget);
	// Callback called by the EDS framework for incoming objects
	// An instance of a CallbackHandler shall be passed in context
	static EdsError EDSCALLBACK HandleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context);
	// Callback called by the EDS framework for incoming property events
	// An instance of a CallbackHandler shall be passed in context
	static EdsError EDSCALLBACK HandlePropertyEvent(EdsPropertyEvent event, EdsPropertyID cameraProperty, EdsUInt32 inParam, EdsVoid* context);
private:
	Camera* camera_;
	TaskRunner* task_runner_;
	HWND complete_message_destination_;
	CString* download_target_;
};

