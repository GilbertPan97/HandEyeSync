#pragma once

// Robot Communication Library - Generated from FANUC robot interface sample code
// Based on frrjif.h/cpp and frrobotiflib.h/cpp 
// Uses direct IDispatch calls to communicate with FANUC robots

// Include ATL core headers
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

// Namespace
namespace RobotComm {

// Version information
extern const TCHAR* Version;

// Initialize COM library (call at the beginning of the application)
inline HRESULT Initialize() {
    return ::CoInitialize(NULL);
}

// Release COM library (call at the end of the application)
inline void Uninitialize() {
    ::CoUninitialize();
}

// Forward declarations
class Core;
class DataTable;
class LibCore;
class DataNumReg;
class DataPosReg;
class DataPosRegXyzwpr;
class DataPosRegMG;
class DataCurPos;
class DataTask;
class DataSysVar;
class DataSysVarPos;
class DataAlarm;
class DataString;

} // namespace RobotComm

// Constants
#include "RobotComm_Constants.h"

// Include all component headers
#include "libCore/RobotComm_LibCore.h"
#include "RobotComm_Core.h"
#include "RobotComm_DataTable.h"
#include "RobotComm_DataNumReg.h"
#include "RobotComm_DataPosReg.h"
#include "RobotComm_DataPosRegXyzwpr.h"
#include "RobotComm_DataPosRegMG.h"
#include "RobotComm_DataCurPos.h"
#include "RobotComm_DataTask.h"
#include "RobotComm_DataSysVar.h"
#include "RobotComm_DataSysVarPos.h"
#include "RobotComm_DataAlarm.h"
#include "RobotComm_DataString.h" 