@echo off

if not exist ".//build//CMakeFiles" call .//GenerateProject.bat

cd ".//build//CMakeFiles"

set /p type="Select Build Mode (Debug/Release)":

cmake --build .. --config %type%

cd ..//..

echo.

pause