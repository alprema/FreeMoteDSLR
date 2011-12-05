#pragma once
#include "CameraTask.h"
#include <EDSDK.h>

class DownloadTask : public CameraTask
{
public:
	// The downloadTarget is optional, the image won't be saved to disk if not set
	// downloadTarget will be copied if specified
	DownloadTask(Camera* camera, EdsDirectoryItemRef directoryToDownload, HWND completeMessageDestination, CString* downloadTarget = NULL);
	~DownloadTask();
	void Process();
private:
	EdsDirectoryItemRef directory_to_download_;
	HWND complete_message_destination_;
	CString* download_target_;
};

