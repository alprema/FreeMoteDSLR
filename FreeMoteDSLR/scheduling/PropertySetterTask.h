#pragma once
#include "CameraTask.h"

// Sets a property to a given camera value (only value type)
template <class TProperty>
class PropertySetterTask : public CameraTask
{
public:
	PropertySetterTask(Camera* camera, int propertyType, TProperty propertyValue);
	void Process();
private:
	int property_type_;
	TProperty property_value_;
};

template <class TProperty>
PropertySetterTask<TProperty>::PropertySetterTask(Camera* camera, int propertyType, TProperty propertyValue)
	: CameraTask(camera), property_type_(propertyType), property_value_(propertyValue)
{
}

template <class TProperty>
void PropertySetterTask<TProperty>::Process()
{
	EdsError err = EdsSetPropertyData(camera_->GetInnerCamera(), property_type_, 0, sizeof(property_value_), &property_value_);
}