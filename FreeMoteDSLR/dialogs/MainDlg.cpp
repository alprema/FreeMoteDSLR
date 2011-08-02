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
	// Set the window's title
	WTL::CString* productName = camera_->GetProductName();
	SetWindowText((LPCTSTR) ("Connected to: " + *productName));
	delete(productName);
	camera_image_ = GetDlgItem(IDC_CAMERA_IMAGE);
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

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
	delete(camera_);
	if (NULL != preview_bitmap_ && NULL != DeleteObject(preview_bitmap_))
		preview_bitmap_ = NULL;
	preview_bytes_ = NULL;
}

LRESULT CMainDlg::takePictureAsync(LPVOID param)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CMainDlg* dlg = (CMainDlg*)param;
	Image* image = dlg->camera_->TakePicture();

	image->GetImagePreview(dlg->preview_bytes_, CMainDlg::kPreviewWidth, CMainDlg::kPreviewHeight);
	dlg->camera_image_.SetBitmap(dlg->preview_bitmap_);
	dlg->camera_image_.RedrawWindow();

	CoUninitialize();
	return NULL;
}

LRESULT CMainDlg::OnBnClickedTakePicture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CMainDlg::takePictureAsync, this, 0, NULL);
	return 0;
}