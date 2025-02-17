#ifndef GOCATOR_COMMON_H
#define GOCATOR_COMMON_H

#include "clog.h"
#include "../SensorCommon.h"

#include <GoSdk/GoSdk.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RECEIVE_TIMEOUT         (20000000) 
#define INVALID_RANGE_16BIT     ((signed short)0x8000)          
#define DOUBLE_MAX              ((k64f)1.7976931348623157e+308) 
#define INVALID_RANGE_DOUBLE    ((k64f)-DOUBLE_MAX) 

#define NM_TO_MM(VALUE) (((k64f)(VALUE))/1000000.0)
#define UM_TO_MM(VALUE) (((k64f)(VALUE))/1000.0)

typedef struct {
    kAssembly api;                // Gocator API assembly
    GoSystem system;              // Gocator system
    GoSensor sensor;              // Gocator sensor
    GoSetup setup;
} Gocator_Handle;

/* ====================== Function declarations =========================== */ 
kStatus Gocator_Initialize(Gocator_Handle* handle);

kStatus Gocator_Connect(Gocator_Handle* handle, const char* sensorIp);

kStatus Gocator_Open(Gocator_Handle* handle);

kStatus Gocator_Close(Gocator_Handle* handle);

kStatus Gocator_DisConnect(Gocator_Handle* handle, ...);

kStatus Gocator_ReceiveProfileData(Gocator_Handle* handle, ProfileData* data);

void Gocator_Cleanup(Gocator_Handle* handle);

// Function to discover cameras
kStatus Gocator_Discover(Sensor_List* cameraList);

// Function to free the camera list
void Gocator_FreeList(Sensor_List* cameraList);

#ifdef __cplusplus
}
#endif

#endif // GOCATOR_COMMON_H
