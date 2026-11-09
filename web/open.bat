@echo off
rem Opens one web course in the default browser. Same job as ./open.
set "page=%~dp0%~1\index.html"
if not exist "%page%" (
    echo No such course: %~1
    exit /b 1
)
echo.
echo   This course runs in your browser. Opening:
echo   %page%
echo.
start "" "%page%"
echo   If no window appeared, open that file yourself.
echo.
pause
