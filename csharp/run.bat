@echo off
rem Launches the C# course on Windows. Same job as ./run on Linux -- see
rem that file's comment for why a wrapper is needed instead of pointing the
rem launcher straight at the .csproj. Untested on Windows, same as the rest
rem of this repo's Windows path.
cd /d "%~dp0src"
dotnet run
