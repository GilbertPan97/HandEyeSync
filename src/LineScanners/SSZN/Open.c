#include "SRCommon.h"

#include <stdio.h>
#include <stdlib.h>

// Open to a sensor using the provided IP address
bool Sszn_Open(Sszn_Handle* handle, const char* sensorIp, int deviceId) {
    if (handle == NULL || sensorIp == NULL) {
        return EXIT_FAILURE; 
    }

    // Step 1: Convert IP address to byte array and store it in the handle's Ethernet configuration
    unsigned char ipParts[4];
    if (sscanf_s(sensorIp, "%hhd.%hhd.%hhd.%hhd", 
        &ipParts[0], &ipParts[1], &ipParts[2], &ipParts[3]) != 4) {
        return EXIT_FAILURE;        // Invalid IP address format
    }

    // Copy the IP bytes into the Ethernet configuration
    for (int i = 0; i < 4; i++) {
        handle->SREthernetConFig.abyIpAddress[i] = ipParts[i];
    }

    // Step 2: Update the DEVICE_ID
    handle->DEVICE_ID = deviceId;

    // Step 3: Connecting to the sensor
    int status = SR7IF_EthernetOpen(handle->DEVICE_ID, &handle->SREthernetConFig);
    clog("Connecting to SSZN sensor at IP: %s\n", sensorIp);

    return EXIT_SUCCESS;
}

// Close the sensor using the provided device ID
bool Sszn_Close(Sszn_Handle* handle) {
    if (handle == NULL) {
        return EXIT_FAILURE;  // Invalid handle
    }

    // Step 1: Close communication with the sensor
    int status = SR7IF_CommClose(handle->DEVICE_ID);
    if (status != 0) {
        return EXIT_FAILURE;  // Failed to close the communication
    }

    // Step 2: Clear the device ID and Ethernet configuration for safety
    handle->DEVICE_ID = -1;
    memset(&handle->SREthernetConFig, 0, sizeof(SR7IF_ETHERNET_CONFIG));

    clog("Disconnected from SSZN sensor with device ID: %d\n", handle->DEVICE_ID);

    return EXIT_SUCCESS;
}