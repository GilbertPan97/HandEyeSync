#include "SRCommon.h"

// Function to check if the given SR7_DEV_SETTING_MAP structure is valid
bool Sszn_isValidSR7DevSettingMap(const SR7_DEV_SETTING_MAP *map) {
    if (!map) {
        return EXIT_FAILURE;
    }
    
    // Ensure DEVICE_ID, PROG, SETTING_PAGE, and PAGE_ITEM are non-negative
    if (map->DEVICE_ID < 0 || map->PROG < -1 || 
        map->SETTING_PAGE < 0 || map->PAGE_ITEM < 0) {
        return EXIT_FAILURE;
    }
    
    // Validate ITEM_TARGET values (example: must be within a specific range)
    bool dev_target_status = EXIT_FAILURE;
    for (int i = 0; i < 4; i++) {
        if (map->DEV_TARGET[i] != -1) {
            dev_target_status = EXIT_SUCCESS;
            break;
        }
    }
    
    return EXIT_SUCCESS;
}

// Function to initialize the SR7_DEV_SETTING_MAP structure with default negative values
void Sszn_initSR7DevSettingMap(SR7_DEV_SETTING_MAP *map) {
    if (map) {
        map->DEVICE_ID = -1;
        map->PROG = -1;
        map->SETTING_PAGE = -1;
        map->PAGE_ITEM = -1;
        for (int i = 0; i < 4; i++) {
            map->DEV_TARGET[i] = -1;
        }
    }
}

bool Sszn_GetDevSetting(SR7_DEV_SETTING_MAP *map, int data_size, int* parameter) {

    if (Sszn_isValidSR7DevSettingMap(map) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (!parameter) {
        clog("parameter pointer is null!");
        return EXIT_FAILURE;
    }

    int result;
    if (data_size > 0) { 
        result =  SR7IF_GetSetting(map->DEVICE_ID, map->PROG, map->SETTING_PAGE, 
            map->PAGE_ITEM, map->DEV_TARGET, parameter, data_size);
    }
    else {
        clog("Unknown data size for Sszn_GetDevSetting.");
        return EXIT_FAILURE;
    }

    if (result < 0) {
        clog("SR7IF_GetSetting Fail.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool Sszn_GetROI(Sszn_Handle* handle, double* xzRange) {
    // Validate input parameters
    if (handle == NULL) {
        return false;
    }

    // Initialize data object
    SR7IF_Data DataObject = {0};

    // Get the profile point count
    int profilePointCount = SR7IF_ProfileDataWidth(handle->DEVICE_ID, DataObject);
    // Get the X-axis pitch value
    double xPitch = SR7IF_ProfileData_XPitch(handle->DEVICE_ID, DataObject);

    // Validate profile data
    if (profilePointCount <= 0 || xPitch <= 0) {
        return false;
    }

    // Calculate ROI range: [x_start, x_end, y_start, y_end]
    xzRange[0] = 0.0;
    xzRange[1] = xPitch * profilePointCount;
    xzRange[2] = -23.0;     // TODO: Get z-direction range from sensor
    xzRange[3] = 23.0;

    return true;
}