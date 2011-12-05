#include "stdafx.h"
#include "DownloadTask.h"

DownloadTask::DownloadTask(Camera* camera, EdsDirectoryItemRef directoryToDownload, HWND completeMessageDestination, CString* downloadTarget)
	: CameraTask(camera), directory_to_download_(directoryToDownload), complete_message_destination_(completeMessageDestination),
	  download_target_(NULL)
{
	if (NULL != downloadTarget)
		download_target_ = new CString(*downloadTarget);
}

DownloadTask::~DownloadTask()
{
	// TODO: Check that everything's called
	if (NULL != directory_to_download_)
		EdsRelease(directory_to_download_);
	if (NULL != download_target_)
		delete(download_target_);
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

	// Saving to disk if download_target_ is set
	if (NULL != download_target_)
	{
		EdsStreamRef fileStream;
		errorCode = EdsCreateFileStream(directoryItemInfo.szFileName, kEdsFileCreateDisposition_CreateAlways, kEdsAccess_Write, &fileStream);
		if (EDS_ERR_OK == errorCode)
		{
			errorCode = EdsCopyData(memStream, directoryItemInfo.size, fileStream);
		}
		if (EDS_ERR_OK == errorCode)
		{
			EdsRelease(fileStream);
		}
		if (EDS_ERR_OK == errorCode)
		{
			*download_target_ += "\\";
			*download_target_ += directoryItemInfo.szFileName;
			CString currentFileName(directoryItemInfo.szFileName);
			if (!MoveFile((LPCWSTR)currentFileName, (LPCWSTR)*download_target_))
				errorCode = EDS_ERR_FILE_IO_ERROR;
		}
		if (EDS_ERR_OK == errorCode)
		{
			errorCode = EdsSeek(memStream, 0, kEdsSeek_Begin);
		}
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