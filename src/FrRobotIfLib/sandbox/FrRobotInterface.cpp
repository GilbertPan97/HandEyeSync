#include "FrRobotInterface.h"

FrRobotInterface::FrRobotInterface() {
    // Make core object
    // pCore = new _Core;
    // if (!pCore->CreateDispatch(_T("FRRJIF.Core"))) 
    // {
    //     AfxMessageBox(_T("Unable to create FRRJIF.Core"));
    //     pCore = NULL;
    //     return ;
    // }
    // pLibCore = new ILibCore(pCore->GetLib());

    // // Make data table
    // pDataTable = new _DataTable(pCore->GetDataTable());

    // // Set data table items
    // pAlarm = new _DataAlarm(pDataTable->AddAlarm(ALARM_LIST, 5, 0));
    // pAlarmCurrent = new _DataAlarm(pDataTable->AddAlarm(ALARM_CURRENT, 1, 0));
    // pCurPos = new _DataCurPos(pDataTable->AddCurPos(CURPOS, 1));
    // pCurPosUF = new _DataCurPos(pDataTable->AddCurPosUF(CURPOS, 1, 15));
    // pNumReg = new _DataNumReg(pDataTable->AddNumReg(NUMREG_INT, 1, 5));
    // pNumReg2 = new _DataNumReg(pDataTable->AddNumReg(NUMREG_REAL, 6, 10));
    // pPosReg = new _DataPosReg(pDataTable->AddPosReg(POSREG, 1, 1, 5));
    // pPosReg2 = new _DataPosReg(pDataTable->AddPosReg(POSREG, 2, 1, 5));
    // pPosRegXyzwpr = new _DataPosRegXyzwpr(pDataTable->AddPosRegXyzwpr(POSREG_XYZWPR, 1, 1, 5));
    // pPosRegMG = new _DataPosRegMG(pDataTable->AddPosRegMG(POSREGMG, _T("C,J6"), 1, 5));
    // pSysVarInt = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_INT, _T("$FAST_CLOCK")));
    // pSysVarInt2 = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_INT, _T("$TIMER[10].$TIMER_VAL")));
    // pSysVarReal = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_REAL, _T("$MOR_GRP[1].$CURRENT_ANG[1]")));
    // pSysVarReal2 = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_REAL, _T("$DUTY_TEMP")));
    // pSysVarString = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_STRING, _T("$TIMER[10].$COMMENT")));
    // pSysVarPos = new _DataSysVarPos(pDataTable->AddSysVarPos(SYSVAR_POS, _T("$MNUTOOL[1,1]")));
    // pVarString = new _DataSysVar(pDataTable->AddSysVar(SYSVAR_STRING, _T("$[HTTPKCL]CMDS[1]")));
    // pTask = new _DataTask(pDataTable->AddTask(TASK, 1));
    // pStrReg = new _DataString(pDataTable->AddString(STRREG, 1, 5));

    // // 2nd data table.
    // // You must not set the first data table.
    // pDataTable2 = new _DataTable(pCore->GetDataTable2());
    // pNumReg3 = new _DataNumReg(pDataTable2->AddNumReg(NUMREG_INT, 1, 5));
    // pSysVarIntArray[0] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[1].$TIMER_VAL")));
    // pSysVarIntArray[1] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[2].$TIMER_VAL")));
    // pSysVarIntArray[2] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[3].$TIMER_VAL")));
    // pSysVarIntArray[3] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[4].$TIMER_VAL")));
    // pSysVarIntArray[4] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[5].$TIMER_VAL")));
    // pSysVarIntArray[5] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[6].$TIMER_VAL")));
    // pSysVarIntArray[6] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[7].$TIMER_VAL")));
    // pSysVarIntArray[7] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[8].$TIMER_VAL")));
    // pSysVarIntArray[8] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[9].$TIMER_VAL")));
    // pSysVarIntArray[9] = new _DataSysVar(pDataTable2->AddSysVar(SYSVAR_INT, _T("$TIMER[10].$TIMER_VAL")));

}

FrRobotInterface::~FrRobotInterface() {
    
}