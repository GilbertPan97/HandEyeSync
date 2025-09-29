# envs.ps1
# This script configures and builds all third-party libraries with CMake
# using existing CMakeLists.txt that already includes subdirectories.

# Get the script's root directory
$RootDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ThirdPartyDir = Join-Path $RootDir "3rd-party"

# Set default build type
$BuildType = "RelWithDebInfo"

# Function to configure and build the 3rd-party project
function Build-ThirdParty() {
    Write-Host "=============================="
    Write-Host "Building 3rd-party libraries"
    Write-Host "=============================="

    $BuildDir = Join-Path $ThirdPartyDir "build"

    # Create build directory if it does not exist
    if (-Not (Test-Path $BuildDir)) {
        New-Item -ItemType Directory -Path $BuildDir | Out-Null
    }

    Push-Location $BuildDir

    # Run CMake configure
    Write-Host "Configuring CMake..."
    cmake $ThirdPartyDir -DCMAKE_BUILD_TYPE=$BuildType

    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed"
        Pop-Location
        return
    }

    # Build all targets (UTILITY targets are included in ALL_BUILD)
    Write-Host "Building all targets..."
    cmake --build . --config $BuildType --target ALL_BUILD

    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed"
    }

    Pop-Location
}

# Execute the build
Build-ThirdParty

Write-Host "=============================="
Write-Host "3rd-party libraries build completed"
Write-Host "=============================="
