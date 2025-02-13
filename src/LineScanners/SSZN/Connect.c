#include "SRCommon.h"

#include <stdio.h>
#include <stdlib.h>

// Connect to a sensor using the provided IP address
bool Sszn_Connect(Sszn_Handle* handle, const char* sensorIp, int deviceId) {
    if (handle == NULL || sensorIp == NULL) {
        return EXIT_FAILURE; 
    }

    // Step 1: Convert IP address to byte array and store it in the handle's Ethernet configuration
    unsigned char ipParts[4];
    if (sscanf(sensorIp, "%hhd.%hhd.%hhd.%hhd", 
        &ipParts[0], &ipParts[1], &ipParts[2], &ipParts[3]) != 4) {
        return EXIT_FAILURE;        // Invalid IP address format
    }

    // Copy the IP bytes into the Ethernet configuration
    for (int i = 0; i < 4; i++) {
        handle->SREthernetConFig.abyIpAddress[i] = ipParts[i];
    }

    // Step 2: Update the DEVICE_ID
    handle->DEVICE_ID = deviceId;

    // // Step 3: Initialize the DataObject 
    // memset(&handle->DataObject, 0, sizeof(SR7IF_Data));  // Clear previous data

    // Step 4: Connecting to the sensor
    SR7IF_EthernetOpen(handle->DEVICE_ID, &handle->SREthernetConFig);
    clog("Connecting to SSZN sensor at IP: %s\n", sensorIp);

    return EXIT_SUCCESS;
}
