#pragma once
#include <EDSDK.h>
#include <atlmisc.h>

class Image
{
friend class DownloadTask;
public:
	~Image(void);
	EdsImageRef GetPicure() { return image_ref_; }
	void SetPicture(EdsImageRef image);
private:
	Image();
	EdsImageRef image_ref_;
};
