#pragma once
#include "PropertyHandler.h"
#include "../MainDlg.h"

class TitleHandler : public PropertyHandler
{
public:
	TitleHandler(CMainDlg* window);
	void SetValue(LPARAM value);
private:
	CMainDlg* main_dlg_;
};