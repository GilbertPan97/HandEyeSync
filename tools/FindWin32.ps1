Get-ChildItem -Path "./HandEyeSync" -Recurse -Include *.exe, *.dll | ForEach-Object {
    $path = $_.FullName
    $bytes = [System.IO.File]::ReadAllBytes($path)

    # 检查PE头：IMAGE_FILE_MACHINE_I386 = 0x014c（32位）
    if ($bytes[0x3C] -ne $null) {
        $peOffset = [BitConverter]::ToInt32($bytes, 0x3C)
        $machine = [BitConverter]::ToUInt16($bytes, $peOffset + 4)
        if ($machine -eq 0x014c) {
            Write-Output "Win32: $path"
        }
    }
}
