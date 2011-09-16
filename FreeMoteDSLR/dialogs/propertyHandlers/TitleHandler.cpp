#include "stdafx.h"
#include "TitleHandler.h"


TitleHandler::TitleHandler(CMainDlg* mainDlg)
	:main_dlg_(mainDlg)
{
}

void TitleHandler::SetValue(LPARAM value)
{
	char* titleValue = (char*)value;
	CString title(titleValue);
	main_dlg_->SetWindowText("Connected to: " + title);
	delete(titleValue);
}