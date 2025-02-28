#ifndef LINE_SCANNER_INTERFACE_H
#define LINE_SCANNER_INTERFACE_H

#include "GocatorCommon.h"
#include "SRCommon.h"

#include <string>
#include <vector>
#include <memory>

// Define camera status enumeration
enum class CameraStatus {
    DEV_READY,         // Camera is ready and functioning properly
    DEV_ERROR,         // Camera encountered an error
    DEV_NOT_CONNECTED, // Camera is not connected
};

// Define camera brand enumeration
enum class CameraBrand {
    LMI,   // LMI camera brand
    SSZN,  // SSZN camera brand
};

// Struct to hold camera information
struct CameraInfo {
    int id;              // Unique identifier for the camera
    std::string brand;   // Camera brand
    std::string ipAddress; // IP address of the camera
    bool isConnected;    // Connection status (true if connected, false otherwise)

    // Default constructor
    CameraInfo() 
        : id(-1), brand("N/A"), ipAddress("0.0.0.0"), isConnected(false) {}

    // Parameterized constructor
    CameraInfo(int cameraId, const std::string& cameraBrand, const std::string& ip)
        : id(cameraId), brand(cameraBrand), ipAddress(ip), isConnected(false) {}
};

// Define the Line Scanner Interface class
class LineScannerInterface {
private:
    Gocator_Handle gocator_;    // Handle for the specific camera
    Sszn_Handle sszn_;          // Handle for the specific camera
    CameraBrand curBrand_;

    ProfileData profile_;         // Data from the camera scan

public:
    // Default constructor, initializes the camera handle and prepares the interface.
    LineScannerInterface();
    LineScannerInterface(const std::string brand);
    
    // Destructor, cleans up resources and shuts down the camera.
    ~LineScannerInterface();

    /**
     * @brief Sets the brand of the camera based on a string input.
     * 
     * This function allows you to set the brand type of the camera using a string. 
     * For example, passing "LMI" sets the brand to LMI, and passing "SSZN" sets it to SSZN.
     * 
     * @param brand A string representing the brand type to be set for the camera.
     */
    void SetBrand(const std::string& brand);

    /**
     * @brief Scans for available cameras and populates the camera list.
     * 
     * @param cameraList A vector to hold the camera information (CameraInfo).
     * @return CameraStatus The status of the scanning process.
     */
    CameraStatus Scan(std::vector<CameraInfo>& cameraList);

    /**
     * @brief Connects to a camera using the given IP address.
     * 
     * @param cameraIp The IP address of the camera to connect to.
     * @return CameraStatus The status of the connection process.
     */
    CameraStatus Connect(const std::string& cameraIp);

    /**
     * @brief Disconnects the camera. This version uses the camera's IP address.
     * 
     * @return CameraStatus The status of the disconnection process.
     */
    CameraStatus Disconnect();

    /**
     * @brief Disconnects the camera using a specific IP address.
     * 
     * @param cameraIp The IP address of the camera to disconnect from.
     * @return CameraStatus The status of the disconnection process.
     */
    CameraStatus Disconnect(const std::string& cameraIp);

    /**
     * @brief Turns the camera laser on or off.
     * 
     * @param open Boolean flag to control the laser (true to open, false to close).
     * @return CameraStatus The status of the operation.
     */
    CameraStatus SetStatus(bool open);

    /**
     * @brief Captures one frame of data from the camera.
     * 
     * @return CameraStatus The status of the data capture process.
     */
    CameraStatus GrabOnce();

    /**
     * @brief Retrieves the data captured by the camera.
     * 
     * @return Gocator_Data The data obtained from the camera.
     */
    ProfileData RetriveData();

    /**
     * @brief Shuts down the camera and releases any associated resources.
     */
    void Shutdown();

private:
    std::vector<CameraInfo> ConvertToCameraInfoList(const Sensor_List& sdkCameraList);
    // void RemoveInvalidPoints();
};

#endif // LINE_SCANNER_INTERFACE_H
