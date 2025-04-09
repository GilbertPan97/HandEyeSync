// frrjiftestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "frrjiftest.h"
#include "frrjiftestDlg.h"
#include "DlgProxy.h"

#include "InputBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestDlg dialog

IMPLEMENT_DYNAMIC(CFrrjiftestDlg, CDialog);

CFrrjiftestDlg::CFrrjiftestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrrjiftestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrrjiftestDlg)
	m_txtResult = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	pDataTable = NULL;
	pDataTable2 = NULL;
}

CFrrjiftestDlg::~CFrrjiftestDlg()
{
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CFrrjiftestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrrjiftestDlg)
	DDX_Control(pDX, IDC_CHECK_LOOP, m_chkLoop);
	DDX_Text(pDX, IDC_TXT_RESULT, m_txtResult);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFrrjiftestDlg, CDialog)
	//{{AFX_MSG_MAP(CFrrjiftestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENUITEM_EXIT, OnMenuitemExit)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_CHECK_LOOP, OnCheckLoop)
	// ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SET_NUMREG, OnButtonSetNumreg)
	ON_BN_CLICKED(IDC_BUTTON_SET_POSREGJ, OnButtonSetPosregJ)
	ON_BN_CLICKED(IDC_BUTTON_SET_POSREGX, OnButtonSetPosregX)
	ON_BN_CLICKED(IDC_BUTTON_SET_POSREGX2, OnButtonSetPosregX2)
	ON_BN_CLICKED(IDC_BUTTON_SET_SDO, OnButtonSetSDO)
	ON_BN_CLICKED(IDC_BUTTON_SET_SDO2, OnButtonSetSDO2)
	ON_BN_CLICKED(IDC_BUTTON_SET_SDO3, OnButtonSetSDO3)
	ON_BN_CLICKED(IDC_BUTTON_SET_SDI, OnButtonSetSDI)
	ON_BN_CLICKED(IDC_BUTTON_SET_RDO, OnButtonSetRDO)
	ON_BN_CLICKED(IDC_BUTTON_SET_RDI, OnButtonSetRDI)
	ON_BN_CLICKED(IDC_BUTTON_SET_GO, OnButtonSetGO)
	ON_BN_CLICKED(IDC_BUTTON_SET_GO2, OnButtonSetGO2)
	ON_BN_CLICKED(IDC_BUTTON_SET_GI, OnButtonSetGI)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALARM, OnButtonClearAlarm)
	ON_BN_CLICKED(IDC_BUTTON_SET_SYSVAR, OnButtonSetSysvar)
	ON_BN_CLICKED(IDC_BUTTON_SET_STRREG, OnButtonSetStrreg)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH2, OnButtonRefresh2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SET_POSREGMG, &CFrrjiftestDlg::OnBnClickedButtonSetPosregmg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrrjiftestDlg message handlers

BOOL CFrrjiftestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	SetMenu(&menu);
	menu.Detach();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	mintSetIOToOn = 1;
	msubInit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFrrjiftestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFrrjiftestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CFrrjiftestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFrrjiftestDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CFrrjiftestDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CFrrjiftestDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CFrrjiftestDlg::CanExit()
{
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CFrrjiftestDlg::OnMenuitemExit() 
{
	if (pCore) pCore->Disconnect();
	msubClearVars();
	OnCancel();
}

/////////////////////////////////////////////////////////////////////////////

void CFrrjiftestDlg::msubInit() 
{
	TRY
	{
		// Make core object
		pCore = new _Core;
		if (!pCore->CreateDispatch(_T("FRRJIF.Core"))) 
		{
			AfxMessageBox(_T("Unable to create FRRJIF.Core"));
			pCore = NULL;
			return ;
		}
		pLibCore = new ILibCore(pCore->GetLib());

		// Make data table
		pDataTable = new _DataTable(pCore->GetDataTable());

		// Set data table items
		pAlarm = new _DataAlarm(pDataTable->AddAlarm(ALARM_LIST, 5, 0));
		pAlarmCurrent = new _DataAlarm(pDataTable->AddAlarm(ALARM_CURRENT, 1, 0));
		pCurPos = new _DataCurPos(pDataTable->AddCurPos(CURPOS, 1));
		pCurPosUF = new _DataCurPos(pDataTable->AddCurPosUF(CURPOS, 1, 15));
		pNumReg = new _DataNumReg(pDataTable->AddNumReg(NUMREG_INT, 1, 5));
		pNumReg2 = new _DataNumReg(pDataTable->AddNumReg(NUMREG_REAL, 6, 10));
		pPosReg = new _DataPosReg(pDataTable->AddPosReg(POSREG, 1, 1, 5));
		pPosReg2 = new _DataPosReg(pDataTable->AddPosReg(POSREG, 2, 1, 5));
		pPosRegXyzwpr = new _DataPosRegXyzwpr(pDataTable->AddPosRegXyzwpr(POSREG_XYZWPR, 1, 1, 5));
		pPosRegMG = new _DataPosRegMG(pDataTable->AddPosRegMG(POSREGMG, _T("C,J6"), 1, 5));
		pSysVarInt = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_INT, _T("$FAST_CLOCK")));
		pSysVarInt2 = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_INT, _T("$TIMER[10].$TIMER_VAL")));
		pSysVarReal = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_REAL, _T("$MOR_GRP[1].$CURRENT_ANG[1]")));
		pSysVarReal2 = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_REAL, _T("$DUTY_TEMP")));
		pSysVarString = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_STRING, _T("$TIMER[10].$COMMENT")));
		pSysVarPos = new _DataSysVarPos(pDataTable->AddSysVarPos(SYSVAR_POS, _T("$MNUTOOL[1,1]")));
		pVarString = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_STRING, _T("$[HTTPKCL]CMDS[1]")));
		pTask = new _DataTask(pDataTable->AddTask(TASK, 1));
		pStrReg = new _DataString(pDataTable->AddString(STRREG, 1, 5));

		// 2nd data table.
		// You must not set the first data table.
		pDataTable2 = new _DataTable(pCore->GetDataTable2());
		pNumReg3 = new _DataNumReg(pDataTable2->AddNumReg(NUMREG_INT, 1, 5));
		pSysVarIntArray[0] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[1].$TIMER_VAL")));
		pSysVarIntArray[1] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[2].$TIMER_VAL")));
		pSysVarIntArray[2] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[3].$TIMER_VAL")));
		pSysVarIntArray[3] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[4].$TIMER_VAL")));
		pSysVarIntArray[4] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[5].$TIMER_VAL")));
		pSysVarIntArray[5] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[6].$TIMER_VAL")));
		pSysVarIntArray[6] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[7].$TIMER_VAL")));
		pSysVarIntArray[7] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[8].$TIMER_VAL")));
		pSysVarIntArray[8] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[9].$TIMER_VAL")));
		pSysVarIntArray[9] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[10].$TIMER_VAL")));

		// Host name
		if (HostName.IsEmpty()) 
		{
			CInputBox dlg;
			HostName = AfxGetApp()->GetProfileString(_T("Settings"), _T("HostName"), _T(""));
			dlg.m_Prompt = _T("Please input robot host name");
			dlg.m_Value = HostName;
			dlg.DoModal();
			HostName = dlg.m_Value;
			AfxGetApp()->WriteProfileString(_T("Settings"), _T("HostName"), HostName);
		}

		// Time out
		int intTmp;
		intTmp = AfxGetApp()->GetProfileInt(_T("Settings"), _T("TimeOut"), -1);
		if (intTmp > 0) pCore->SetTimeOutValue(intTmp);

		// Connect
		int intRes;
		intRes = pCore->Connect(HostName);
		if (intRes)
		{
			// OK
			msubConnected();
		}
		else
		{
			// NG
			msubDisconnected();
		}
	}
	CATCH (CException, e)
	{
		e->ReportError();
	}
	END_CATCH
}

