// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__137AF4A4_DA8D_43F3_872F_73B4424AA843__INCLUDED_)
#define AFX_DLGPROXY_H__137AF4A4_DA8D_43F3_872F_73B4424AA843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFrrjiftestDlg;

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestDlgAutoProxy command target

class CFrrjiftestDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CFrrjiftestDlgAutoProxy)

	CFrrjiftestDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CFrrjiftestDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrrjiftestDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrrjiftestDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CFrrjiftestDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CFrrjiftestDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFrrjiftestDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__137AF4A4_DA8D_43F3_872F_73B4424AA843__INCLUDED_)
