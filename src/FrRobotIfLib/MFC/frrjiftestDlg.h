// frrjiftestDlg.h : header file
//

#include "frrjif.h"
#include "frrobotiflib.h"

#if !defined(AFX_FRRJIFTESTDLG_H__926EFAB4_D344_46F7_9678_51E788764D55__INCLUDED_)
#define AFX_FRRJIFTESTDLG_H__926EFAB4_D344_46F7_9678_51E788764D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFrrjiftestDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestDlg dialog

class CFrrjiftestDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrrjiftestDlg);
	friend class CFrrjiftestDlgAutoProxy;

// Construction
public:
	CFrrjiftestDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CFrrjiftestDlg();

// Dialog Data
	//{{AFX_DATA(CFrrjiftestDlg)
	enum { IDD = IDD_FRRJIFTEST_DIALOG };
	CButton	m_chkLoop;
	CString	m_txtResult;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrrjiftestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	_Core *pCore;
	ILibCore *pLibCore;
	_DataTable *pDataTable;
	_DataTable *pDataTable2;
	_DataAlarm *pAlarm;
	_DataAlarm *pAlarmCurrent;
	_DataCurPos *pCurPos;
	_DataCurPos *pCurPosUF;
	_DataNumReg *pNumReg;
	_DataNumReg *pNumReg2;
	_DataNumReg *pNumReg3;
	_DataPosReg *pPosReg;
	_DataPosReg *pPosReg2;
	_DataPosRegXyzwpr *pPosRegXyzwpr;
	_DataPosRegMG *pPosRegMG;
	_DataTask *pTask;
	_DataSysVar *pSysVarInt;
	_DataSysVar *pSysVarInt2;
	_DataSysVar *pSysVarReal;
	_DataSysVar *pSysVarReal2;
	_DataSysVar *pSysVarString;
	_DataSysVarPos *pSysVarPos;
	_DataSysVar *pSysVarIntArray[10];
	_DataSysVar *pVarString;
	_DataString *pStrReg;


	CString HostName;
	int	mintSetIOToOn;

	void msubInit();
	void msubConnected();
	void msubDisconnected();
	void msubDisconnected2();
	void msubClearVars();
	void msubAddGIOResult(CString &pstr, long *buf, int count);
	void msubAddIOResult(CString &pstr, short *buf, int count);
	void msubAddAlarmResult(CString &pstr, _DataAlarm *pAlarm, int count);
	void msubAddJointResult(CString &pstr, float J1, float J2, float J3, float J4, float J5, float J6, float J7, float J8, float J9, short UT, short ValidJ);
	void msubAddXyzwprResult(CString &pstr, float X, float Y, float Z, float W, float P, float R, float E1, float E2, float E3, short C1, short C2, short C3, short C4, short C5, short C6, short C7, short UF, short UT, short validc);
	void msubAddTaskResult(CString &pstr, _DataTask *pTask);
	void msubSetSDO(int index);
	void msubSetGO(int index);

protected:
	CFrrjiftestDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CFrrjiftestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMenuitemExit();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonConnect();
	afx_msg void OnCheckLoop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSetNumreg();
	afx_msg void OnButtonSetPosregJ();
	afx_msg void OnButtonSetPosregX();
	afx_msg void OnButtonSetPosregX2();
	afx_msg void OnButtonSetSDO();
	afx_msg void OnButtonSetSDO2();
	afx_msg void OnButtonSetSDO3();
	afx_msg void OnButtonSetSDI();
	afx_msg void OnButtonSetRDO();
	afx_msg void OnButtonSetRDI();
	afx_msg void OnButtonSetGO();
	afx_msg void OnButtonSetGO2();
	afx_msg void OnButtonSetGI();
	afx_msg void OnButtonClearAlarm();
	afx_msg void OnButtonSetSysvar();
	afx_msg void OnButtonSetStrreg();
	afx_msg void OnButtonRefresh2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetPosregmg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRRJIFTESTDLG_H__926EFAB4_D344_46F7_9678_51E788764D55__INCLUDED_)
