# OD Pedal Build Script
# Builds the VST3 plugin locally

param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release"
)

Write-Host "=== OD Pedal Build ===" -ForegroundColor Cyan
Write-Host "Configuration: $Configuration" -ForegroundColor Yellow

# Configure
Write-Host "`nConfiguring..." -ForegroundColor Cyan
cmake -S . -B build -G "Ninja Multi-Config"
if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Configuration failed" -ForegroundColor Red
    exit 1
}

# Build
Write-Host "`nBuilding..." -ForegroundColor Cyan
cmake --build build --config $Configuration --parallel
if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Build failed" -ForegroundColor Red
    exit 1
}

# Verify VST3
$vst3Path = "build\ODPedal_artefacts\$Configuration\VST3\ODPedal.vst3"
if (Test-Path $vst3Path) {
    Write-Host "`n[SUCCESS] Build successful!" -ForegroundColor Green
    Write-Host "VST3 plugin: $vst3Path" -ForegroundColor Green
} else {
    Write-Host "`n[WARNING] Build completed but VST3 not found at expected path" -ForegroundColor Yellow
    Write-Host "Searching for .vst3 files..." -ForegroundColor Yellow
    Get-ChildItem -Path build -Filter "*.vst3" -Recurse | ForEach-Object {
        Write-Host "Found: $($_.FullName)" -ForegroundColor Green
    }
}
