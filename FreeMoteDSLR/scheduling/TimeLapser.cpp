#include "stdafx.h"
#include "TimeLapser.h"
#include "../exceptions/SysException.h"

TimeLapser::TimeLapser(TaskRunner* taskRunner, HWND windowHandlerToBindTimerTo)
	:task_runner_(taskRunner), started_(false), window_handler_to_bind_timer_to_(windowHandlerToBindTimerTo)
{
}

TimeLapser::~TimeLapser(void)
{
	Stop();
}

void TimeLapser::Start(int timeOutMilliseconds)
{
	if (NULL == SetTimer(window_handler_to_bind_timer_to_, (UINT_PTR)this, timeOutMilliseconds, TimeLapser::TimerProc))
		throw new SysException("Unable to create a new event");
	started_ = true;
}

void TimeLapser::Stop()
{
	if (started_)
		started_ = !KillTimer(window_handler_to_bind_timer_to_, (UINT_PTR)this);
}

void CALLBACK TimeLapser::TimerProc(__in_opt HWND hwnd, __in UINT uMsg, __in UINT_PTR idEvent, __in_opt DWORD dwTime)
{
	TimeLapser* that = (TimeLapser*)idEvent;
}
