#pragma once
#include "stdafx.h"

// Class in charge of handling a camera property 
// and binding it to a UI component
// The property handlers are in charge of freeing the values
// passed to it
class PropertyHandler
{
public:
	virtual ~PropertyHandler(void) {};
	virtual void SetValue(LPARAM value) = 0;
};