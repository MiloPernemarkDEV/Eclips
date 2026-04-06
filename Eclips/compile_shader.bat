@echo off
:: ================================
:: Compile Vulkan shaders to SPIR-V
:: ================================

:: Check if VULKAN_SDK is set
if "%VULKAN_SDK%"=="" (
    echo ERROR: VULKAN_SDK environment variable not set!
    pause
    exit /b
)

:: Path to glslc.exe
set GLSLC=%VULKAN_SDK%\Bin\glslc.exe

:: Check if glslc.exe exists
if not exist "%GLSLC%" (
    echo ERROR: glslc.exe not found at %GLSLC%
    pause
    exit /b
)

:: Check if shader files exist
if not exist "Shaders\shader.vert" (
    echo ERROR: shader.vert not found in Shaders folder
    pause
    exit /b
)

if not exist "Shaders\shader.frag" (
    echo ERROR: shader.frag not found in Shaders folder
    pause
    exit /b
)

:: Compile vertex shader
"%GLSLC%" Shaders\shader.vert -o Shaders\vert.spv

:: Compile fragment shader
"%GLSLC%" Shaders\shader.frag -o Shaders\frag.spv

echo Shaders compiled successfully!
pause