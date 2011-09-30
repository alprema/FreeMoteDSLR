#pragma once
#include "../scheduling/TaskRunner.h"
#include "Camera.h"
#include <EDSDK.h>

class CallbackHandler
{
public:
	CallbackHandler(Camera* camera, TaskRunner* taskRunner, HWND completeMessageDestination);
	// Callback called by the EDS framework for incoming objects
	static EdsError EDSCALLBACK HandleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context);
	// Callback called by the EDS framework for incoming property events
	static EdsError EDSCALLBACK HandlePropertyEvent(EdsPropertyEvent event, EdsPropertyID cameraProperty, EdsUInt32 inParam, EdsVoid* context);
private:
	Camera* camera_;
	TaskRunner* task_runner_;
	HWND complete_message_destination_;
};

