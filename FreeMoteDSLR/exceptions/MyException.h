#pragma once
#include <exception>

class MyException : std::exception
{
public:
	virtual const char* what() const throw();
	MyException(char* message);
	virtual ~MyException(void);
private:
	char* message_;
};
