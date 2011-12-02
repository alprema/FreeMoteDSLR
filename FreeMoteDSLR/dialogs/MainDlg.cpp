// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../resources/resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include <EDSDK.h>

#include <atlwin.h>

#include "../wrappers/CameraManager.h"
#include "../wrappers/Camera.h"
#include "../exceptions/ArgumentException.h"
#include "../scheduling/OpenSessionTask.h"
#include "../scheduling/CloseSessionTask.h"
#include "../scheduling/TakePictureTask.h"
#include "../scheduling/PreviewTask.h"
#include "../scheduling/PropertyRetrieverTask.h"
#include "../scheduling/PropertyPossibleValuesRetrieverTask.h"
#include "../scheduling/PropertySetterTask.h"
#include "propertyHandlers/TitleHandler.h"
#include "propertyHandlers/MultiValuePropertyHandler.h"
#include "propertyHandlers/textMappings/IsoSpeedMapping.h"
#include "propertyHandlers/textMappings/ApertureMapping.h"
#include "propertyHandlers/textMappings/SpeedMapping.h"

#ifndef __ATLIMAGE_H_
#define __ATLTYPES_H__	// Use the WTL types
#include <atlimage.h>
#endif

const int CMainDlg::kPreviewWidth = 800;
const int CMainDlg::kPreviewHeight = 533; // Make sure to keep 1.5 ratio

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

