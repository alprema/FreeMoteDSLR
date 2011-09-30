#pragma once
#include "CameraTask.h"

// Class in charge of retrieving all the possible values for a given
// property type
class PropertyPossibleValuesRetrieverTask : public CameraTask
{
public:
	PropertyPossibleValuesRetrieverTask(Camera* camera, int cameraProperty, HWND completeMessageDestination);
	void Process();
private:
	int camera_property_;
	HWND complete_message_destination_;
};

