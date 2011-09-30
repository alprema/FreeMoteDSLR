#include "stdafx.h"
#include "PropertyRetrieverTask.h"

template <>
void PropertyRetrieverTask<char*>::Process()
{
	EdsDataType dataType;
	EdsUInt32 outSize;
	EdsError err = EdsGetPropertySize(camera_->GetInnerCamera(), camera_property_, 0, &dataType, &outSize);
	char* name = new char[outSize];
	err = EdsGetPropertyData(camera_->GetInnerCamera(), camera_property_, 0, sizeof(char) * outSize, name);
	if (EDS_ERR_OK == err)
		PostMessage(complete_message_destination_, WM_PROPERTY_CHANGED, camera_property_, (LPARAM)name);
}