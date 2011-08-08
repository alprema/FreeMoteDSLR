#pragma once
class Task
{
public:
	virtual ~Task(void) {};
	virtual void Process() = 0;
};

