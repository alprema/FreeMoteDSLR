#include "StdAfx.h"
#include "TaskRunner.h"
#include <string>
#include "./exceptions/SysException.h"

const int TaskRunner::kLoopWaitMilliseconds = 10;
const int TaskRunner::kDefaultQueueSize = 20;

TaskRunner::TaskRunner(int queueLength)
	:queue_length_(queueLength), worker_queue_index_(0), producer_queue_index_(0)
{
	tasks_queue_ = new Task*[queue_length_];
	memset(tasks_queue_, NULL, sizeof(int) * queue_length_);
	stop_event_ = CreateEvent(NULL, true, false, _T("stopEvent"));
	if (NULL == stop_event_)
		throw new SysException("Unable to create a new event");
	worker_thread_ = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)TaskRunner::workerProcess, this, 0, NULL);
	if (NULL == worker_thread_)
		throw new SysException("Unable to create a new thread");
}

TaskRunner::~TaskRunner(void)
{
	delete[] tasks_queue_;
	CloseHandle(worker_thread_);
	CloseHandle(stop_event_);
}

// The task will be deleted by the TaskRunner when it is processed
bool TaskRunner::InsertTask(Task* task)
{
	Task* initialValue = (Task*)InterlockedCompareExchangePointer((PVOID*)(tasks_queue_ + producer_queue_index_), task, NULL);
	if (NULL == initialValue)
	{
		producer_queue_index_ = (producer_queue_index_ + 1) % queue_length_;
		return true;
	}
	return false;

}

void TaskRunner::StopAndWait()
{
	if (!SetEvent(stop_event_))
		throw new SysException("Unable to set the stop event");
	WaitForSingleObject(worker_thread_, INFINITE);
}

// We wait for the stop event to be set and unpile tasks from the queue
// and process them
LRESULT TaskRunner::workerProcess(LPVOID param)
{
	TaskRunner* that = (TaskRunner*)param;
	do {
		if (NULL == that->tasks_queue_[that->worker_queue_index_])
			continue;
		
		Task* taskToProcess = that->tasks_queue_[that->worker_queue_index_];
		InterlockedExchangePointer((void**)(that->tasks_queue_ + that->worker_queue_index_), NULL);
		that->worker_queue_index_ = (that->worker_queue_index_ + 1) % that->queue_length_;
		taskToProcess->Process();
		delete taskToProcess;
	} while (WAIT_TIMEOUT == WaitForSingleObject(that->stop_event_, kLoopWaitMilliseconds));
	return 0;
}