// Handles all the custom messages (CMessageFilter doesn't receive messages when the app is in a modal loop (like resize))
LRESULT CMainDlg::CustomMessagesHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	switch(uMsg){
		case WM_IMAGE_DOWNLOADED: {
			// TODO: Store the image object somewhere
			task_runner_->InsertTask(new PreviewTask((Image*)lParam, preview_bytes_, CMainDlg::kPreviewWidth, CMainDlg::kPreviewHeight, m_hWnd));
			return TRUE;
		}
		case WM_PREVIEW_GENERATED: {
			Image* downloadedImage = (Image*)lParam;
			delete(downloadedImage);
			// Only refresh if the preview was a success
			if (wParam)
				RedrawPreview();
			return TRUE;
		}
		case WM_PROPERTY_CHANGED: {
			PropertyHandler* handler = property_handlers_[wParam];
			if (NULL != handler)
				handler->SetValue(lParam);
			return TRUE;
		}
		case WM_PROPERTY_POSSIBLE_VALUES_CHANGED: {
			PropertyHandler* handler = property_handlers_[wParam];
			if (NULL != handler) {
				// Setting the possible values in the combo and asking for a refresh of the selected value to select the right one
				handler->SetPossibleValues(lParam);
				task_runner_->InsertTask(new PropertyRetrieverTask<int>(camera_, wParam, m_hWnd));
			}
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	camera_image_ = GetDlgItem(IDC_CAMERA_IMAGE);
	
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	// Worker thread management
	task_runner_ = new TaskRunner();
	callback_handler_ = new CallbackHandler(camera_, task_runner_, m_hWnd);
	task_runner_->InsertTask(new OpenSessionTask(camera_, callback_handler_));

	// Property handling
	// Remember to handle the property change / desc change in the HandleObjectEvent when adding one here
	// ISO Speed
	property_handlers_[kEdsPropID_ISOSpeed] = new MultiValuePropertyHandler((CComboBox)GetDlgItem(IDC_ISO_COMBO), new IsoSpeedMapping());
	combobox_to_camera_property_[IDC_ISO_COMBO] = kEdsPropID_ISOSpeed;
	task_runner_->InsertTask(new PropertyPossibleValuesRetrieverTask(camera_, kEdsPropID_ISOSpeed, m_hWnd));
	// Aperture
	property_handlers_[kEdsPropID_Av] = new MultiValuePropertyHandler((CComboBox)GetDlgItem(IDC_APERTURE_COMBO), new ApertureMapping());
	combobox_to_camera_property_[IDC_APERTURE_COMBO] = kEdsPropID_Av;
	task_runner_->InsertTask(new PropertyPossibleValuesRetrieverTask(camera_, kEdsPropID_Av, m_hWnd));
	// Speed
	property_handlers_[kEdsPropID_Tv] = new MultiValuePropertyHandler((CComboBox)GetDlgItem(IDC_SPEED_COMBO), new SpeedMapping());
	combobox_to_camera_property_[IDC_SPEED_COMBO] = kEdsPropID_Tv;
	task_runner_->InsertTask(new PropertyPossibleValuesRetrieverTask(camera_, kEdsPropID_Tv, m_hWnd));
	// Title
	property_handlers_[kEdsPropID_ProductName] = new TitleHandler(this);
	task_runner_->InsertTask(new PropertyRetrieverTask<char*>(camera_, kEdsPropID_ProductName, m_hWnd));

	UIAddChildWindowContainer(m_hWnd);

	// Let's create a HBITMAP to store a preview of the picture
	BITMAPINFOHEADER headerInfo;
	headerInfo.biSize = sizeof(BITMAPINFOHEADER);
	headerInfo.biWidth = CMainDlg::kPreviewWidth;
	headerInfo.biHeight = -1 * CMainDlg::kPreviewHeight;
	headerInfo.biPlanes = 1;
	headerInfo.biBitCount = 24;
	headerInfo.biCompression = BI_RGB;
	headerInfo.biSizeImage = 0;
	headerInfo.biClrUsed = 0;
	headerInfo.biClrImportant = 0;
	headerInfo.biXPelsPerMeter = 0;
	headerInfo.biYPelsPerMeter = 0;

	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader = headerInfo;
	RGBQUAD quad = { 0, 0, 0, 0 };
	bitmapInfo.bmiColors[0] = quad;
	preview_bitmap_ = CreateDIBSection(NULL, (const BITMAPINFO*)&bitmapInfo, DIB_RGB_COLORS, (void**) &preview_bytes_, NULL, 0);
	camera_image_.SetBitmap(preview_bitmap_);

	// Browse dialog
	open_folder_dialog_ = new CFolderDialog(m_hWnd, _T("Pick the destination folder"));
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	task_runner_->InsertTask(new CloseSessionTask(camera_));
	// We wait the worker's loop time to give it a change to unpile the task, then wait for it to finish whatever the result
	Sleep(TaskRunner::kLoopWaitMilliseconds);
	task_runner_->StopAndWait();
	DestroyWindow();
	::PostQuitMessage(nVal);
}

CMainDlg::CMainDlg(Camera* camera)
	:camera_(camera)
{
	if (NULL == camera)
		throw new ArgumentException("camera is NULL");
}

CMainDlg::~CMainDlg(void)
{
	delete(callback_handler_);
	delete(task_runner_);
	delete(camera_);
	delete(open_folder_dialog_);
	if (NULL != preview_bitmap_ && NULL != DeleteObject(preview_bitmap_))
		preview_bitmap_ = NULL;
	preview_bytes_ = NULL;
	for (auto ite = property_handlers_.begin(); ite != property_handlers_.end(); ++ite)
		delete(ite->second);
}

void CMainDlg::RedrawPreview()
{
	camera_image_.RedrawWindow();
}

LRESULT CMainDlg::OnBnClickedTakePicture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	task_runner_->InsertTask(new TakePictureTask(camera_));
	return 0;
}

LRESULT CMainDlg::OnSettingsComboSelchange(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	int cameraProperty = combobox_to_camera_property_[wID];
	CComboBox combobox(hWndCtl);
	task_runner_->InsertTask(new PropertySetterTask<int>(camera_, cameraProperty, combobox.GetItemData(combobox.GetCurSel())));
	return 0;
}


LRESULT CMainDlg::OnEnableSaveCheckboxClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CButton enableSaveCheckBox = (CButton)GetDlgItem(IDC_ENABLE_SAVE_CHECKBOX);
	CEdit targetEdit = (CEdit)GetDlgItem(IDC_TARGET_EDIT);
	CButton browseButton = (CButton)GetDlgItem(IDC_BROWSE_BUTTON);
	
	targetEdit.EnableWindow(enableSaveCheckBox.GetCheck());
	browseButton.EnableWindow(enableSaveCheckBox.GetCheck());
	return 0;
}


LRESULT CMainDlg::OnBrowseButtonClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	open_folder_dialog_->DoModal(m_hWnd);
	CEdit targetEdit = (CEdit)GetDlgItem(IDC_TARGET_EDIT);
	targetEdit.SetSelAll();
	targetEdit.SetWindowTextW(open_folder_dialog_->GetFolderPath());
	return 0;
}
