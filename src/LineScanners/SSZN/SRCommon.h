#ifndef SR_COMMON_H
#define SR_COMMON_H

#include "clog.h"
#include "SR7Link.h"
#include "../SensorCommon.h"

#include <stdint.h> 
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int DEVICE_ID;
    SR7IF_ETHERNET_CONFIG SREthernetConFig;
    SR7IF_Data DataObject;
} Sszn_Handle;

/* ====================== Function declarations =========================== */
bool Sszn_Initialize(Sszn_Handle* handle);

bool Sszn_Discover(Sensor_List* cameraList);

void Sszn_FreeList(Sensor_List* cameraList);

bool Sszn_Open(Sszn_Handle* handle, const char* sensorIp, int deviceId);

bool Sszn_Close(Sszn_Handle* handle);

bool Sszn_ReceiveProfileData(Sszn_Handle* handle, ProfileData* data);

#ifdef __cplusplus
}
#endif

#endif