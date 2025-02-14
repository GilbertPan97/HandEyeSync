#ifndef LINE_SCANNER_INTERFACE_H
#define LINE_SCANNER_INTERFACE_H

#include "GocatorCommon.h"

#include <string>
#include <vector>
#include <memory>

// Define camera status enumeration
enum class CameraStatus {
    DEV_READY,         // Camera is ready and functioning properly
    DEV_ERROR,         // Camera encountered an error
    DEV_NOT_CONNECTED, // Camera is not connected
};

// Define camera branch enumeration
enum class CameraBranch {
    LMI,   // LMI camera branch
    SSZN,  // SSZN camera branch
};

// Struct to hold camera information
struct CameraInfo {
    int id;              // Unique identifier for the camera
    std::string brand;   // Camera brand
    std::string ipAddress; // IP address of the camera
    bool isConnected;    // Connection status (true if connected, false otherwise)

    // Default constructor
    CameraInfo() 
        : id(-1), brand("N/A"), ipAddress("0.0.0.0"), isConnected(false) {} // Initialize with default values

    // Parameterized constructor
    CameraInfo(int cameraId, const std::string& cameraBrand, const std::string& ip)
        : id(cameraId), brand(cameraBrand), ipAddress(ip), isConnected(false) {} // Default isConnected to false
};

// Define the Line Scanner Interface class
class LineScannerInterface {
private:
    Gocator_Handle gocator;    // Handle for the specific camera
    Gocator_Data data;         // Data from the camera scan

public:
    // Default constructor, initializes the camera handle and prepares the interface.
    LineScannerInterface();
    
    // Destructor, cleans up resources and shuts down the camera.
    ~LineScannerInterface();

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
     * @brief Captures one frame of data from the camera.
     * 
     * @return CameraStatus The status of the data capture process.
     */
    CameraStatus GrabOnce();

    /**
     * @brief Turns the camera laser on or off.
     * 
     * @param open Boolean flag to control the laser (true to open, false to close).
     * @return CameraStatus The status of the operation.
     */
    CameraStatus SetStatus(bool open);

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
     * @brief Retrieves the data captured by the camera.
     * 
     * @return Gocator_Data The data obtained from the camera.
     */
    Gocator_Data RetriveData();

    /**
     * @brief Shuts down the camera and releases any associated resources.
     */
    void Shutdown();

private:
    std::vector<CameraInfo> ConvertToCameraInfoList(const Gocator_List& sdkCameraList);
    void RemoveInvalidPoints();
};

#endif // LINE_SCANNER_INTERFACE_H
