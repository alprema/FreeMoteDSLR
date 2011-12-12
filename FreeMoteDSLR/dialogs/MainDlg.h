// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../resources/resource.h"
#include "../wrappers/Camera.h"
#include "../scheduling/TaskRunner.h"
#include "../scheduling/TimeLapser.h"
#include "../wrappers/CallbackHandler.h"
#include "propertyHandlers/PropertyHandler.h"
#include <hash_map>

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	CMainDlg(Camera* camera);
	~CMainDlg(void);
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_RANGE_HANDLER(WM_IMAGE_DOWNLOADED, WM_PROPERTY_POSSIBLE_VALUES_CHANGED, CustomMessagesHandler)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(ID_TAKE_PICTURE, BN_CLICKED, OnBnClickedTakePicture)
		COMMAND_HANDLER(IDC_ISO_COMBO, CBN_SELCHANGE, OnSettingsComboSelchange)
		COMMAND_HANDLER(IDC_APERTURE_COMBO, CBN_SELCHANGE, OnSettingsComboSelchange)
		COMMAND_HANDLER(IDC_SPEED_COMBO, CBN_SELCHANGE, OnSettingsComboSelchange)
		COMMAND_HANDLER(IDC_ENABLE_SAVE_CHECKBOX, BN_CLICKED, OnEnableSaveCheckboxClicked)
		COMMAND_HANDLER(IDC_BROWSE_BUTTON, BN_CLICKED, OnBrowseButtonClicked)
		COMMAND_HANDLER(IDC_START_TIMELAPSE_BUTTON, BN_CLICKED, OnStartTimelapseButtonClicked)
		COMMAND_HANDLER(IDC_STOP_TIMELAPSE_BUTTON, BN_CLICKED, OnStopTimelapseButtonClicked)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT CustomMessagesHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);
private:
	Camera* camera_;
	// Selects the path in the path box as the "target path" after verification
	void selectPath();
	// Enables / disables the controls depending on the status of the timelapsing
	void toggleTimeLapseUI(bool timeLapsing);
	CStatic camera_image_;
	CFolderDialog* open_folder_dialog_;
	// The bitmap handle to the preview
	HBITMAP preview_bitmap_;
	// The bitmap bytes
	BYTE* preview_bytes_;
	static const int kPreviewWidth;
	static const int kPreviewHeight;
	static const int kMaxTargetPathLength;
	static const int kMaxTimeoutDigits;
	TaskRunner* task_runner_;
	CallbackHandler* callback_handler_;
	TimeLapser* time_lapser_;
	std::hash_map<int, PropertyHandler*> property_handlers_;
	std::hash_map<int, int> combobox_to_camera_property_;
public:
	LRESULT OnBnClickedTakePicture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void RedrawPreview();
	LRESULT OnSettingsComboSelchange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnableSaveCheckboxClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBrowseButtonClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStartTimelapseButtonClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStopTimelapseButtonClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
