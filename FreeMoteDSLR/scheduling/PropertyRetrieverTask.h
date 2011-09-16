#pragma once
#include "CameraTask.h"

template <class TProperty>
class PropertyRetrieverTask: public CameraTask
{
public:
	PropertyRetrieverTask(Camera* camera, int cameraProperty, HWND completeMessageDestination);
	void Process();
private:
	int camera_property_;
	HWND complete_message_destination_;
};

template <class TProperty>
PropertyRetrieverTask<TProperty>::PropertyRetrieverTask(Camera* camera, int cameraProperty, HWND completeMessageDestination)
	: CameraTask(camera), camera_property_(cameraProperty), complete_message_destination_(completeMessageDestination)
{
}

template <class TProperty>
void PropertyRetrieverTask<TProperty>::Process()
{
	TProperty* value = new TProperty;
	EdsError err = EdsGetPropertyData(camera_->GetInnerCamera(), camera_property_, 0, sizeof(TProperty), value);
	PostMessage(complete_message_destination_, WM_PROPERTY_CHANGED, camera_property_, (LPARAM)value);
}

template <>
void PropertyRetrieverTask<char*>::Process()
{
	EdsDataType dataType;
	EdsUInt32 outSize;
	EdsError err = EdsGetPropertySize(camera_->GetInnerCamera(), camera_property_, 0, &dataType, &outSize);
	char* name = new char[outSize];
	err = EdsGetPropertyData(camera_->GetInnerCamera(), camera_property_, 0, sizeof(char) * outSize, name);
	PostMessage(complete_message_destination_, WM_PROPERTY_CHANGED, camera_property_, (LPARAM)name);
}