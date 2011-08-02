#include "stdafx.h"
#include "Image.h"
#include <EDSDK.h>
#include "../exceptions/InvalidOperationException.h"

Image::Image()
	:is_ready_(false), image_ref_(NULL)
{
}

Image::~Image(void)
{
	EdsRelease(image_ref_);
	image_ref_ = NULL;
}

void Image::SetPicture(EdsImageRef image)
{
	if (NULL != image_ref_)
		throw new InvalidOperationException("The image was already set");
	image_ref_ = image;
	is_ready_ = true;
}

// Releases a stream and returns nothing
// TODO: Find a better way to collect it => RAII through wrapper / lambda
void Image::StreamReleaser(EdsStreamRef stream)
{
	EdsRelease(stream);
}

// Writes the image bytes in the provided buffer at the desired size
// (be careful to respect ratio)
void Image::GetImagePreview(byte* targetBuffer, int width, int height)
{
	EdsImageInfo imageInfo;
	EdsError err = EdsGetImageInfo(image_ref_, kEdsImageSrc_RAWFullView, &imageInfo);
	if (EDS_ERR_OK != err)
		return;
	EdsStreamRef jpegStream = NULL;
	err = EdsCreateMemoryStream(0, &jpegStream);
	if (EDS_ERR_OK != err)
		return;
	EdsRect sourceRect = { { 0, 0 }, { imageInfo.width, imageInfo.height } };
	EdsSize destinationSize = { width, height };
	err = EdsGetImage(image_ref_, kEdsImageSrc_RAWFullView, kEdsTargetImageType_RGB, sourceRect, destinationSize, jpegStream);
	if (EDS_ERR_OK != err)
		return StreamReleaser(jpegStream);
	EdsUInt32 streamLength;
	err = EdsGetLength(jpegStream, &streamLength);
	if (EDS_ERR_OK != err)
		return StreamReleaser(jpegStream);
	EdsVoid* jpegStreamPointer;
	err = EdsGetPointer(jpegStream, &jpegStreamPointer);
	if (EDS_ERR_OK != err)
		return StreamReleaser(jpegStream);
	
	// Let's swap red and blue
	byte* jpegBytes = (byte*)jpegStreamPointer;
	for (int i = 0; i < width * height; ++i)
	{
		*(targetBuffer++) = jpegBytes[2];
		*(targetBuffer++) = jpegBytes[1];
		*(targetBuffer++) = jpegBytes[0];
		jpegBytes += 3;
	}

	return StreamReleaser(jpegStream);
}