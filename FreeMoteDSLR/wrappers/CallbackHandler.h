#pragma once
#include "../scheduling/TaskRunner.h"
#include "Camera.h"
#include <EDSDK.h>

class CallbackHandler
{
public:
	CallbackHandler(Camera* camera, TaskRunner* taskRunner, HWND completeMessageDestination);
	// Callback called by the EDS framework for incoming events
	static EdsError EDSCALLBACK HandleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context);
private:
	Camera* camera_;
	TaskRunner* task_runner_;
	HWND complete_message_destination_;
};

