#pragma once
#include "CameraTask.h"

class TakePictureTask : public CameraTask
{
public:
	TakePictureTask(Camera* camera);
	void Process();
};

