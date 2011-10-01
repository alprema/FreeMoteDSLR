#include "stdafx.h"
#include "CallbackHandler.h"
#include "../scheduling/DownloadTask.h"
#include "../scheduling/PropertyRetrieverTask.h"
#include "../scheduling/PropertyPossibleValuesRetrieverTask.h"

CallbackHandler::CallbackHandler(Camera* camera, TaskRunner* taskRunner, HWND completeMessageDestination)
	:camera_(camera), task_runner_(taskRunner), complete_message_destination_(completeMessageDestination)
{
}

// An instance of a CallbackHandler shall be passed in context
EdsError EDSCALLBACK CallbackHandler::HandleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context)
{
	CallbackHandler* that = (CallbackHandler*)context;
	switch (event) {
		case kEdsObjectEvent_DirItemRequestTransfer: {
			that->task_runner_->InsertTask(new DownloadTask(that->camera_, (EdsDirectoryItemRef)object, that->complete_message_destination_));
			return EDS_ERR_OK;
		}
	}
	// object is normally release by the task, but since no task matches this event, we need to delete it here
	if (NULL != object)
		EdsRelease(object);
	return EDS_ERR_OK;
}

// An instance of a CallbackHandler shall be passed in context
EdsError EDSCALLBACK CallbackHandler::HandlePropertyEvent(EdsPropertyEvent event, EdsPropertyID cameraProperty, EdsUInt32 inParam, EdsVoid* context)
{
	// TODO: Find a way to know the type of the properties to avoid the useless tests (+ udpate related MainDlg comment when done)
	CallbackHandler* that = (CallbackHandler*)context;
	switch (event) {
		case kEdsPropertyEvent_PropertyChanged: {
			if (cameraProperty != kEdsPropID_ISOSpeed)
				break;
			that->task_runner_->InsertTask(new PropertyRetrieverTask<int>(that->camera_, cameraProperty, that->complete_message_destination_));
		}
		case kEdsPropertyEvent_PropertyDescChanged: {
			if (cameraProperty != kEdsPropID_ISOSpeed)
				break;
			that->task_runner_->InsertTask(new PropertyPossibleValuesRetrieverTask(that->camera_, cameraProperty, that->complete_message_destination_));
		}
	}
	return EDS_ERR_OK;
}