#pragma once
#include <EDSDK.h>

// Simple RAII wrapper for EdsStreamRef
// TODO: Test memory release
class StreamWrapper
{
public:
	StreamWrapper(EdsStreamRef stream) : stream_(stream) {};
	~StreamWrapper(void) { EdsRelease(stream_); };
private:
	EdsStreamRef stream_;
};

