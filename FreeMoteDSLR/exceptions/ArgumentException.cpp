#include "stdafx.h"
#include "ArgumentException.h"
#include "MyException.h"

ArgumentException::ArgumentException(char* message)
	: MyException(message)
{
}