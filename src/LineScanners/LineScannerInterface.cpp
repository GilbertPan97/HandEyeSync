#include "LineScannerInterface.h"
#include "SensorCommon.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

LineScannerInterface::LineScannerInterface() {
    // Create lmi and sszn camera hanle
    gocator_ = Gocator_Handle();
    kStatus go_status = Gocator_Initialize(&gocator_);

    sszn_ = Sszn_Handle();
    bool ss_status = Sszn_Initialize(&sszn_);

    if (go_status != kOK && ss_status != EXIT_SUCCESS){
        clog("Error: Sensor handle initial fail.");
    }
}

LineScannerInterface::LineScannerInterface(const std::string brand){
    // Create lmi and sszn camera hanle
    gocator_ = Gocator_Handle();
    kStatus go_status = Gocator_Initialize(&gocator_);

    sszn_ = Sszn_Handle();
    bool ss_status = Sszn_Initialize(&sszn_);

    if (go_status != kOK && ss_status != EXIT_SUCCESS){
        clog("Error: Sensor handle initial fail.");
    }

    SetBrand(brand);      // Set Current Sensor Brand
}

LineScannerInterface::~LineScannerInterface() {
    SetStatus(false);
    Shutdown();
}

void LineScannerInterface::SetBrand(const std::string& brand) {
    if (brand == "LMI") {
        curBrand_ = CameraBrand::LMI;  // Set to LMI if the input string is "LMI"
        clog("Info: Set current sensor brand to LMI");
    } else if (brand == "SSZN") {
        curBrand_ = CameraBrand::SSZN; // Set to SSZN if the input string is "SSZN"
        clog("Info: Set current sensor brand to SSZN");
    } else {
        clog("Error: Unknown sensor brand");
    }
}

CameraStatus LineScannerInterface::Scan(std::vector<CameraInfo>& cameraList) {
    // C data type for camera list
    Sensor_List cList;
    cList.cam_info = nullptr;
    cList.count = 0;

    // Check which brand to use and call the appropriate discovery function
    if (curBrand_ == CameraBrand::LMI) {
        Gocator_Discover(&cList); // Use Gocator_Discover for LMI cameras
    } else if (curBrand_ == CameraBrand::SSZN) {
        Sszn_Discover(&cList); // Use Sszn_Discover for SSZN cameras
    } else {
        // If an unknown brand is set, return an error status
        return CameraStatus::DEV_ERROR;
    }

    // Populate the output vector with discovered cameras
    cameraList = ConvertToCameraInfoList(cList);

    return CameraStatus::DEV_READY; // Return ready status after successful discovery
}

CameraStatus LineScannerInterface::Connect(const std::string& cameraIp) {
    // Duplicate the camera IP to pass it to the connection function
    const char* ip_s = _strdup(cameraIp.c_str());
    kStatus status;

    // Check which brand to use and call the appropriate connection function
    if (curBrand_ == CameraBrand::LMI) {
        status = Gocator_Connect(&gocator_, ip_s); // LMI cameras need connect before open
    } else if (curBrand_ == CameraBrand::SSZN) {
        // SSZN cameras don't require explicit connection
        clog("Info: SSZN camera does not require explicit connection, it will open directly.");
        if(Sszn_Open(&sszn_, ip_s, 0)==EXIT_SUCCESS)      // FIXME: Divice id should be dynamical for multi sensor connect
            status = kOK;
    } else {
        // If an unknown brand is set, return an error status
        free((void*)ip_s);                  // Free the duplicated string
        return CameraStatus::DEV_ERROR;     // Unknown brand type
    }

    free((void*)ip_s);      // Free the duplicated string after using it

    return (status == kOK) ? CameraStatus::DEV_READY : CameraStatus::DEV_NOT_CONNECTED;
}

// Disconnect the camera without IP address
CameraStatus LineScannerInterface::Disconnect() {
    kStatus status;

    // Check the current camera brand and call the appropriate disconnect function
    if (curBrand_ == CameraBrand::LMI) {
        status = Gocator_DisConnect(&gocator_);  // Disconnect the LMI camera
    } else if (curBrand_ == CameraBrand::SSZN) {
        // SSZN camera does not require explicit disconnection
        clog("Info: SSZN camera does not require explicit disconnection, it will close directly.");
        if(Sszn_Close(&sszn_)==EXIT_SUCCESS)
            status = kOK;
    } else {
        return CameraStatus::DEV_ERROR;  // Unsupported camera brand
    }

    // Return the corresponding camera status based on the disconnection result
    return (status == kOK) ? CameraStatus::DEV_READY : CameraStatus::DEV_ERROR;
}

