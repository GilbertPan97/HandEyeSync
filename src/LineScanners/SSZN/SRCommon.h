#ifndef SR_COMMON_H
#define SR_COMMON_H

#include "clog.h"
#include "SR7Link.h"

#include <stdint.h> 
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SsznProfilePoint {
    double x;   // x-coordinate in engineering units (mm)
    double z;   // z-coordinate in engineering units (mm)
    unsigned char intensity;
} SsznProfilePoint;

typedef struct {
    SsznProfilePoint* profileBuffer;    // buffer for profile points
    size_t pointCount;              // number of valid profile points
    size_t bufferSize;              // number of original buffer points
} Sszn_Data;

typedef struct {
    int DEVICE_ID;
    SR7IF_ETHERNET_CONFIG SREthernetConFig;
    SR7IF_Data DataObject;
} Sszn_Handle;

// Structure to hold camera information
typedef struct {
    int id;                     // Camera ID
    char ipAddress[64];         // Camera IP address
} Sszn_Info;

// Structure to hold a list of discovered cameras
typedef struct {
    Sszn_Info* cam_info;        // Pointer to an array of CameraInfo
    size_t count;               // Number of discovered cameras
} Sszn_List;

/* ====================== Function declarations =========================== */
bool Sszn_Initialize(Sszn_Handle* handle);

bool Sszn_Discover(Sszn_List* cameraList);

bool Sszn_Open(Sszn_Handle* handle, const char* sensorIp, int deviceId);

bool Sszn_ReceiveProfileData(Sszn_Handle* handle, Sszn_Data* data);

#ifdef __cplusplus
}
#endif

#endif