void CFrrjiftestDlg::msubConnected() 
{
	m_txtResult = _T("Connect OK to ") + HostName;
	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("Disconnect"));
	m_chkLoop.EnableWindow(TRUE);

	// Timer
	SetTimer(1, 1000, NULL);
}

void CFrrjiftestDlg::msubDisconnected() 
{
	m_txtResult = _T("Disconnected to ") + HostName;
	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("Connect"));
	msubClearVars();
	m_chkLoop.EnableWindow(FALSE);

	// Timer
	KillTimer(1);
}

void CFrrjiftestDlg::msubDisconnected2() 
{
	msubDisconnected();
}

void CFrrjiftestDlg::msubClearVars() 
{
	if (pCore) delete pCore;
	pCore = NULL;
	if (pLibCore) delete pLibCore;
	pLibCore = NULL;
	if (pDataTable) delete pDataTable;
	pDataTable = NULL;
	if (pDataTable2) delete pDataTable2;
	pDataTable2 = NULL;
	if (pAlarm) delete pAlarm;
	pAlarm = NULL;
	if (pAlarmCurrent) delete pAlarmCurrent;
	pAlarmCurrent = NULL;
	if (pCurPos) delete pCurPos;
	pCurPos = NULL;
	if (pCurPosUF) delete pCurPosUF;
	pCurPosUF = NULL;
	if (pNumReg) delete pNumReg;
	pNumReg = NULL;
	if (pNumReg2) delete pNumReg2;
	pNumReg2 = NULL;
	if (pNumReg3) delete pNumReg3;
	pNumReg3 = NULL;
	if (pPosReg) delete pPosReg;
	pPosReg = NULL;
	if (pPosReg2) delete pPosReg2;
	pPosReg2 = NULL;
	if (pPosRegXyzwpr) delete pPosRegXyzwpr;
	pPosRegXyzwpr = NULL;
	if (pPosRegMG) delete pPosRegMG;
	pPosRegMG = NULL;
	if (pSysVarInt) delete pSysVarInt;
	pSysVarInt = NULL;
	if (pSysVarInt2) delete pSysVarInt2;
	pSysVarInt2 = NULL;
	if (pSysVarReal) delete pSysVarReal;
	pSysVarReal = NULL;
	if (pSysVarReal2) delete pSysVarReal2;
	pSysVarReal2 = NULL;
	if (pSysVarString) delete pSysVarString;
	pSysVarString = NULL;
	if (pVarString) delete pVarString;
	pVarString = NULL;
	if (pSysVarPos) delete pSysVarPos;
	pSysVarPos = NULL;
	if (pTask) delete pTask;
	pTask = NULL;
	if (pStrReg) delete pStrReg;
	pStrReg = NULL;
	for (int ii = 0; ii <= 9; ii++)
	{
		if (pSysVarIntArray[ii]) delete pSysVarIntArray[ii];
		pSysVarIntArray[ii] = NULL;
	}

}


