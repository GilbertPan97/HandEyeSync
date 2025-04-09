// frrjiftest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "frrjiftest.h"
#include "frrjiftestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestApp

BEGIN_MESSAGE_MAP(CFrrjiftestApp, CWinApp)
	//{{AFX_MSG_MAP(CFrrjiftestApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestApp construction

CFrrjiftestApp::CFrrjiftestApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFrrjiftestApp object

CFrrjiftestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestApp initialization

BOOL CFrrjiftestApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization

	if (RunEmbedded() || RunAutomated())
	{
		COleTemplateServer::RegisterAll();
	}
	else
	{
		COleObjectFactory::UpdateRegistryAll();
	}

	CFrrjiftestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
