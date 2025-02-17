#include "SRCommon.h"

#include <stdio.h>
#include <stdlib.h>

void RemoveInvalidPoints(ProfileData* data) {

}

// Receives profile data from the Sszn sensor.
int Sszn_ReceiveProfileData(Sszn_Handle* handle, ProfileData* data) {
    // Initial buffer to store one profile data
    ProfilePoint* profileBuffer = NULL;
    int* pProfileData = NULL;
    unsigned char* grayData = NULL;

    int profilePointCount = 0;      // Single profile points count
    double xPixth = 0.0;            // Gap (mm) of profile point in X-direction

    // TODO: Prevent execution if the sensor is not in 2.5D mode
    if (handle == NULL || data == NULL) {
        return EXIT_FAILURE; // Invalid parameters
    }

    // Start measurement (capturing data)
    int result = SR7IF_StartMeasure(handle->DEVICE_ID, 20000);
    if (result != 0) {
        return EXIT_FAILURE; // Measurement start failed
    }

    // Bind data reception
    result = SR7IF_ReceiveData(handle->DEVICE_ID, handle->DataObject);
    if (result != 0) {
        SR7IF_StopMeasure(handle->DEVICE_ID); // Stop measurement in case of failure
        return EXIT_FAILURE; // Data binding failed
    }

    // Get the profile point count (width of the profile)
    profilePointCount = SR7IF_ProfileDataWidth(handle->DEVICE_ID, handle->DataObject);
    xPixth = SR7IF_ProfileData_XPitch(handle->DEVICE_ID, handle->DataObject);

    // Allocate memory for profile data
    pProfileData = (int*)malloc(profilePointCount * sizeof(int)); // Profile buffer
    if (pProfileData == NULL) {
        SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
        return EXIT_FAILURE;    // Memory allocation failure
    }

    // Allocate memory for gray intensity data
    grayData = (unsigned char*)malloc(profilePointCount * sizeof(unsigned char));
    if (grayData == NULL) {
        free(pProfileData);     // Free previously allocated memory
        SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
        return EXIT_FAILURE;    // Memory allocation failure
    }

    // Get intensity data (gray scale values)
    result = SR7IF_GetIntensityData(handle->DEVICE_ID, handle->DataObject, grayData);
    if (result != 0) {
        free(pProfileData);     // Free memory
        free(grayData);         // Free memory
        SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
        return EXIT_FAILURE;    // Data retrieval failed
    }

    // Get a single profile
    unsigned int pEncoder = 0;
    result = SR7IF_GetSingleProfile(handle->DEVICE_ID, pProfileData, &pEncoder);
    if (result != 0) {
        free(pProfileData);     // Free memory
        free(grayData);         // Free memory
        SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
        return EXIT_FAILURE;    // Data retrieval failed
    }

    // Convert received data to XZ coordinates
    profileBuffer = malloc(profilePointCount * sizeof(ProfilePoint));
    if (profileBuffer == NULL) {
        free(pProfileData);     // Free memory
        free(grayData);         // Free memory
        SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
        return EXIT_FAILURE;    // Memory allocation failure
    }

    // Initialize data buffer and convert to ProfileXZ
    data->profileBuffer = profileBuffer;
    data->totalCount = profilePointCount;
    data->validPoints = profilePointCount;

    for (size_t i = 0; i < profilePointCount; i++) {
        profileBuffer[i].x = i * xPixth;                    // X coordinate
        profileBuffer[i].z = pProfileData[i] / 100000.0;    // Z coordinate (converted from profile data)
        profileBuffer[i].intensity = grayData[i];           // Intensity data for the profile point
    }

    // Free the temporary profile data as it's no longer needed
    free(pProfileData);
    free(grayData);

    // Stop measurement (finish capturing data)
    result = SR7IF_StopMeasure(handle->DEVICE_ID);
    if (result != 0) {
        return EXIT_FAILURE;    // Failed to stop the measurement
    }

    // FIXME: How to remove all invalid points and update data->validPoints.

    return EXIT_SUCCESS;        // Success
}
