#pragma once
#include "Task.h"
#include "../wrappers/Image.h"

class PreviewTask : public Task
{
public:
	PreviewTask(Image* image, BYTE* targetBuffer, long targetWidth, long targetHeight, HWND completeMessageDestination);
	void Process();
private:
	void ExitProcess(bool success);
	Image* image_;
	BYTE* target_buffer_;
	long target_width_;
	long target_height_;
	HWND complete_message_destination_;
};