void CFrrjiftestDlg::OnButtonRefresh() 
{
	TRY
	{
		int ii;
		int blnIO;
		COleVariant *pVal;
		CString strTmp;
		float X, Y, Z, W, P, R, E1, E2, E3, J1, J2, J3, J4, J5, J6, J7, J8, J9;
		short C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC, ValidJ;
		short shtSDO[48];
		short shtSDO2[40];
		short shtSDO3[40];
		short shtSDI[40];
		short shtRDO[10];
		short shtRDI[10];
		short shtSO[20];
		short shtSI[20];
		short shtUO[20];
		short shtUI[20];
		long lngAO[3];
		long lngAI[3];
		long lngGO[3];
		long lngGO2[3];
		long lngGI[3];
		short shtWO[6];
		short shtWI[6];
		short shtWSI[6];
		int blnSDO;
		int blnSDO2;
		int blnSDO3;
		int blnSDI;
		int blnRDO;
		int blnRDI;
		int blnSO;
		int blnSI;
		int blnUO;
		int blnUI;
		int blnAO;
		int blnAI;
		int blnGO;
		int blnGO2;
		int blnGI;
		int blnWO;
		int blnWI;
		int blnWSI;
		BSTR strValue;
		CString strSpace = _T("                                                                                                   ");


		// Check
		if (pCore == NULL) return;

		// Init
		m_txtResult = "";
		pVal = new COleVariant();
		blnIO = 1;

		// Read IO
		blnSDO = pLibCore->ReadSDO(1, shtSDO, 48);
		blnSDO2 = pLibCore->ReadSDO(10001, shtSDO2, 40);
		blnSDO3 = pLibCore->ReadSDO(11001, shtSDO3, 40);
		blnSDI = pLibCore->ReadSDI(1, shtSDI, 40);
		blnRDO = pLibCore->ReadRDO(1, shtRDO, 8);
		blnRDI = pLibCore->ReadRDI(1, shtRDI, 8);
		blnSO = pLibCore->ReadSO(0, shtSO, 15);
		blnSI = pLibCore->ReadSI(0, shtSI, 15);
		blnUO = pLibCore->ReadUO(1, shtUO, 15);
		blnUI = pLibCore->ReadUI(1, shtUI, 15);
		blnGO = pLibCore->ReadGO(1, lngGO, 3);
		blnGO2 = pLibCore->ReadGO(10001 - 6000, lngGO2, 3);	// Offset -6000 for PMC data table
		blnGI = pLibCore->ReadGI(1, lngGI, 3);
		// read AO. Offset 1000 for AO
		blnAO = pLibCore->ReadGO(1000 + 1, lngAO, 3);
		// read AI. Offset 1000 for AI
		blnAI = pLibCore->ReadGI(1000 + 1, lngAI, 3);
		// read WO. Offset 8000 for WO
		blnWO = pLibCore->ReadSDO(8000 + 1, shtWO, 5);
		// read WI. Offset 8000 for WI
		blnWI = pLibCore->ReadSDI(8000 + 1, shtWI, 5);
		// read WSI. Offset 8400 for WSI
		blnWSI = pLibCore->ReadSDI(8400 + 1, shtWSI, 5);

		// Refresh data table to get data from robot
		pDataTable->Refresh();

		// System Variables
		m_txtResult += _T("--- SysVar ---\r\n");
		if (pSysVarInt->GetValue(pVal))
		{
			strTmp.Format(_T("%s = %d\r\n"), pSysVarInt->GetSysVarName(), pVal->intVal);
			m_txtResult += strTmp;
		}
		else
		{
			strTmp.Format(_T("%s : Error\r\n"), pSysVarInt->GetSysVarName());
			m_txtResult += strTmp;
		}
		if (pSysVarInt2->GetValue(pVal))
		{
			strTmp.Format(_T("%s = %d\r\n"), pSysVarInt2->GetSysVarName(), pVal->intVal);
			m_txtResult += strTmp;
		}
		else
		{
			strTmp.Format(_T("%s : Error\r\n"), pSysVarInt2->GetSysVarName());
			m_txtResult += strTmp;
		}
		if (pSysVarReal->GetValue(pVal))
		{
			strTmp.Format(_T("%s = %f\r\n"), pSysVarReal->GetSysVarName(), pVal->fltVal);
			m_txtResult += strTmp;
		}
		else
		{
			strTmp.Format(_T("%s : Error\r\n"), pSysVarReal->GetSysVarName());
			m_txtResult += strTmp;
		}
		if (pSysVarReal2->GetValue(pVal))
		{
			strTmp.Format(_T("%s = %f\r\n"), pSysVarReal2->GetSysVarName(), pVal->fltVal);
			m_txtResult += strTmp;
		}
		else
		{
			strTmp.Format(_T("%s : Error\r\n"), pSysVarReal2->GetSysVarName());
			m_txtResult += strTmp;
		}
		if (pSysVarString->GetValue(pVal))
		{
			strTmp.Format(_T("%s = %S\r\n"), pSysVarString->GetSysVarName(), pVal->pbVal);
			m_txtResult += strTmp;
		}
		else
		{
			strTmp.Format(_T("%s : Error\r\n"), pSysVarString->GetSysVarName());
			m_txtResult += strTmp;
		}
		if (pSysVarPos->GetValueXyzwpr(&X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC))
		{
			strTmp.Format(_T("%s = "), pSysVarPos->GetSysVarName());
			msubAddXyzwprResult(m_txtResult, X, Y, Z, W, P, R, E1, E2, E3, C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC);
		}
		else
		{
			strTmp.Format(_T("%s : Error\r\n"), pSysVarPos->GetSysVarName());
			m_txtResult += strTmp;
		}
		if (pVarString->GetValue(pVal))
		{
			strTmp.Format(_T("%s = %S\r\n"), pVarString->GetSysVarName(), pVal->pbVal);
			m_txtResult += strTmp;
		}
		else
		{
			strTmp.Format(_T("%s : Error\r\n"), pVarString->GetSysVarName());
			m_txtResult += strTmp;
		}

		// Current Position
		m_txtResult += _T("\r\n--- CurPos GP1 World ---\r\n");
		if (pCurPos->GetValueXyzwpr(&X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC))
		{
			msubAddXyzwprResult(m_txtResult, X, Y, Z, W, P, R, E1, E2, E3, C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC);
		}
		else
		{
			m_txtResult += _T("CurPos Error\r\n");
		}
		if (pCurPos->GetValueJoint(&J1, &J2, &J3, &J4, &J5, &J6, &J7, &J8, &J9, &UT, &ValidJ))
		{
			msubAddJointResult(m_txtResult, J1, J2, J3, J4, J5, J6, J7, J8, J9, UT, ValidJ);
		}
		else
		{
			m_txtResult += _T("CurPos Error\r\n");
		}

		// Current Position User Frame based
		m_txtResult += _T("\r\n--- CurPos GP1 Current User Frame ---\r\n");
		if (pCurPosUF->GetValueXyzwpr(&X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC))
		{
			msubAddXyzwprResult(m_txtResult, X, Y, Z, W, P, R, E1, E2, E3, C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC);
		}
		else
		{
			m_txtResult += _T("CurPos Error\r\n");
		}
		if (pCurPosUF->GetValueJoint(&J1, &J2, &J3, &J4, &J5, &J6, &J7, &J8, &J9, &UT, &ValidJ))
		{
			msubAddJointResult(m_txtResult, J1, J2, J3, J4, J5, J6, J7, J8, J9, UT, ValidJ);
		}
		else
		{
			m_txtResult += _T("CurPos Error\r\n");
		}

		// Numeric Register
		m_txtResult += _T("\r\n--- NumReg ---\r\n");
		for (ii = pNumReg->GetStartIndex() ; ii <=pNumReg->GetEndIndex() ; ii++) 
		{
			if (pNumReg->GetValue(ii, pVal)) 
			{
				strTmp.Format(_T("R[%d] = %d\r\n"), ii, pVal->intVal);
				m_txtResult += strTmp;
			}
			else
			{
				strTmp.Format(_T("R[%d] : Error\r\n"), ii);
				m_txtResult += strTmp;
			}
		}
		for (ii = pNumReg2->GetStartIndex() ; ii <=pNumReg2->GetEndIndex() ; ii++) 
		{
			if (pNumReg2->GetValue(ii, pVal)) 
			{
				strTmp.Format(_T("R[%d] = %g\r\n"), ii, pVal->fltVal);
				m_txtResult += strTmp;
			}
			else
			{
				strTmp.Format(_T("R[%d] : Error\r\n"), ii);
				m_txtResult += strTmp;
			}
		}

		// Position Register
		m_txtResult += _T("\r\n--- PosReg ---\r\n");
		for (ii = pPosReg->GetStartIndex() ; ii <=pPosReg->GetEndIndex() ; ii++) 
		{
			strTmp.Format(_T("PR[%d] : "), ii);
			m_txtResult += strTmp;
			if (pPosReg->GetValueXyzwpr(ii, &X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC))
			{
				msubAddXyzwprResult(m_txtResult, X, Y, Z, W, P, R, E1, E2, E3, C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC);
			}
			else
			{
				m_txtResult += _T("PosReg Error\r\n");
			}
			if (pPosReg->GetValueJoint(ii, &J1, &J2, &J3, &J4, &J5, &J6, &J7, &J8, &J9, &UT, &ValidJ))
			{
				msubAddJointResult(m_txtResult, J1, J2, J3, J4, J5, J6, J7, J8, J9, UT, ValidJ);
			}
			else
			{
				m_txtResult += _T("PosReg Error\r\n");
			}
		}

		// Position Register
		m_txtResult += _T("\r\n--- PosReg2 ---\r\n");
		for (ii = pPosReg2->GetStartIndex() ; ii <=pPosReg2->GetEndIndex() ; ii++) 
		{
			strTmp.Format(_T("PR[GP2:%d] : "), ii);
			m_txtResult += strTmp;
			if (pPosReg2->GetValueXyzwpr(ii, &X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC))
			{
				msubAddXyzwprResult(m_txtResult, X, Y, Z, W, P, R, E1, E2, E3, C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC);
			}
			else
			{
				m_txtResult += _T("PosReg2 Error\r\n");
			}
			if (pPosReg2->GetValueJoint(ii, &J1, &J2, &J3, &J4, &J5, &J6, &J7, &J8, &J9, &UT, &ValidJ))
			{
				msubAddJointResult(m_txtResult, J1, J2, J3, J4, J5, J6, J7, J8, J9, UT, ValidJ);
			}
			else
			{
				m_txtResult += _T("PosReg2 Error\r\n");
			}
		}

		// Task
		msubAddTaskResult(m_txtResult, pTask);

		// Display read IO result
		m_txtResult += _T("\r\n--- IO ---\r\n");
		if (!blnSDO) 
		{
				strTmp.Format(_T("SDO[%d-%d] : "), 1, 40);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtSDO, 40);
		}
		else
		{
				m_txtResult += _T("READ SDO Error\r\n");
		}
		if (!blnSDO2) 
		{
				strTmp.Format(_T("SDO[%d-%d] : "), 10001, 10040);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtSDO2, 40);
		}
		else
		{
				m_txtResult += _T("READ SDO2 Error\r\n");
		}
		if (!blnSDO3) 
		{
				strTmp.Format(_T("SDO[%d-%d] : "), 11001, 11040);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtSDO3, 40);
		}
		else
		{
				m_txtResult += _T("READ SDO3 Error\r\n");
		}
		if (!blnSDI) 
		{
				strTmp.Format(_T("SDI[%d-%d] : "), 1, 40);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtSDI, 40);
		}
		else
		{
				m_txtResult += _T("READ SDI Error\r\n");
		}
		if (!blnRDO) 
		{
				strTmp.Format(_T("RDO[%d-%d] : "), 1, 8);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtRDO, 8);
		}
		else
		{
				m_txtResult += _T("READ RDO Error\r\n");
		}
		if (!blnRDI) 
		{
				strTmp.Format(_T("RDI[%d-%d] : "), 1, 8);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtRDI, 8);
		}
		else
		{
				m_txtResult += _T("READ RDI Error\r\n");
		}
		if (!blnSO) 
		{
				strTmp.Format(_T("SO[%d-%d] : "), 0, 14);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtSO, 15);
		}
		else
		{
				m_txtResult += _T("READ SO Error\r\n");
		}
		if (!blnSI) 
		{
				strTmp.Format(_T("SI[%d-%d] : "), 0, 14);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtSI, 15);
		}
		else
		{
				m_txtResult += _T("READ SI Error\r\n");
		}
		if (!blnUO) 
		{
				strTmp.Format(_T("UO[%d-%d] : "), 1, 15);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtUO, 15);
		}
		else
		{
				m_txtResult += _T("READ UO Error\r\n");
		}
		if (!blnUI) 
		{
				strTmp.Format(_T("UI[%d-%d] : "), 1, 15);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtUI, 15);
		}
		else
		{
				m_txtResult += _T("READ UI Error\r\n");
		}
		if (!blnGO) 
		{
				strTmp.Format(_T("GO[%d-%d] : "), 1, 3);
				m_txtResult += strTmp;
				msubAddGIOResult(m_txtResult, lngGO, 3);
		}
		else
		{
				m_txtResult += _T("READ GO Error\r\n");
		}
		if (!blnGO2) 
		{
				strTmp.Format(_T("GO[%d-%d] : "), 10001, 10003);
				m_txtResult += strTmp;
				msubAddGIOResult(m_txtResult, lngGO2, 3);
		}
		else
		{
				m_txtResult += _T("READ GO2 Error\r\n");
		}
		if (!blnGI) 
		{
				strTmp.Format(_T("GI[%d-%d] : "), 1, 3);
				m_txtResult += strTmp;
				msubAddGIOResult(m_txtResult, lngGI, 3);
		}
		else
		{
				m_txtResult += _T("READ GI Error\r\n");
		}
		if (!blnAO) 
		{
				strTmp.Format(_T("AO[%d-%d] : "), 1, 3);
				m_txtResult += strTmp;
				msubAddGIOResult(m_txtResult, lngAO, 3);
		}
		else
		{
				m_txtResult += _T("READ AO Error\r\n");
		}
		if (!blnAI) 
		{
				strTmp.Format(_T("AI[%d-%d] : "), 1, 3);
				m_txtResult += strTmp;
				msubAddGIOResult(m_txtResult, lngAI, 3);
		}
		else
		{
				m_txtResult += _T("READ AI Error\r\n");
		}
		if (!blnWO) 
		{
				strTmp.Format(_T("WO[%d-%d] : "), 1, 5);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtWO, 5);
		}
		else
		{
				m_txtResult += _T("READ WO Error\r\n");
		}
		if (!blnWI) 
		{
				strTmp.Format(_T("WI[%d-%d] : "), 1, 5);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtWI, 5);
		}
		else
		{
				m_txtResult += _T("READ WI Error\r\n");
		}
		if (!blnWSI) 
		{
				strTmp.Format(_T("WSI[%d-%d] : "), 1, 5);
				m_txtResult += strTmp;
				msubAddIOResult(m_txtResult, shtWSI, 5);
		}
		else
		{
				m_txtResult += _T("READ WSI Error\r\n");
		}

		// Alarms
		m_txtResult += _T("\r\n");
		for (ii = 1 ; ii <= 5 ; ii++) 
		{
			msubAddAlarmResult(m_txtResult, pAlarm, ii);
		}
		msubAddAlarmResult(m_txtResult, pAlarmCurrent, 1);

		// String Register
		strValue = strSpace.AllocSysString();
		m_txtResult += _T("\r\n--- StrReg ---\r\n");
		for (ii = pStrReg->GetStartIndex() ; ii <= pStrReg->GetEndIndex() ; ii++) 
		{
			if (pStrReg->GetValue(ii, &strValue))
			{
				strTmp.Format(_T("SR[%d] : = %s\r\n"), ii, strValue);
				m_txtResult += strTmp;
			}
			else
			{
				m_txtResult += _T("StrReg Error\r\n");
			}
		}

		// Set result
		UpdateData(FALSE);

		delete pVal;
	}
	CATCH (CException, e)
	{
		e->ReportError();
		msubDisconnected();
	}
	END_CATCH

}

