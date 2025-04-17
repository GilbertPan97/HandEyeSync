#ifndef FRROBOTINTERFACE_H
#define FRROBOTINTERFACE_H

#include "frrjif.h"
#include "frrobotiflib.h"

// #if !defined(AFX_FRRJIFTESTDLG_H__926EFAB4_D344_46F7_9678_51E788764D55__INCLUDED_)
// #define AFX_FRRJIFTESTDLG_H__926EFAB4_D344_46F7_9678_51E788764D55__INCLUDED_
// #endif

class FrRobotInterface {
private:
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

public:
    FrRobotInterface();
    ~FrRobotInterface();

    BOOL Connect(CString hostname);

    BOOL DisConnect();

private:
    BOOL IsValidIPAddress(const CString& ipAddress);

};

#endif      // FRROBOTINTERFACE_H