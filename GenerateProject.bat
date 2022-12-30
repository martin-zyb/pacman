@echo off

if exist ".//build//CMakeFiles" (
    set /p var="The project has been generated, do you want to regenerate it (y/n)":
)^
else (
    set var="p"
)

if %var%==y (
    rd /s /q build
)^
else if %var%==n (
    goto end
)

if not exist ".//build" md build

cd build

cmake ..

@cd ..

echo.

:end
pause