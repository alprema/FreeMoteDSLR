#pragma once
#include "CameraTask.h"

class CloseSessionTask : public CameraTask
{
public:
	CloseSessionTask(Camera* camera);
	void Process();
};

