param(
  [string]$Name = "LeftAtriumModel",
  [string]$OutputDir = "dist",
  [switch]$IncludeNodesMuscles = $true,
  [switch]$IncludePreviousRuns = $true,
  [switch]$Zip = $true,
  [switch]$Build = $true,
  [switch]$KeepFolder = $false
)

$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
$timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
$packageName = "$Name-$timestamp"
$outRoot = Join-Path $repoRoot $OutputDir
$dest = Join-Path $outRoot $packageName

Write-Host "Packaging to: $dest"
New-Item -ItemType Directory -Force -Path $dest | Out-Null

# Locate executable
$exeCandidates = @(
  (Join-Path $repoRoot "build/svt.exe"),
  (Join-Path $repoRoot "build/svt")
)
$exe = $exeCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $exe) {
  if ($Build) {
    Write-Host "Executable not found. Building with CMake..."
    $buildDir = Join-Path $repoRoot 'build'
    $ninja = Get-Command ninja -ErrorAction SilentlyContinue
    $genArgs = @()
    if ($ninja) { $genArgs = @('-G','Ninja') }
    & cmake -S $repoRoot -B $buildDir @genArgs
    if ($LASTEXITCODE -ne 0) { throw "CMake configure failed ($LASTEXITCODE)." }
    & cmake --build $buildDir
    if ($LASTEXITCODE -ne 0) { throw "CMake build failed ($LASTEXITCODE)." }
    $exe = $exeCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if (-not $exe) { throw "Build completed but executable not found in $buildDir." }
  } else {
    Write-Error "Executable not found. Run build first or pass -Build to package.ps1."
  }
}

# Always output as svt.exe for Windows
Copy-Item $exe -Destination (Join-Path $dest "svt.exe") -Force

# Include simulation setup files
$setupFiles = @(
  "BasicSimulationSetup",
  "IntermediateSimulationSetup",
  "AdvancedSimulationSetup",
  "simulationSetup.txt"
)
foreach ($f in $setupFiles) {
  $src = Join-Path $repoRoot $f
  if (Test-Path $src) {
    Copy-Item $src -Destination (Join-Path $dest (Split-Path $src -Leaf)) -Force
    Write-Host "Added: $f"
  } else {
    Write-Warning "Missing setup file: $f"
  }
}

# Include NodesMuscles data
if ($IncludeNodesMuscles) {
  $nodesPath = Join-Path $repoRoot "NodesMuscles"
  if (Test-Path $nodesPath) {
    Copy-Item $nodesPath -Destination (Join-Path $dest "NodesMuscles") -Recurse -Force
    Write-Host "Added: NodesMuscles"
  } else {
    Write-Warning "NodesMuscles folder not found"
  }
}

# Include PreviousRunsFile demos
if ($IncludePreviousRuns) {
  $prevPath = Join-Path $repoRoot "PreviousRunsFile"
  if (Test-Path $prevPath) {
    Copy-Item $prevPath -Destination (Join-Path $dest "PreviousRunsFile") -Recurse -Force
    Write-Host "Added: PreviousRunsFile"
  } else {
    Write-Warning "PreviousRunsFile folder not found"
  }
}

# Include UI config if present
$iniPath = Join-Path $repoRoot "imgui.ini"
if (Test-Path $iniPath) {
  Copy-Item $iniPath -Destination (Join-Path $dest "imgui.ini") -Force
  Write-Host "Added: imgui.ini"
}

# Write a minimal run README
$runReadme = @"
Left Atrium Model - Portable Package

Requirements:
- Windows with NVIDIA CUDA-enabled GPU and current drivers
- CUDA Toolkit and MSVC runtime installed
- FFmpeg (optional) for Record Video/Screenshot:
  winget install --id Gyan.FFmpeg -e

Run:
- Double-click svt.exe or run from PowerShell:
  .\\svt.exe

Simulation setups:
- BasicSimulationSetup, IntermediateSimulationSetup, AdvancedSimulationSetup included.
- To use a setup, replace the contents of simulationSetup.txt with one of the setup files.

Data:
- NodesMuscles contains the available meshes.
- PreviousRunsFile contains demo run files and a place to save your runs.

"@
Set-Content -Path (Join-Path $dest "README-run.txt") -Value $runReadme -Encoding UTF8
Write-Host "Added: README-run.txt"

# Zip the package
if ($Zip) {
  New-Item -ItemType Directory -Force -Path $outRoot | Out-Null
  $zipPath = Join-Path $outRoot ("$packageName.zip")
  if (Test-Path $zipPath) { Remove-Item $zipPath -Force }
  Compress-Archive -Path $dest -DestinationPath $zipPath
  Write-Host "Zip created: $zipPath"
  if (-not $KeepFolder) {
    Remove-Item $dest -Recurse -Force
    Write-Host "Cleaned folder: $dest"
  }
}

Write-Host "Packaging complete."