// Disconnect the camera with IP address
CameraStatus LineScannerInterface::Disconnect(const std::string& cameraIp) {
    const char* ip_s = _strdup(cameraIp.c_str());  // Duplicate the IP address for the function call
    kStatus status;

    // Check the current camera brand and call the appropriate disconnect function
    if (curBrand_ == CameraBrand::LMI) {
        status = Gocator_DisConnect(&gocator_, ip_s);  // Disconnect the LMI camera
    } else if (curBrand_ == CameraBrand::SSZN) {
        // SSZN camera does not require explicit disconnection
        clog("Info: SSZN camera does not require explicit disconnection, it will close directly.");
        if(Sszn_Close(&sszn_)==EXIT_SUCCESS)
            status = kOK;
    } else {
        free((void*)ip_s);  // Free the duplicated IP address memory
        return CameraStatus::DEV_ERROR;  // Unsupported camera brand
    }

    free((void*)ip_s);  // Free the duplicated IP address memory

    // Return the corresponding camera status based on the disconnection result
    return (status == kOK) ? CameraStatus::DEV_NOT_CONNECTED : CameraStatus::DEV_ERROR;
}

std::array<double, 4> LineScannerInterface::GetSensorROI() {
    double xzRange[4];  // Array to store [x_start, x_end, z_start, z_end]
    auto sta = Sszn_GetROI(&sszn_, xzRange);  // Retrieve ROI range from the sensor

    // Check if ROI retrieval was successful
    if (!sta) {
        // Handle error (could log an error or throw an exception if necessary)
        return {0.0, 0.0, 0.0, 0.0};
    }

    // Construct the ROI array using the retrieved values
    std::array<double, 4> xzRangeArr = {xzRange[0], xzRange[1], xzRange[2], xzRange[3]};

    return xzRangeArr;
}

CameraStatus LineScannerInterface::SetStatus(bool open) {
    kStatus status;     // For LMI, we expect a kStatus return value

    // Check the current camera brand and perform actions accordingly
    if (curBrand_ == CameraBrand::LMI) {
        // For LMI cameras, open or close based on the input flag
        if (open) {
            status = Gocator_Open(&gocator_);
        } else {
            status = Gocator_Close(&gocator_);
        }
        status = kOK;
        clog("Info: Set status successfully (status: %s).\n", open ? "true" : "false");
    } 
    else if (curBrand_ == CameraBrand::SSZN) {
        /* For SSZN cameras, explicitly open or close has been process when camera connect */

        // Check SSZN sensor batch processing mode (batch == 2.5D) and switch
        // BUG: Not finish Sszn_GetDevSetting debug
        SR7_DEV_SETTING_MAP setting_map;
        Sszn_initSR7DevSettingMap(&setting_map);

        setting_map.DEVICE_ID = sszn_.DEVICE_ID;
        setting_map.PROG = -1;      // Return Current Using Feature Map
        memset(setting_map.DEV_TARGET, 0, sizeof(setting_map.DEV_TARGET));    // BUG: How to match sszn_.DEVICE_ID and DEV_TARGET
        
        // 1. Check Batch Processing Switch
        setting_map.SETTING_PAGE = 0x00;
        setting_map.PAGE_ITEM = 0x03;
        int p_size = 1;
        int parameter = 10;
        if (Sszn_GetDevSetting(&setting_map, p_size, &parameter) == EXIT_FAILURE) {
            throw std::runtime_error("Get sszn device setting fail.");
        }

        if (parameter == 1) {
            // BUG: throw will get memory error.
            throw std::runtime_error("Batch processing switch should be closed bofore open sszn sensor.");
        }

        // 2. Check Batch Processing Mode
        setting_map.SETTING_PAGE = 0x30;
        setting_map.PAGE_ITEM = 0x03;
        int p_size1 = 1;
        int parameter1 = 10;
        if (Sszn_GetDevSetting(&setting_map, p_size1, &parameter1) == EXIT_FAILURE) {
            throw std::runtime_error("Get sszn device setting fail.");
        }

        if (parameter1 == 0) {
            throw std::runtime_error("Batch processing mode should be set as 2.5D.");
        }

        status = kOK;
    } 
    else {
        return CameraStatus::DEV_ERROR;  // Unsupported camera brand
    }
        
    // Return the corresponding camera status based on the disconnection result
    return (status == kOK) ? CameraStatus::DEV_READY : CameraStatus::DEV_ERROR;
}

