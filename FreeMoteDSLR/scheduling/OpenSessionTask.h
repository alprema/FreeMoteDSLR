#pragma once
#include "CameraTask.h"
#include "../wrappers/CallbackHandler.h"
#include "../wrappers/Camera.h"

class OpenSessionTask : public CameraTask
{
public:
	OpenSessionTask(Camera* camera, CallbackHandler* callbackHandler);
	void Process();
private:
	CallbackHandler* callback_handler_;
};