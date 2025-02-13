#include "SRCommon.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    // Initialize the camera list
    Sszn_List cameraList;
    Sszn_Handle handle = {0};

    // Call the discover function to populate the camera list
    bool status = Sszn_Discover(&cameraList);
    if (!status) {
        printf("Camera discovery failed.\n");
        return EXIT_FAILURE; // Exit if discovery fails
    }

    // Print the discovered camera information
    printf("Discovered %zu cameras:\n", cameraList.count);
    for (int i = 0; i < cameraList.count; ++i) {
        printf("Camera ID: %d, IP Address: %s\n", cameraList.cam_info[i].id, cameraList.cam_info[i].ipAddress);
        
        // Attempt to connect to the camera using its IP address
        printf("Attempting to connect to Camera %d at IP %s...\n", cameraList.cam_info[i].id, cameraList.cam_info[i].ipAddress);
        
        // Call the connect function
        status = Sszn_Connect(&handle, cameraList.cam_info[i].ipAddress, i + 1);
        
        // Check connection status
        if (status) {
            printf("Successfully connected to Camera %d at IP %s\n", cameraList.cam_info[i].id, cameraList.cam_info[i].ipAddress);
        } else {
            printf("Failed to connect to Camera %d at IP %s\n", cameraList.cam_info[i].id, cameraList.cam_info[i].ipAddress);
        }
    }

    // Free the allocated memory for camera list
    free(cameraList.cam_info);

    return EXIT_SUCCESS; // Indicate successful execution
}