CameraStatus LineScannerInterface::GrabOnce() {
    bool success;
    
    if (curBrand_ == CameraBrand::LMI) {
        // For LMI cameras, use the Gocator function to receive profile data
        if (Gocator_ReceiveProfileData(&gocator_, &profile_) == kOK)
            success = EXIT_SUCCESS;
    } 
    else if (curBrand_ == CameraBrand::SSZN) {
        // For SSZN cameras, use the SSZN-specific function (assuming it's named similarly)
        if (Sszn_ReceiveProfileData(&sszn_, &profile_) == EXIT_SUCCESS){
            success = EXIT_SUCCESS;
        }
        else {
            throw std::runtime_error("SSZN grab failed. Please check measurement mode \
                and whether batch processing are turned off.");
        }
    } 
    else {
        return CameraStatus::DEV_ERROR;  // Unsupported camera brand
    }

    // Check the result of the data retrieval
    if (success == EXIT_FAILURE) {
        clog("Error: ReceiveProfileData failed, skip single grab.");
        return CameraStatus::DEV_ERROR;  // Return error if data retrieval fails
    }

    // Optionally, remove invalid points if necessary (currently commented out)
    // RemoveInvalidPoints();

    return CameraStatus::DEV_READY;  // Successfully retrieved the profile data
}


ProfileData LineScannerInterface::RetriveData() {
    if (profile_.validPoints > 0){
        // clog("Info: Valid points - %d", profile_.validPoints);
        return profile_;
    } 
    else {
        clog("Error: No sensor data can be retrive.");
        return profile_;
    }
}

void LineScannerInterface::Shutdown() {
    // Shutdown the camera and free resources
    if (gocator_.api != NULL) {
        Gocator_Cleanup(&gocator_);
        clog("Device closed down.");
    }

    // Shutdown SSZN camera and free resources
    if (sszn_.DEVICE_ID != 0) {
        // Reset the configuration and data object
        sszn_.DEVICE_ID = -1;
        memset(&sszn_.SREthernetConFig, 0, sizeof(SR7IF_ETHERNET_CONFIG));  // Clear ethernet config
        memset(&sszn_.DataObject, 0, sizeof(SR7IF_Data));  // Clear data object
        clog("SSZN device closed down.");
    }
}

// Function to convert GoSdk_CameraList to std::vector<CameraInfo>
std::vector<CameraInfo> LineScannerInterface::ConvertToCameraInfoList(const Sensor_List& sdkCameraList) {
    std::vector<CameraInfo> cameraInfoList;

    // Iterate through the GoSdk_CameraList
    for (size_t i = 0; i < sdkCameraList.count; ++i) {
        CameraInfo cameraInfo;
        cameraInfo.id = sdkCameraList.cam_info[i].id; // Copy the ID
        cameraInfo.ipAddress = sdkCameraList.cam_info[i].ipAddress; // Copy the IP address

        if (curBrand_==CameraBrand::LMI)
            cameraInfo.brand = "LMI";
        else if(curBrand_==CameraBrand::SSZN)
            cameraInfo.brand = "SSZN";
        else
            cameraInfo.brand = "N/A";

        // Add the cameraInfo to the vector
        cameraInfoList.push_back(cameraInfo);
    }

    return cameraInfoList; // Return the populated vector
}

// void LineScannerInterface::RemoveInvalidPoints() {
//     // Create a temporary container to store valid ProfilePoint
//     std::vector<ProfilePoint> validPoints;

//     // Iterate through profileBuffer and filter out invalid points
//     for (size_t i = 0; i < profile_.validPoints; ++i) {
//         // std::cout << "Point-[ " << i << " ]: x = " << data.profileBuffer[i].x
//         //           << ", y = [ " << data.profileBuffer[i].z << " ]\n";
//         // Check if both x and z are not equal to INVALID_RANGE_DOUBLE
//         if (profile_.profileBuffer[i].x != INVALID_RANGE_DOUBLE && 
//             profile_.profileBuffer[i].z != INVALID_RANGE_DOUBLE) {
//             validPoints.push_back(profile_.profileBuffer[i]); // Add valid point to the temporary container
//         }
//     }

//     // Update validPoints to the number of valid points
//     profile_.validPoints = validPoints.size();

//     // Reallocate profileBuffer to store valid points
//     if (profile_.validPoints > 0) {
//         // Allocate memory for the valid points
//         profile_.profileBuffer = new ProfilePoint[profile_.validPoints];
//         std::copy(validPoints.begin(), validPoints.end(), profile_.profileBuffer); // Copy valid points to profileBuffer
//     } else {
//         // If no valid points, set profileBuffer to nullptr
//         delete[] profile_.profileBuffer; // Free existing memory
//         profile_.profileBuffer = nullptr;
//     }
// }

