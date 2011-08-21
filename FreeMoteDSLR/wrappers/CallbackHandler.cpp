#include "stdafx.h"
#include "CallbackHandler.h"
#include "../scheduling/DownloadTask.h"

CallbackHandler::CallbackHandler(Camera* camera, TaskRunner* taskRunner, HWND completeMessageDestination)
	:camera_(camera), task_runner_(taskRunner), complete_message_destination_(completeMessageDestination)
{
}

// An instance of a CallbackHandler shall be passed in context
EdsError EDSCALLBACK CallbackHandler::HandleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context)
{
	CallbackHandler* that = (CallbackHandler*)context;

	// "object" will be released by the task
	that->task_runner_->InsertTask(new DownloadTask(that->camera_, (EdsDirectoryItemRef)object, that->complete_message_destination_));

	return EDS_ERR_OK;
}