#include "SR7Link.h"

typedef struct SsznProfilePoint {
    double x;   // x-coordinate in engineering units (mm)
    double z;   // z-coordinate in engineering units (mm)
    unsigned char intensity;
} SsznProfilePoint;

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

