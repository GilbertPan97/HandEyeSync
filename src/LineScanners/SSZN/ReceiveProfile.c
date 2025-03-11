#include "SRCommon.h"

#include <stdio.h>
#include <stdlib.h>

void RemoveInvalidPoints(ProfileData* data) {
    // Check for null pointers or zero valid points
    if (!data || !data->profileBuffer || data->validPoints == 0) {
        return; // Invalid data, return immediately
    }

    size_t validCount = 0;
    for (size_t i = 0; i < data->validPoints; ++i) {
        // Keep points where z is not -10000
        if (data->profileBuffer[i].z != -10000) {
            // Move valid points to the front of the buffer
            data->profileBuffer[validCount++] = data->profileBuffer[i];
        }
    }

    // Update the count of valid points
    data->validPoints = validCount;
}


// Receives profile data from the Sszn sensor.
bool Sszn_ReceiveProfileData(Sszn_Handle* handle, ProfileData* data) {
    // Initial buffer to store one profile data
    ProfilePoint* profileBuffer = NULL;
    int* pProfileData = NULL;
    unsigned char* grayData = NULL;
    int result;

    int profilePointCount = 0;      // Single profile points count
    double xPixth = 0.0;            // Gap (mm) of profile point in X-direction
    if (handle == NULL || data == NULL) {
        return EXIT_FAILURE; // Invalid parameters
    }

    // TODO: Prevent execution if the sensor is not in 2.5D mode
    // result =  SR7IF_GetSetting(handle->DEVICE_ID, int Type, 
    //     int Category, int Item, int Target[4], void *pData, int DataSize);

    // // Stop batch measure, SR7IF_GetSingleProfile
    // result = SR7IF_StopMeasure(handle->DEVICE_ID);

    // Get the profile point count (width of the profile)
    profilePointCount = SR7IF_ProfileDataWidth(handle->DEVICE_ID, handle->DataObject);
    xPixth = SR7IF_ProfileData_XPitch(handle->DEVICE_ID, handle->DataObject);

    // Allocate memory for profile data
    pProfileData = (int*)malloc(profilePointCount * sizeof(int)); // Profile buffer
    if (pProfileData == NULL) {
        SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
        return EXIT_FAILURE;    // Memory allocation failure
    }

    // Get a single profile
    unsigned int pEncoder = 0;
    result = SR7IF_GetSingleProfile(handle->DEVICE_ID, pProfileData, &pEncoder);
    if (result != 0) {
        free(pProfileData);     // Free memory
        SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
        return EXIT_FAILURE;    // Data retrieval failed
    }

    // Allocate memory for gray intensity data
    grayData = (unsigned char*)malloc(profilePointCount * sizeof(unsigned char));
    if (grayData == NULL) {
        free(pProfileData);     // Free previously allocated memory
        SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
        return EXIT_FAILURE;    // Memory allocation failure
    }

    // // Get intensity data (gray scale values)
    // result = SR7IF_GetIntensityData(handle->DEVICE_ID, handle->DataObject, grayData);
    // if (result != 0) {
    //     free(pProfileData);     // Free memory
    //     free(grayData);         // Free memory
    //     SR7IF_StopMeasure(handle->DEVICE_ID);   // Stop measurement in case of failure
    //     return EXIT_FAILURE;    // Data retrieval failed
    // }

    // Initialize data buffer and convert to ProfileXZ
    profileBuffer = (ProfilePoint*)malloc(profilePointCount * sizeof(ProfilePoint));
    data->profileBuffer = profileBuffer;
    data->totalCount = profilePointCount;
    data->validPoints = profilePointCount;

    for (size_t i = 0; i < profilePointCount; i++) {
        profileBuffer[i].x = i * xPixth;                    // X coordinate
        profileBuffer[i].z = pProfileData[i] / 100000.0;    // Z coordinate (converted from profile data)
        // profileBuffer[i].intensity = grayData[i];           // Intensity data for the profile point
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
    RemoveInvalidPoints(data);

    return EXIT_SUCCESS;        // Success
}
