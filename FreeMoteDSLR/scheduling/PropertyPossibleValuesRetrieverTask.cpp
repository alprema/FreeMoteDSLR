#include "stdafx.h"
#include "PropertyPossibleValuesRetrieverTask.h"
#include <vector>

PropertyPossibleValuesRetrieverTask::PropertyPossibleValuesRetrieverTask(Camera* camera, int cameraProperty, HWND completeMessageDestination)
	: CameraTask(camera), camera_property_(cameraProperty), complete_message_destination_(completeMessageDestination)
{
}

// Stores all the possible values in a vector and sends them to the main thread,
// which is then in charge of deleting it
void PropertyPossibleValuesRetrieverTask::Process()
{
	EdsPropertyDesc propertyDesc;
	EdsError err = EdsGetPropertyDesc(camera_->GetInnerCamera(), camera_property_, &propertyDesc);
	if (EDS_ERR_OK != err)
		return;
	std::vector<int>* possibleValues = new std::vector<int>();
	for (int i = 0; i < propertyDesc.numElements; ++i)
		possibleValues->push_back(propertyDesc.propDesc[i]);
	PostMessage(complete_message_destination_, WM_PROPERTY_POSSIBLE_VALUES_CHANGED, camera_property_, (LPARAM)possibleValues);
}
