// FreeMoteDSLR.cpp : main source file for FreeMoteDSLR.exe
//

#include "stdafx.h"

#include "resources/resource.h"

#include "dialogs/aboutdlg.h"
#include "dialogs/MainDlg.h"
#include "wrappers/CameraManager.h"
#include "wrappers/Camera.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT, Camera* camera = NULL)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain(camera);

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	//HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));


	CameraManager* manager = new CameraManager();
	Camera* camera = manager->GetCamera();	

	int nRet = 1;
	if (NULL == camera)
		AtlMessageBox(NULL, _T("No camera connected, please connect one and make sure it's turned on and try again"));
	else
		nRet = Run(lpstrCmdLine, nCmdShow, camera);

	delete(manager);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
