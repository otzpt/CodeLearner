# CodeLearner installer -- Windows, x86-64.
#
#   irm https://raw.githubusercontent.com/otzpt/CodeLearner/main/install.ps1 | iex
#
# Installs the latest release under %LOCALAPPDATA% and puts a `codelearner`
# command on your PATH. No administrator rights, because nothing is written
# outside your own profile.
#
# To remove it again, or to install somewhere else, download the script and
# run it directly rather than piping it:
#
#   .\install.ps1 -Uninstall
#   .\install.ps1 -Prefix D:\Tools\CodeLearner

[CmdletBinding()]
param(
    [switch]$Uninstall,
    [string]$Prefix = (Join-Path $env:LOCALAPPDATA 'Programs\CodeLearner')
)

$ErrorActionPreference = 'Stop'

$repo       = 'otzpt/CodeLearner'
$asset      = 'codelearner-windows-x86_64.zip'
$bundleDir  = 'codelearner-windows-x86_64'

function Remove-FromUserPath {
    param([string]$Directory)
    # Read the User scope specifically, never $env:Path -- that one is the
    # machine and user values already joined, and writing it back to User
    # scope would copy every machine entry into this account permanently.
    $userPath = [Environment]::GetEnvironmentVariable('Path', 'User')
    if (-not $userPath) { return }
    $kept = $userPath.Split(';') |
        Where-Object { $_ -and ($_.TrimEnd('\') -ne $Directory.TrimEnd('\')) }
    [Environment]::SetEnvironmentVariable('Path', ($kept -join ';'), 'User')
}

if ($Uninstall) {
    if (Test-Path $Prefix) {
        Remove-Item -Recurse -Force $Prefix
        Write-Host "Removed $Prefix"
    } else {
        Write-Host "Nothing installed at $Prefix"
    }
    Remove-FromUserPath -Directory $Prefix
    Write-Host 'Removed it from your PATH. Open a new terminal for that to take effect.'
    return
}

if ([Environment]::Is64BitOperatingSystem -eq $false) {
    throw 'The prebuilt release is x86-64 only. Build from source instead: https://github.com/otzpt/CodeLearner#running'
}

# Windows PowerShell 5.1 still negotiates TLS 1.0 by default on some
# installs, which GitHub refuses outright.
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

# /releases/latest/download/ is a redirect GitHub maintains to the newest
# release, so there is no API call and no JSON to parse.
$url = "https://github.com/$repo/releases/latest/download/$asset"
$tmp = Join-Path ([IO.Path]::GetTempPath()) ("codelearner-" + [Guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $tmp | Out-Null

try {
    Write-Host "Downloading $asset..."
    $zip = Join-Path $tmp $asset
    # The progress bar makes Invoke-WebRequest an order of magnitude slower
    # on a large file in Windows PowerShell.
    $oldProgress = $ProgressPreference
    $ProgressPreference = 'SilentlyContinue'
    Invoke-WebRequest -Uri $url -OutFile $zip -UseBasicParsing
    $ProgressPreference = $oldProgress

    Write-Host 'Extracting...'
    Expand-Archive -Path $zip -DestinationPath $tmp -Force
    $extracted = Join-Path $tmp $bundleDir
    if (-not (Test-Path $extracted)) {
        throw "The archive did not contain $bundleDir as expected."
    }

    # Replace any previous install rather than merging into it: a release
    # that drops a course would otherwise leave the old one behind, still
    # listed by the launcher and no longer supported by anything around it.
    if (Test-Path $Prefix) { Remove-Item -Recurse -Force $Prefix }
    $parent = Split-Path -Parent $Prefix
    if (-not (Test-Path $parent)) { New-Item -ItemType Directory -Path $parent | Out-Null }
    Move-Item -Path $extracted -Destination $Prefix
} finally {
    Remove-Item -Recurse -Force $tmp -ErrorAction SilentlyContinue
}

# codelearner.bat already ships at the root of the bundle and already does
# the right thing (cd into launcher\, then run launcher.exe -- the launcher
# finds each course by a path relative to its own directory). Putting that
# directory on PATH is the whole of what is left to do.
$userPath = [Environment]::GetEnvironmentVariable('Path', 'User')
$onPath = $userPath -and ($userPath.Split(';') |
    Where-Object { $_ -and ($_.TrimEnd('\') -eq $Prefix.TrimEnd('\')) })

if (-not $onPath) {
    $newPath = if ($userPath) { "$userPath;$Prefix" } else { $Prefix }
    [Environment]::SetEnvironmentVariable('Path', $newPath, 'User')
    $pathNote = 'Added it to your PATH. Open a new terminal, then run:  codelearner'
} else {
    $pathNote = 'Already on your PATH. Run:  codelearner'
}

Write-Host ''
Write-Host "Installed to $Prefix"
Write-Host $pathNote