void CFrrjiftestDlg::msubAddGIOResult(CString &pstr, long *buf, int count)
{
	int ii;
	CString strTmp;
	CString strTmp2;
	for (ii = 0 ; ii < count ; ii++) 
	{
		if (ii != 0) strTmp += _T(",");
		strTmp2.Format(_T("%d"), *(buf + ii));
		strTmp += strTmp2;
	}
	pstr += strTmp;
	pstr += _T("\r\n");
}

void CFrrjiftestDlg::msubAddIOResult(CString &pstr, short *buf, int count)
{
	int ii;
	CString strTmp;
	CString strTmp2;
	for (ii = 0 ; ii < count ; ii++) 
	{
		if (ii != 0) strTmp += _T(",");
		strTmp2.Format(_T("%d"), *(buf + ii));
		strTmp += strTmp2;
	}
	pstr += strTmp;
	pstr += _T("\r\n");
}

void CFrrjiftestDlg::msubAddAlarmResult(CString &pstr, _DataAlarm *pAlarm, int count)
{
	short AlarmID, AlarmNumber, CauseID, CauseAlarmNumber, Severity, Year, Month, Day, Hour, Minute, Second;
	BSTR AlarmMessage, CauseAlarmMessage, SeverityMessage;
	int blnRes;
	CString strTmp = _T("                                                                                                   ");

	AlarmMessage = strTmp.AllocSysString();
	CauseAlarmMessage = strTmp.AllocSysString();
	SeverityMessage = strTmp.AllocSysString();
	blnRes = pAlarm->GetValue(count, &AlarmID, &AlarmNumber, &CauseID, &CauseAlarmNumber, &Severity, &Year, &Month, &Day, &Hour, &Minute, &Second, &AlarmMessage, &CauseAlarmMessage, &SeverityMessage); 

	strTmp.Format(_T("-- Alarm %d --\r\n"), count);
	pstr += strTmp;
	if (!blnRes)
	{
		strTmp.Format(_T("Error\r\n"));
		pstr += strTmp;
		return;
	}
	strTmp.Format(_T("%d, %d, %d, %d, %d  "), AlarmID, AlarmNumber, CauseID, CauseAlarmNumber, Severity);
	pstr += strTmp;
	strTmp.Format(_T("%d/%d/%d, %d:%d:%d\r\n"), Year, Month, Day, Hour, Minute, Second);
	pstr += strTmp;

	strTmp = AlarmMessage;
	if (strTmp != _T("")) 
	{
		pstr += strTmp;
		pstr += _T("\r\n");
	}
	strTmp = CauseAlarmMessage;
	if (strTmp != _T("")) 
	{
		pstr += strTmp;
		pstr += _T("\r\n");
	}
	strTmp = SeverityMessage;
	if (strTmp != _T("")) 
	{
		pstr += strTmp;
		pstr += _T("\r\n");
	}

	if (AlarmMessage) ::SysFreeString(AlarmMessage);
	if (CauseAlarmMessage) ::SysFreeString(CauseAlarmMessage);
	if (SeverityMessage) ::SysFreeString(SeverityMessage);
}

