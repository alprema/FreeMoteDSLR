#pragma once
#include <EDSDK.h>
#include <atlmisc.h>

class Image
{
friend class Camera;
public:
	~Image(void);
	void SetPicture(EdsImageRef image);
	bool IsReady() { return is_ready_; }
	// Writes a preview of the image in the desired format
	void GetImagePreview(byte* targetBuffer, int width, int height);
private:
	Image();
	// Releases the given stream
	void StreamReleaser(EdsStreamRef stream);
	EdsImageRef image_ref_;
	volatile bool is_ready_;
};
