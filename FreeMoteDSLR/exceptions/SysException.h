#pragma once
#include "MyException.h"

class SysException : MyException
{
public:
	SysException(char* message);
};