void CFrrjiftestDlg::msubAddJointResult(CString &pstr, float J1, float J2, float J3, float J4, float J5, float J6, float J7, float J8, float J9, short UT, short ValidJ)
{
	CString strTmp;

	if (ValidJ)
	{
		strTmp.Format(_T("%f %f %f %f %f %f %f %f %f\r\n"), J1, J2, J3, J4, J5, J6, J7, J8, J9);
		pstr += strTmp;
	}
}

void CFrrjiftestDlg::msubAddXyzwprResult(CString &pstr, float X, float Y, float Z, float W, float P, float R, float E1, float E2, float E3, short C1, short C2, short C3, short C4, short C5, short C6, short C7, short UF, short UT, short validc)
{
	CString strTmp;

	strTmp.Format(_T("UF = %d, UT = %d\r\n"), UF, UT);
	pstr += strTmp;
	if (validc)
	{
		strTmp.Format(_T("%f %f %f %f %f %f %f %f %f\r\n"), X, Y, Z, W, P, R, E1, E2, E3);
		pstr += strTmp;
		if (C1) pstr += _T("F"); else  pstr += _T("N");
		if (C2) pstr += _T("L"); else  pstr += _T("R");
		if (C3) pstr += _T("U"); else  pstr += _T("D");
		if (C4) pstr += _T("T"); else  pstr += _T("B");
		strTmp.Format(_T(" %d %d %d\r\n"), C5, C6, C7);
		pstr += strTmp;
	}
}

