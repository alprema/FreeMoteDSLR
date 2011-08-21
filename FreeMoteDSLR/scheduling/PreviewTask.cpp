#include "stdafx.h"
#include "PreviewTask.h"
#include <EDSDK.h>
#include "../wrappers/StreamWrapper.h"

PreviewTask::PreviewTask(Image* image, BYTE* targetBuffer, long targetWidth, long targetHeight, HWND completeMessageDestination)
	:image_(image), target_buffer_(targetBuffer), target_width_(targetWidth), target_height_(targetHeight), complete_message_destination_(completeMessageDestination)
{
}

// Called when exiting process, will send a message to the main thread 
void PreviewTask::ExitProcess(bool success)
{
	PostMessage(complete_message_destination_, WM_PREVIEW_GENERATED, success, (LPARAM)image_);
}

// Writes the image bytes in the provided buffer at the desired size
// (be careful to respect ratio)
void PreviewTask::Process()
{
	EdsImageInfo imageInfo;
	EdsError err = EdsGetImageInfo(image_->GetPicure(), kEdsImageSrc_RAWFullView, &imageInfo);
	if (EDS_ERR_OK != err)
		return ExitProcess(false);
	EdsStreamRef jpegStream = NULL;
	err = EdsCreateMemoryStream(0, &jpegStream);
	if (EDS_ERR_OK != err)
		return ExitProcess(false);
	StreamWrapper jpegStreamWrapper(jpegStream); // Will release the stream on destruction
	EdsRect sourceRect = { { 0, 0 }, { imageInfo.width, imageInfo.height } };
	EdsSize destinationSize = { target_width_, target_height_ };
	err = EdsGetImage(image_->GetPicure(), kEdsImageSrc_RAWFullView, kEdsTargetImageType_RGB, sourceRect, destinationSize, jpegStream);
	if (EDS_ERR_OK != err)
		return ExitProcess(false);
	EdsUInt32 streamLength;
	err = EdsGetLength(jpegStream, &streamLength);
	if (EDS_ERR_OK != err)
		return ExitProcess(false);
	EdsVoid* jpegStreamPointer;
	err = EdsGetPointer(jpegStream, &jpegStreamPointer);
	if (EDS_ERR_OK != err)
		return ExitProcess(false);
	
	// Let's swap red and blue channels
	byte* jpegBytes = (byte*)jpegStreamPointer;
	for (int i = 0; i < target_width_ * target_height_; ++i)
	{
		*(target_buffer_++) = jpegBytes[2];
		*(target_buffer_++) = jpegBytes[1];
		*(target_buffer_++) = jpegBytes[0];
		jpegBytes += 3;
	}

	return ExitProcess(true);
}