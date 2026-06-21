param(
    [string]$BuildDir = "build",
    [string]$OutputDir = "release"
)

$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$buildPath = Join-Path $root $BuildDir
$outputPath = Join-Path $root $OutputDir
$binaryStage = Join-Path $outputPath "Dormora-binary"
$sourceStage = Join-Path $outputPath "Dormora-source"

if (-not (Test-Path (Join-Path $buildPath "udrms_gui.exe"))) {
    throw "Build the project first. Missing $BuildDir\udrms_gui.exe."
}

if (Test-Path $outputPath) {
    Remove-Item -LiteralPath $outputPath -Recurse -Force
}
New-Item -ItemType Directory -Path $binaryStage, $sourceStage | Out-Null

$binaryItems = @(
    "udrms_gui.exe",
    "Qt6Core.dll",
    "Qt6Gui.dll",
    "Qt6Svg.dll",
    "Qt6Widgets.dll",
    "D3Dcompiler_47.dll",
    "libgcc_s_seh-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "opengl32sw.dll"
)

foreach ($item in $binaryItems) {
    $source = Join-Path $buildPath $item
    if (Test-Path $source) {
        Copy-Item -LiteralPath $source -Destination $binaryStage
    }
}

foreach ($pluginDir in @("generic", "iconengines", "imageformats", "platforms", "styles", "translations")) {
    $source = Join-Path $buildPath $pluginDir
    if (Test-Path $source) {
        Copy-Item -LiteralPath $source -Destination $binaryStage -Recurse
    }
}

@"
@echo off
cd /d "%~dp0"
start "" "%~dp0udrms_gui.exe"
"@ | Set-Content -LiteralPath (Join-Path $binaryStage "Launch Dormora.bat") -Encoding ASCII
Copy-Item -LiteralPath (Join-Path $root "README.md") -Destination $binaryStage
Copy-Item -LiteralPath (Join-Path $root "extras\docs\SUBMISSION.md") -Destination $binaryStage

$sourceExcludes = @(
    ".git",
    "build",
    "release",
    ".vs",
    ".vscode",
    "*.user",
    "*.autosave",
    "udrms-data.json"
)

Get-ChildItem -LiteralPath $root -Force | Where-Object {
    $name = $_.Name
    -not ($sourceExcludes | Where-Object { $name -like $_ })
} | ForEach-Object {
    Copy-Item -LiteralPath $_.FullName -Destination $sourceStage -Recurse -Force
}

$umlDraftsPath = Join-Path $sourceStage "extras\docs"
if (Test-Path $umlDraftsPath) {
    Get-ChildItem -LiteralPath $umlDraftsPath -Filter "uml-*" | Remove-Item -Force
}

Compress-Archive -Path (Join-Path $binaryStage "*") -DestinationPath (Join-Path $outputPath "Dormora-binary.zip") -Force
Compress-Archive -Path (Join-Path $sourceStage "*") -DestinationPath (Join-Path $outputPath "Dormora-source.zip") -Force

Write-Host "Created:"
Write-Host "  $outputPath\Dormora-binary.zip"
Write-Host "  $outputPath\Dormora-source.zip"
