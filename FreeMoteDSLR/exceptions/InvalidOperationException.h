#pragma once
#include "MyException.h"

class InvalidOperationException : MyException
{
public:
	InvalidOperationException(char* message);
};
