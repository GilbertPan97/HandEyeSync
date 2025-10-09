<#
.SYNOPSIS
    Build and install Boost using bootstrap and b2 on Windows.

.PARAMETER BoostDir
    Path to Boost source directory (where bootstrap.bat is located).

.PARAMETER InstallDir
    Path to Boost installation directory (used only in install step).

.PARAMETER JobCount
    Number of parallel jobs for b2 (used only in install step).

.PARAMETER Step
    Step to execute: "bootstrap" or "install".

.EXAMPLE
    powershell -File build_boost.ps1 -BoostDir "C:\boost\src" -Step bootstrap
    powershell -File build_boost.ps1 -BoostDir "C:\boost\src" -InstallDir "C:\boost\install" -JobCount 4 -Step install
#>

function Find-VcVarsAll {
    <#
    .SYNOPSIS
        Search for vcvarsall.bat in typical Visual Studio installation paths.
    .OUTPUTS
        Returns the full path to vcvarsall.bat if found, otherwise $null
    #>

    # Common VS editions
    $vsEditions = @("Community", "Professional", "Enterprise", "BuildTools")

    # Default VS installation root
    $vsRoot = "C:\Program Files\Microsoft Visual Studio\2022"

    foreach ($edition in $vsEditions) {
        $candidate = Join-Path -Path $vsRoot -ChildPath "$edition\VC\Auxiliary\Build\vcvarsall.bat"
        if (Test-Path $candidate) {
            return $candidate
        }
    }

    # Optionally, check environment variable if VSINSTALLDIR is set
    if ($env:VSINSTALLDIR) {
        $candidate = Join-Path -Path $env:VSINSTALLDIR -ChildPath "VC\Auxiliary\Build\vcvarsall.bat"
        if (Test-Path $candidate) {
            return $candidate
        }
    }

    return $null
}

# Example usage (just for testing, not actually called in your current script)
# $vcvarsPath = Find-VcVarsAll
# Write-Host "Found vcvarsall.bat at: $vcvarsPath"

[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)]
    [string]$BoostDir,

    [Parameter(Mandatory=$false)]
    [string]$InstallDir,

    [Parameter(Mandatory=$false)]
    [int]$JobCount = 4,

    [Parameter(Mandatory=$true)]
    [ValidateSet("bootstrap", "install")]
    [string]$Step
)

# Ensure the Boost directory exists
if (-Not (Test-Path $BoostDir)) {
    Write-Error "Boost directory '$BoostDir' does not exist!"
    exit 1
}

# Switch to the Boost directory
Set-Location $BoostDir

switch ($Step) {
    "bootstrap" {
        Write-Host "Running bootstrap.bat in $BoostDir..."
        $bootstrapExe = Join-Path $BoostDir "bootstrap.bat"
        if (-Not (Test-Path $bootstrapExe)) {
            Write-Error "bootstrap.bat not found in $BoostDir"
            exit 1
        }
        # Run bootstrap
        & $bootstrapExe
        if ($LASTEXITCODE -ne 0) {
            Write-Error "bootstrap.bat failed with exit code $LASTEXITCODE"
            exit $LASTEXITCODE
        }
        Write-Host "Bootstrap completed successfully."
    }

    "install" {
        if (-Not $InstallDir) {
            Write-Error "InstallDir must be provided for 'install' step"
            exit 1
        }

        # Ensure the install directory exists, or create it
        if (-Not (Test-Path $InstallDir)) {
            Write-Host "Creating install directory: $InstallDir"
            New-Item -ItemType Directory -Path $InstallDir
        }

        $JobOption = ""
        if ($JobCount) { 
            $JobOption = "-j$JobCount" 
        }

        Write-Host "Running b2 install in $BoostDir..."

        $b2Exe = Join-Path $BoostDir "b2.exe"
        if (-Not (Test-Path $b2Exe)) {
            Write-Error "b2.exe not found in $BoostDir. Did you run bootstrap?"
            exit 1
        }

        # Run b2 with job count, 64-bit address model, and installation prefix
        & $b2Exe install --prefix="$InstallDir" address-model=64 architecture=x86 threading=multi $JobOption
        if ($LASTEXITCODE -ne 0) {
            Write-Error "b2 install failed with exit code $LASTEXITCODE"
            exit $LASTEXITCODE
        }
        Write-Host "Boost installed successfully to $InstallDir."
    }
}