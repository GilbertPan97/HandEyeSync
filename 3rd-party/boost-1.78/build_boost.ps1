# build_boost.ps1
& "$PSScriptRoot/../.cache/boost-1.78-src/bootstrap.bat"
& "$PSScriptRoot/../.cache//b2.exe" -j4 --build-type=complete --with=all --prefix="$PSScriptRoot" install
