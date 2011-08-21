#pragma once
#include "Task.h"
#include <Windows.h>

// The class handles messages asynchronously
// It can be stopped/joined but cannot be restarted.
// To restart it, just use a new object
class TaskRunner
{
public:
	// Initializes the task runner and starts it
	TaskRunner(int queueLength = kDefaultQueueSize);
	~TaskRunner(void);
	// Inserts a new task in the queue,
	// Returns false if the queue is full
	bool InsertTask(Task* task);
	// Stops the worker thread and joins it
	void StopAndWait();
	// Milliseconds to "sleep" between two queue checks (for the worker)
	static const int kLoopWaitMilliseconds;
private:
	// Default queue size
	static const int kDefaultQueueSize;

	// The worker thread loop
	static LRESULT workerProcess(LPVOID param);

	Task** tasks_queue_;
	int queue_length_;
	HANDLE worker_thread_;
	// The event to call to stop the thread
	HANDLE stop_event_;
	// Where the worker is at in the queue
	int worker_queue_index_;
	// Where the producer is at in the queue
	int producer_queue_index_;
};