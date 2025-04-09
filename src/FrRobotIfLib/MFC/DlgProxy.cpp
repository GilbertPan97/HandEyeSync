// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "frrjiftest.h"
#include "DlgProxy.h"
#include "frrjiftestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestDlgAutoProxy

IMPLEMENT_DYNCREATE(CFrrjiftestDlgAutoProxy, CCmdTarget)

CFrrjiftestDlgAutoProxy::CFrrjiftestDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CFrrjiftestDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CFrrjiftestDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CFrrjiftestDlgAutoProxy::~CFrrjiftestDlgAutoProxy()
{
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CFrrjiftestDlgAutoProxy::OnFinalRelease()
{
	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CFrrjiftestDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CFrrjiftestDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFrrjiftestDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CFrrjiftestDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IFrrjiftest to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {C7B64F3F-490C-44DE-9C95-3A50CA11E6CA}
static const IID IID_IFrrjiftest =
{ 0xc7b64f3f, 0x490c, 0x44de, { 0x9c, 0x95, 0x3a, 0x50, 0xca, 0x11, 0xe6, 0xca } };

BEGIN_INTERFACE_MAP(CFrrjiftestDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CFrrjiftestDlgAutoProxy, IID_IFrrjiftest, Dispatch)
END_INTERFACE_MAP()

// {79B77B5D-D197-4C24-9B5B-B5CCC3E19279}
IMPLEMENT_OLECREATE2(CFrrjiftestDlgAutoProxy, "Frrjiftest.Application", 0x79b77b5d, 0xd197, 0x4c24, 0x9b, 0x5b, 0xb5, 0xcc, 0xc3, 0xe1, 0x92, 0x79)

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestDlgAutoProxy message handlers
