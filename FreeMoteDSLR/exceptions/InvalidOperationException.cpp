#include "stdafx.h"
#include "InvalidOperationException.h"
#include "MyException.h"

InvalidOperationException::InvalidOperationException(char* message)
	: MyException(message)
{
}