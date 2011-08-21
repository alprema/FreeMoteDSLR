#include "stdafx.h"
#include "DownloadTask.h"

DownloadTask::DownloadTask(Camera* camera, EdsDirectoryItemRef directoryToDownload, HWND completeMessageDestination)
	: CameraTask(camera), directory_to_download_(directoryToDownload), complete_message_destination_(completeMessageDestination)
{
}

DownloadTask::~DownloadTask()
{
	// Check that it's called
	if (NULL != directory_to_download_)
		EdsRelease(directory_to_download_);
	directory_to_download_ = NULL;
}

// Posts an image to the main thread, the main thread is then in charge of deleting it
void DownloadTask::Process()
{
	EdsDirectoryItemInfo directoryItemInfo;
	EdsError errorCode = EdsGetDirectoryItemInfo(directory_to_download_, &directoryItemInfo);

	// Creating the memory stream
	EdsStreamRef memStream = NULL;
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsCreateMemoryStream(directoryItemInfo.size, &memStream);
	}

	// Downloading to it
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsDownload(directory_to_download_, directoryItemInfo.size, memStream);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsDownloadComplete(directory_to_download_);
	}

	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsSeek(memStream, 0, kEdsSeek_Begin);
	}

	EdsImageRef newImage;
	if (EDS_ERR_OK == errorCode)
	{
		errorCode = EdsCreateImageRef(memStream, &newImage);
	}
	
	Image* newPicture = new Image();
	if (EDS_ERR_OK == errorCode)
	{
		newPicture->SetPicture(newImage); // Put the handlers in the class
	}
	
	if (NULL != memStream)
	{
		EdsRelease(memStream);
		memStream = NULL;
	}

	// Signaling main thread that the download is over
	PostMessage(complete_message_destination_, WM_IMAGE_DOWNLOADED, NULL, (LPARAM)newPicture);
}