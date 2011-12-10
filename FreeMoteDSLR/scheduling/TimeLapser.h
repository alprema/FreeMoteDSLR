#pragma once
#include "TaskRunner.h"

class TimeLapser
{
public:
	TimeLapser(TaskRunner* taskRunner, HWND windowHandlerToBindTimerTo);
	// Will try to stop the time lapse if it's running
	~TimeLapser(void);
	// Starts the time lapse with "timeOutMilliseconds" between every triggering
	void Start(int timeOutMilliseconds);
	// Stops the time lapse
	void Stop();
private:
	TaskRunner* task_runner_;
	// The callback called by the Win32 API, we use the idEvent to pass a pointer to
	// the instance of TimeLapser which created the timer
	static void CALLBACK TimerProc(__in_opt HWND hwnd, __in UINT uMsg, __in UINT_PTR idEvent, __in_opt DWORD dwTime);
	bool started_;
	HWND window_handler_to_bind_timer_to_;
};

