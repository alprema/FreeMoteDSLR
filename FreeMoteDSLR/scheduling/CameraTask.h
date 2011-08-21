#pragma once
#include "Task.h"
#include "../wrappers/Camera.h"

// A camera related Task
class CameraTask : public Task
{
public:
	CameraTask(Camera* camera) : camera_(camera) {};
protected:
	Camera* camera_;
};