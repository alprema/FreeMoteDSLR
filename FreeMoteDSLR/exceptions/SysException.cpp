#include "stdafx.h"
#include "SysException.h"
#include "MyException.h"

SysException::SysException(char* message)
	: MyException(message)
{
}