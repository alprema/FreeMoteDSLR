#include "stdafx.h"
#include "CallbackHandler.h"
#include "../scheduling/DownloadTask.h"
#include "../scheduling/PropertyRetrieverTask.h"
#include "../scheduling/PropertyPossibleValuesRetrieverTask.h"

CallbackHandler::CallbackHandler(Camera* camera, TaskRunner* taskRunner, HWND completeMessageDestination)
	: camera_(camera), task_runner_(taskRunner), complete_message_destination_(completeMessageDestination),
	  download_target_(NULL)
{
}

CallbackHandler::~CallbackHandler()
{
	if (NULL != download_target_)
		delete download_target_;
}

void CallbackHandler::SetDownloadTarget(CString* downloadTarget)
{
	// TODO: test that
	if (NULL != download_target_)
		delete download_target_;
	download_target_ = downloadTarget;
}

EdsError EDSCALLBACK CallbackHandler::HandleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context)
{
	CallbackHandler* that = (CallbackHandler*)context;
	switch (event) {
		case kEdsObjectEvent_DirItemRequestTransfer: {
			that->task_runner_->InsertTask(new DownloadTask(that->camera_, (EdsDirectoryItemRef)object, that->complete_message_destination_, that->download_target_));
			return EDS_ERR_OK;
		}
	}
	// object is normally released by the task, but since no task matches this event, we need to delete it here
	if (NULL != object)
		EdsRelease(object);
	return EDS_ERR_OK;
}

EdsError EDSCALLBACK CallbackHandler::HandlePropertyEvent(EdsPropertyEvent event, EdsPropertyID cameraProperty, EdsUInt32 inParam, EdsVoid* context)
{
	// TODO: Find a way to know the type of the properties to avoid the useless tests (+ udpate related MainDlg comment when done)
	CallbackHandler* that = (CallbackHandler*)context;
	switch (event) {
		case kEdsPropertyEvent_PropertyChanged: {
			if (cameraProperty != kEdsPropID_ISOSpeed && cameraProperty != kEdsPropID_Av && cameraProperty != kEdsPropID_Tv)
				break;
			that->task_runner_->InsertTask(new PropertyRetrieverTask<int>(that->camera_, cameraProperty, that->complete_message_destination_));
		}
		case kEdsPropertyEvent_PropertyDescChanged: {
			if (cameraProperty != kEdsPropID_ISOSpeed && cameraProperty != kEdsPropID_Av && cameraProperty != kEdsPropID_Tv)
				break;
			that->task_runner_->InsertTask(new PropertyPossibleValuesRetrieverTask(that->camera_, cameraProperty, that->complete_message_destination_));
		}
	}
	return EDS_ERR_OK;
}