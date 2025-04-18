#pragma once

// Robot Communication Library - Generated from FANUC robot interface sample code
// Based on frrjif.h/cpp and frrobotiflib.h/cpp 
// Uses .NET interop to communicate with FANUC robots

// Include .NET interop headers
#using <System.dll>
#using <System.Core.dll>

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

using namespace System;

// Namespace
namespace RobotComm {

    // Version information
    extern const TCHAR* Version;

    // Initialize .NET Framework (call at the beginning of the application)
    // No need for CoInitialize since it's managed by the .NET runtime
    inline void Initialize() {
        // This function can be left empty if .NET manages initialization internally
    }

    // Release .NET resources (call at the end of the application)
    // No need for CoUninitialize since it's managed by the .NET runtime
    inline void Uninitialize() {
        // This function can be left empty if .NET manages uninitialization internally
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
