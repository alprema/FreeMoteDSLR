#pragma once

// Class in charge of handling a camera property 
// and binding it to a UI component
class PropertyHandler
{
public:
	virtual ~PropertyHandler(void) {};
	// The property handler is in charge of deleting the values container
	virtual void SetPossibleValues(LPARAM value) {};
	// The property handler is in charge of deleting the value object
	virtual void SetValue(LPARAM value) = 0;
};