if (Test-Path "build\svt.exe") {
    & ".\build\svt.exe"
} else {
    Write-Error "svt.exe not found. Run compile.ps1 first."
}
