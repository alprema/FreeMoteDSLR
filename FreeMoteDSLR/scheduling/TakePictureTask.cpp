#include "stdafx.h"
#include "TakePictureTask.h"
#include <EDSDK.h>


TakePictureTask::TakePictureTask(Camera* camera)
	: CameraTask(camera)
{
}

void TakePictureTask::Process()
{
	EdsError errorCode = EdsSendCommand(camera_->GetInnerCamera(), kEdsCameraCommand_TakePicture, 0);
}