void CFrrjiftestDlg::msubAddTaskResult(CString &pstr, _DataTask *pTask)
{
	short LineNumber, State;
	BSTR ProgName, ParentProgName;
	CString strTmp = _T("                                                                                                   ");

	ProgName = strTmp.AllocSysString();
	ParentProgName = strTmp.AllocSysString();

	pTask->GetValue(&ProgName, &LineNumber, &State, &ParentProgName);
    pstr += "\r\n--- Task ---\r\n";

    strTmp.Format(_T("TASK%d : Prog=%s, Line Number=%d, State=%d, ParentProg=%s\r\n"), 1, ProgName, LineNumber, State, ParentProgName);
	pstr += strTmp;

	if (ProgName) ::SysFreeString(ProgName);
	if (ParentProgName) ::SysFreeString(ParentProgName);
}

void CFrrjiftestDlg::OnButtonConnect() 
{
	if (!pCore)
	{
		// connect
		msubInit();
	}
	else
	{
		// disconnect
		pCore->Disconnect();
		msubDisconnected2();
	}
}

void CFrrjiftestDlg::OnCheckLoop() 
{
	
}

void CFrrjiftestDlg::OnTimer(UINT nIDEvent) 
{
	if (m_chkLoop.GetCheck())
	{
		OnButtonRefresh();
	}
	CDialog::OnTimer(nIDEvent);
}

