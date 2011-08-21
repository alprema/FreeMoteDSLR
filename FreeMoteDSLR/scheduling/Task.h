#pragma once

// Represents a task that is sent to the TaskRunner,
// as a general practice, the tasks should not be in charge of 
// heavy objects cleanup
class Task
{
public:
	virtual ~Task(void) {};
	virtual void Process() = 0;
};

