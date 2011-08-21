#include "stdafx.h"
#include "Image.h"
#include <EDSDK.h>
#include "../exceptions/InvalidOperationException.h"

Image::Image()
	:image_ref_(NULL)
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
}