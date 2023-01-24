
// PhotoFilter.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "PhotoFilter.h"
#include "PhotoFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhotoFilterApp

BEGIN_MESSAGE_MAP(CPhotoFilterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPhotoFilterApp 생성

CPhotoFilterApp::CPhotoFilterApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CPhotoFilterApp 개체입니다.

CPhotoFilterApp theApp;


// CPhotoFilterApp 초기화

BOOL CPhotoFilterApp::InitInstance()
{
	CWinApp::InitInstance();
	CPhotoFilterDlg dlg;
	dlg.DoModal();
	
	return FALSE;
}

