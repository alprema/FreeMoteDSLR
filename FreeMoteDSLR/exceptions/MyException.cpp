#include "stdafx.h"
#include "MyException.h"

const char* MyException::what() const throw()
{
	return message_;
}

MyException::MyException(char* message)
	: message_(message)
{
}

MyException::~MyException(void)
{
	free(message_);
}
