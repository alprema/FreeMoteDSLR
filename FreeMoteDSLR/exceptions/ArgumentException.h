#pragma once
#include "MyException.h"

class ArgumentException : MyException
{
public:
	ArgumentException(char* message);
};
