#include "FrRobotInterface.h"
#include <iostream>

// Define the prototype of DllRegisterServer function
typedef HRESULT(WINAPI* DllRegisterServer_t)();

// Function to register COM component by calling DllRegisterServer in the DLL
void RegisterCOMComponent(const char* dllPath) {
    // Load the DLL
    HMODULE hModule = LoadLibraryA(dllPath);
    if (hModule == NULL) {
        std::cerr << "Failed to load DLL: " << dllPath << std::endl;
        return;
    }

    // Get the address of DllRegisterServer function
    DllRegisterServer_t pDllRegisterServer = (DllRegisterServer_t)GetProcAddress(hModule, "DllRegisterServer");
    if (pDllRegisterServer == NULL) {
        std::cerr << "Failed to find DllRegisterServer in the DLL" << std::endl;
        FreeLibrary(hModule);
        return;
    }

    // Call DllRegisterServer function to register the COM component
    HRESULT hr = pDllRegisterServer();
    if (FAILED(hr)) {
        std::cerr << "Failed to register COM component, error code: " << hr << std::endl;
    } else {
        std::cout << "COM component registered successfully!" << std::endl;
    }

    // Unload the DLL after use
    FreeLibrary(hModule);
}

int main() {

    // // Register COM components by calling the RegisterCOMComponent function
    // RegisterCOMComponent("D:\\Program Files\\FANUC\\FRRJIf\\FRRJIf.dll");
    // RegisterCOMComponent("D:\\Program Files\\FANUC\\FRRJIf\\FRRobotIFLib.dll");

    // Create an instance of FrRobotInterface
    FrRobotInterface robotInterface;

    // Provide a valid IP address for connection
    CString validIP = _T("192.168.1.100");

    // Try to connect to the robot
    if (robotInterface.Connect(validIP)) {
        std::cout << "Successfully connected to robot at IP: " << (const char*)validIP << std::endl;
    } else {
        std::cerr << "Failed to connect to robot." << std::endl;
    }

    // Disconnection (if needed)
    if (robotInterface.DisConnect()) {
        std::cout << "Disconnected successfully." << std::endl;
    } else {
        std::cerr << "Failed to disconnect." << std::endl;
    }

    return 0;
}
