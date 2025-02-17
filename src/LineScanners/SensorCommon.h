#ifndef PROFILE_DATA_H
#define PROFILE_DATA_H

#include <stddef.h>  // for size_t
#include <string.h>  // for memcpy, memset
#include <stdio.h>   // for printf

// Structure to hold camera information
typedef struct {
    int id;                     // Camera ID
    char ipAddress[64];         // Camera IP address
} Sensor_Info;

// Structure to hold a list of discovered cameras
typedef struct {
    Sensor_Info* cam_info;        // Pointer to an array of CameraInfo
    size_t count;               // Number of discovered cameras
} Sensor_List;

// ProfilePoint represents a single profile point with x, z coordinates and intensity
typedef struct {
    double x;            // x-coordinate in engineering units (mm)
    double z;            // z-coordinate in engineering units (mm)
    unsigned char intensity; // Intensity value (could be 0-255 or any range depending on sensor)
} ProfilePoint;

// ProfileData stores an array of ProfilePoint and related metadata
typedef struct {
    ProfilePoint* profileBuffer;    // Buffer holding the valid profile points
    size_t validPoints;             // Number of valid points  == profileBuffer.size()
    size_t totalCount;              // Total size for a profile (may be larger than validPoints)
} ProfileData;

// // Function to initialize ProfileData
// int initializeProfileData(ProfileData* profileData, size_t bufferSize) {
//     if (profileData == NULL) {
//         return -1;  // Error: NULL pointer
//     }

//     profileData->profileBuffer = (ProfilePoint*)malloc(bufferSize * sizeof(ProfilePoint));  // Allocate memory
//     if (profileData->profileBuffer == NULL) {
//         return -1;  // Error: Memory allocation failed
//     }

//     profileData->pointCount = 0;        // Initially, no points are set
//     profileData->bufferSize = bufferSize; // Set buffer size
//     return 0;  // Success
// }

// // Function to clean up the ProfileData, especially freeing any allocated memory
// void cleanupProfileData(ProfileData* profileData) {
//     if (profileData != NULL && profileData->profileBuffer != NULL) {
//         free(profileData->profileBuffer);  // Free the allocated memory
//         profileData->profileBuffer = NULL; // Set pointer to NULL after cleanup
//     }
//     profileData->pointCount = 0;          // Reset point count to zero
//     profileData->bufferSize = 0;          // Reset buffer size
// }

// // Function to reset ProfileData (clears pointCount, bufferSize remains)
// void resetProfileData(ProfileData* profileData) {
//     if (profileData != NULL) {
//         profileData->pointCount = 0;  // Reset the point count
//     }
// }

// // Function to set profile data buffer from raw data
// int setProfileData(ProfileData* profileData, const ProfilePoint* data, size_t pointCount) {
//     if (profileData == NULL || data == NULL) {
//         return -1;  // Error: NULL pointer
//     }

//     if (pointCount > profileData->bufferSize) {
//         return -1;  // Error: Point count exceeds buffer size
//     }

//     // Copy data into the profileBuffer
//     memcpy(profileData->profileBuffer, data, pointCount * sizeof(ProfilePoint));
//     profileData->pointCount = pointCount;
//     return 0;  // Success
// }

// // Function to get a specific ProfilePoint
// int getProfilePoint(const ProfileData* profileData, size_t index, ProfilePoint* point) {
//     if (profileData == NULL || point == NULL) {
//         return -1;  // Error: NULL pointer
//     }

//     if (index >= profileData->pointCount) {
//         return -1;  // Error: Index out of range
//     }

//     *point = profileData->profileBuffer[index]; // Get the point at the given index
//     return 0;  // Success
// }

// // Function to set a specific ProfilePoint
// int setProfilePoint(ProfileData* profileData, size_t index, const ProfilePoint* point) {
//     if (profileData == NULL || point == NULL) {
//         return -1;  // Error: NULL pointer
//     }

//     if (index >= profileData->bufferSize) {
//         return -1;  // Error: Index out of range
//     }

//     profileData->profileBuffer[index] = *point;  // Set the point at the given index
//     return 0;  // Success
// }

#endif // PROFILE_DATA_H