void CFrrjiftestDlg::OnButtonSetNumreg() 
{
	int intRand;
	int ii;
	long lngValues[100];
	float fltValues[100];

	intRand = rand() % 9 + 1;
	for (ii = pNumReg->GetStartIndex() ; ii <= pNumReg->GetEndIndex() ; ii++) 
	{
		lngValues[ii - pNumReg->GetStartIndex()] = (long)(ii * intRand);
	}
	pNumReg->SetValuesInt(pNumReg->GetStartIndex(), lngValues, pNumReg->GetEndIndex() - pNumReg->GetStartIndex() + 1);
	for (ii = pNumReg2->GetStartIndex() ; ii <= pNumReg2->GetEndIndex() ; ii++) 
	{
		fltValues[ii - pNumReg2->GetStartIndex()] = (float)(ii * intRand * 1.1);
	}
	pNumReg2->SetValuesReal(pNumReg2->GetStartIndex(), fltValues, pNumReg2->GetEndIndex() - pNumReg2->GetStartIndex() + 1);

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::OnButtonSetPosregJ() 
{
	int intRand;
	int ii, jj;
	float J[9];

	intRand = rand() % 9 + 1;
	for (ii = pPosReg->GetStartIndex() ; ii <= pPosReg->GetEndIndex() ; ii++) 
	{
		for (jj = 0 ; jj <9 ; jj++) 
		{
			J[jj] = (float)(11.11 * (jj + 1) * intRand * ii);

		}
		pPosReg->SetValueJoint2(ii, J[0], J[1], J[2], J[3], J[4], J[5], J[6], J[7], J[8], -1, -1);
	}

	// Refresh data to get result
	OnButtonRefresh();

}

void CFrrjiftestDlg::OnButtonSetPosregX() 
{
	int intRand;
	int ii, jj;
	float X[9];	// XYZWPR + E1-3
	int C[7]; // Config

	C[0] = C[1] = C[2] = C[3] = 0;
	intRand = rand() % 9 + 1;
	for (ii = pPosReg->GetStartIndex() ; ii <= pPosReg->GetEndIndex() ; ii++) 
	{
		for (jj = 0 ; jj <9 ; jj++) 
		{
			X[jj] = (float)(11.11 * (jj + 1) * intRand * ii);

		}
		C[4] = C[5] = C[6] = ii; // Set turn number of Config
		pPosReg->SetValueXyzwpr2(ii, X[0], X[1], X[2], X[3], X[4], X[5], X[6], X[7], X[8], C[0], C[1], C[2], C[3], C[4], C[5], C[6], -1, -1);
	}

	// Refresh data to get result
	OnButtonRefresh();
	
}

void CFrrjiftestDlg::OnButtonSetPosregX2() 
{
	int intRand;
	int ii, jj;
	float X[9];	// XYZWPR + E1-3
	int C[7]; // Config

	C[0] = C[1] = C[2] = C[3] = 0;
	intRand = rand() % 9 + 1;
	for (ii = pPosRegXyzwpr->GetStartIndex() ; ii <= pPosRegXyzwpr->GetEndIndex() ; ii++) 
	{
		for (jj = 0 ; jj <9 ; jj++) 
		{
			X[jj] = (float)(11.11 * (jj + 1) * intRand * ii);

		}
		C[4] = C[5] = C[6] = ii; // Set turn number of Config
		pPosRegXyzwpr->SetValueXyzwpr2(ii, X[0], X[1], X[2], X[3], X[4], X[5], X[6], X[7], X[8], C[0], C[1], C[2], C[3], C[4], C[5], C[6]);
	}
	// Update to change values at a time.
	if (pPosRegXyzwpr->Update() == 0) 
	{
		AfxMessageBox(_T("pPosRegXyzwpr->Update() failed"));
	}

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::msubSetSDO(int index) 
{
	short shtSDO[40];
	int ii;	

	for (ii = 0 ; ii < 40 ; ii++) 
	{
		shtSDO[ii] = mintSetIOToOn;
	}
	if (mintSetIOToOn) mintSetIOToOn = 0; else mintSetIOToOn = 1;

	if (pLibCore->WriteSDO(index, shtSDO, 40) < 0) 
	{
		AfxMessageBox(_T("Write SDO Error"));
	}

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::OnButtonSetSDO() 
{
	msubSetSDO(1);	
}

void CFrrjiftestDlg::OnButtonSetSDO2() 
{
	msubSetSDO(10001);	
}

void CFrrjiftestDlg::OnButtonSetSDO3() 
{
	msubSetSDO(11001);	
}

void CFrrjiftestDlg::OnButtonSetSDI() 
{
	short shtSDI[40];
	int ii;	

	for (ii = 0 ; ii < 40 ; ii++) 
	{
		shtSDI[ii] = mintSetIOToOn;
	}
	if (mintSetIOToOn) mintSetIOToOn = 0; else mintSetIOToOn = 1;

	if (pLibCore->WriteSDI(1, shtSDI, 40) < 0) 
	{
		AfxMessageBox(_T("Write SDI Error"));
	}

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::OnButtonSetRDO() 
{
	short shtIO[8];
	int ii;	

	for (ii = 0 ; ii < 8 ; ii++) 
	{
		shtIO[ii] = mintSetIOToOn;
	}
	if (mintSetIOToOn) mintSetIOToOn = 0; else mintSetIOToOn = 1;

	if (pLibCore->WriteRDO(1, shtIO, 8) < 0) 
	{
		AfxMessageBox(_T("Write RDO Error"));
	}

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::OnButtonSetRDI() 
{
	short shtIO[8];
	int ii;	

	for (ii = 0 ; ii < 8 ; ii++) 
	{
		shtIO[ii] = mintSetIOToOn;
	}
	if (mintSetIOToOn) mintSetIOToOn = 0; else mintSetIOToOn = 1;

	if (pLibCore->WriteRDI(1, shtIO, 8) < 0) 
	{
		AfxMessageBox(_T("Write RDI Error"));
	}

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::msubSetGO(int index) 
{
	long intG[3];
	int ii;	
	int intRand;

	intRand = rand() % 9 + 1;
	for (ii = 0 ; ii < 3 ; ii++) 
	{
		intG[ii] = intRand * (ii + 1);
	}

	if (pLibCore->WriteGO(index, intG, 3) < 0) 
	{
		AfxMessageBox(_T("Write GO Error"));
	}

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::OnButtonSetGO() 
{
	msubSetGO(1);
}

void CFrrjiftestDlg::OnButtonSetGO2() 
{
	msubSetGO(10001 - 6000);	// Offset -6000 for PMC data table
}

void CFrrjiftestDlg::OnButtonSetGI() 
{
	long intG[3];
	int ii;	
	int intRand;

	intRand = rand() % 9 + 1;
	for (ii = 0 ; ii < 3 ; ii++) 
	{
		intG[ii] = intRand * (ii + 1);
	}

	if (pLibCore->WriteGI(1, intG, 3) < 0) 
	{
		AfxMessageBox(_T("Write GI Error"));
	}

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::OnButtonClearAlarm() 
{
	if (pCore) 
	{
		pCore->ClearAlarm(0);
		// Refresh data to get result
		OnButtonRefresh();
	}
}

void CFrrjiftestDlg::OnButtonSetSysvar() 
{
	long lngOld, lngNew;
	float sngOld, sngNew, sngXOld, sngXNew;
	COleVariant *pValInt;
	COleVariant *pValReal;
	COleVariant *pValStr;
	COleVariant *pValStrNew;
	COleVariant *pValStrConf;
	float X, Y, Z, W, P, R, E1, E2, E3;
	short C1, C2, C3, C4, C5, C6, C7, UF, UT, ValidC;

	TRY
	{
		//init
		pValInt = new COleVariant();
		pValReal = new COleVariant();
		pValStr = new COleVariant();
		pValStrNew = new COleVariant(_T("123456789012"));
		pValStrConf = new COleVariant();

		//store old values
		pDataTable->Refresh();
		if (pSysVarInt2->GetValue(pValInt))
		{
			lngOld = pValInt->intVal;
		}
		else
		{
			AfxMessageBox(_T("pSysVarInt2->GetValue Error"));
			return;
		}
		if (pSysVarReal2->GetValue(pValReal))
		{
			sngOld = pValReal->fltVal;
		}
		else
		{
			AfxMessageBox(_T("pSysVarReal2->GetValue Error"));
			return;
		}
		if (pSysVarString->GetValue(pValStr))
		{
		}
		else
		{
			AfxMessageBox(_T("pSysVarString->GetValue Error"));
			return;
		}
		if (pSysVarPos->GetValueXyzwpr(&X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC))
		{
			sngXOld = X;
		}
		else
		{
			AfxMessageBox(_T("pSysVarPos->GetValue Error"));
			return;
		}

		//make new values
	    lngNew = 999;
		sngNew = sngOld + 1;
		sngXNew = sngXOld + 1;
		X = sngXNew;

		//set write dummy values
		pValInt->intVal = lngNew;
		pSysVarInt2->SetValue(*pValInt);
		pValReal->fltVal = sngNew;
		pSysVarReal2->SetValue(*pValReal);
		pSysVarString->SetValue(*pValStrNew);
		pSysVarPos->SetValueXyzwpr2(X, Y, Z, W, P, R, E1, E2, E3, C1, C2, C3, C4, C5, C6, C7, UF, UT);
    
		//confirm
		pDataTable->Refresh();
		if (pSysVarInt2->GetValue(pValInt))
		{
			ASSERT(lngNew == pValInt->intVal);
		}
		else
		{
			AfxMessageBox(_T("pSysVarInt2->GetValue Error"));
			return;
		}
		if (pSysVarReal2->GetValue(pValReal))
		{
			ASSERT(sngNew == pValReal->fltVal);
		}
		else
		{
			AfxMessageBox(_T("pSysVarReal2->GetValue Error"));
			return;
		}
		if (pSysVarString->GetValue(pValStrConf))
		{
		}
		else
		{
			AfxMessageBox(_T("pSysVarString->GetValue Error"));
			return;
		}
		if (pSysVarPos->GetValueXyzwpr(&X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC))
		{
			ASSERT(sngXNew == X);
		}
		else
		{
			AfxMessageBox(_T("pSysVarPos->GetValue Error"));
			return;
		}
    
		//restore old values
		pValInt->intVal = lngOld;
		pSysVarInt2->SetValue(*pValInt);
		pValReal->fltVal = sngOld;
		pSysVarReal2->SetValue(*pValReal);
		pSysVarString->SetValue(*pValStr);
	    X = sngXOld;
		pSysVarPos->SetValueXyzwpr2(X, Y, Z, W, P, R, E1, E2, E3, C1, C2, C3, C4, C5, C6, C7, UF, UT);
    
		//confirm again
		pDataTable->Refresh();
		if (pSysVarInt2->GetValue(pValInt))
		{
			ASSERT(lngOld == pValInt->intVal);
		}
		else
		{
			AfxMessageBox(_T("pSysVarInt2->GetValue Error"));
			return;
		}
		if (pSysVarReal2->GetValue(pValReal))
		{
			ASSERT(sngOld == pValReal->fltVal);
		}
		else
		{
			AfxMessageBox(_T("pSysVarReal2->GetValue Error"));
			return;
		}
		if (pSysVarString->GetValue(pValStrConf))
		{
		}
		else
		{
			AfxMessageBox(_T("pSysVarString->GetValue Error"));
			return;
		}
		if (pSysVarPos->GetValueXyzwpr(&X, &Y, &Z, &W, &P, &R, &E1, &E2, &E3, &C1, &C2, &C3, &C4, &C5, &C6, &C7, &UF, &UT, &ValidC))
		{
			ASSERT(sngXOld == X);
		}
		else
		{
			AfxMessageBox(_T("pSysVarPos->GetValue Error"));
			return;
		}

		delete pValInt;
		delete pValReal;
		delete pValStr;
		delete pValStrNew;
		delete pValStrConf;
	}
	CATCH (CException, e)
	{
		e->ReportError();
	}
	END_CATCH
}

void CFrrjiftestDlg::OnButtonSetStrreg() 
{
	int intRand;
	int ii;
	CString strTmp;
	int intResult;

	intRand = rand() % 9 + 1;
	for (ii = pStrReg->GetStartIndex() ; ii <= pStrReg->GetEndIndex() ; ii++) 
	{
		strTmp.Format(_T("str%d"), intRand + ii);
		intResult = pStrReg->SetValue(ii, strTmp);
		ASSERT(intResult);
	}

	// Need to call Update to send data.
	intResult = pStrReg->Update();
	ASSERT(intResult);

	// Refresh data to get result
	OnButtonRefresh();
}

void CFrrjiftestDlg::OnButtonRefresh2() 
{
	int intRand;
	int ii;
	long lngValues[100];
	float fltValues[100];
	COleVariant *pVal;
	CString strTmp;

	// Check
	if (pCore == NULL) return;

	// Init
	m_txtResult = "";
	pVal = new COleVariant();

	intRand = rand() % 9 + 1;
	for (ii = pNumReg3->GetStartIndex() ; ii <= pNumReg3->GetEndIndex() ; ii++) 
	{
		lngValues[ii - pNumReg3->GetStartIndex()] = (long)(ii * intRand);
	}
	pNumReg3->SetValuesInt(pNumReg3->GetStartIndex(), lngValues, pNumReg3->GetEndIndex() - pNumReg3->GetStartIndex() + 1);

	//Refresh data table
	pDataTable2->Refresh();

	m_txtResult += _T("\r\n--- NumReg ---\r\n");
	for (ii = pNumReg3->GetStartIndex() ; ii <=pNumReg3->GetEndIndex() ; ii++) 
	{
		if (pNumReg3->GetValue(ii, pVal)) 
		{
			strTmp.Format(_T("R[%d] = %d\r\n"), ii, pVal->intVal);
			m_txtResult += strTmp;
		}
		else
		{
			strTmp.Format(_T("R[%d] : Error\r\n"), ii);
			m_txtResult += strTmp;
		}
	}
	m_txtResult += _T("--- SysVar ---\r\n");
	for (ii = 0 ; ii <= 9 ; ii++) 
	{
		if (pSysVarIntArray[ii]->GetValue(pVal))
		{
			strTmp.Format(_T("%s = %d\r\n"), pSysVarIntArray[ii]->GetSysVarName(), pVal->intVal);
			m_txtResult += strTmp;
		}
		else
		{
			strTmp.Format(_T("%s : Error\r\n"), pSysVarIntArray[ii]->GetSysVarName());
			m_txtResult += strTmp;
		}
	}

	// Set result
	UpdateData(FALSE);

}


void CFrrjiftestDlg::OnBnClickedButtonSetPosregmg()
{
	int intRand;
	int ii, jj;
	float X[9];	// XYZWPR + E1-3
	int C[7]; // Config
	float J[9];

	C[0] = C[1] = C[2] = C[3] = 0;
	intRand = rand() % 9 + 1;
	for (ii = pPosRegMG->GetStartIndex() ; ii <= pPosRegMG->GetEndIndex() ; ii++) 
	{
		for (jj = 0 ; jj <9 ; jj++) 
		{
			X[jj] = (float)(11.11 * (jj + 1) * intRand * ii);

		}
		C[4] = C[5] = C[6] = ii; // Set turn number of Config
		pPosRegMG->SetValueXyzwpr2(ii, 1, X[0], X[1], X[2], X[3], X[4], X[5], X[6], X[7], X[8], C[0], C[1], C[2], C[3], C[4], C[5], C[6]);

		for (jj = 0 ; jj <9 ; jj++) 
		{
			J[jj] = (float)(11.11 * (jj + 1) * intRand * ii);

		}
		pPosRegMG->SetValueJoint2(ii, 2, J[0], J[1], J[2], J[3], J[4], J[5], J[6], J[7], J[8]);
	}
	// Update to change values at a time.
	if (pPosRegMG->Update() == 0) 
	{
		AfxMessageBox(_T("pPosRegMG->Update() failed"));
	}

	// Refresh data to get result
	OnButtonRefresh();
}
