#pragma once
#include "CameraTask.h"
#include <EDSDK.h>

class DownloadTask : public CameraTask
{
public:
	DownloadTask(Camera* camera, EdsDirectoryItemRef directoryToDownload, HWND completeMessageDestination);
	~DownloadTask();
	void Process();
private:
	EdsDirectoryItemRef directory_to_download_;
	HWND complete_message_destination_;
};

