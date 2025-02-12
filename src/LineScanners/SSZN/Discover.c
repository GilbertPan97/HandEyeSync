#include "SRCommon.h"

#include <stdio.h>
#include <stdlib.h>

// Function to discover cameras and add them to the camera list
void Sszn_Discover(Sszn_List* cameraList) {
    int ReadNum = 0; // Number of devices found
    int timeOut = 1000; // Timeout in milliseconds, adjust as necessary
    
    // Call the function to search for cameras online
    SR7IF_ETHERNET_CONFIG *pDevices = SR7IF_SearchOnline(&ReadNum, timeOut);
    
    // Check if no devices were found
    if (pDevices == NULL) {
        printf("No devices found.\n");
        return;
    }

    // Allocate memory for the camera list if it is not already allocated
    cameraList->cam_info = (Sszn_Info*)malloc(ReadNum * sizeof(Sszn_Info));
    if (cameraList->cam_info == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Iterate over the found devices and populate the camera list
    for (int i = 0; i < ReadNum; ++i) {
        // Populate camera info (assign a unique ID for each device)
        cameraList->cam_info[i].id = i + 1;
        
        // Convert the 4-byte IP address to a string format (e.g., "192.168.0.1")
        snprintf(cameraList->cam_info[i].ipAddress, sizeof(cameraList->cam_info[i].ipAddress),
                 "%d.%d.%d.%d", pDevices[i].abyIpAddress[0], pDevices[i].abyIpAddress[1],
                 pDevices[i].abyIpAddress[2], pDevices[i].abyIpAddress[3]);
    }

    // Update the camera count
    cameraList->count = ReadNum;

    // Print the number of devices found
    printf("Found %d device(s).\n", ReadNum);
}
