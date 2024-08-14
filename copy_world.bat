@echo off

if not exist ".\system" (
    echo .\system not exist, use xmake to build
    exit /b 1
)

setlocal enabledelayedexpansion
set "world_files="

for %%f in (.\system\*.wld) do (
    set "world_files=%%f"
    goto :found
)

if "%world_files%"=="" (
    echo .\system\*.wld not exist, use xmake to build
    exit /b 1
)

:found
set "terraria_folder=%USERPROFILE%\Documents\My Games\Terraria\Worlds\"
set "tmodloader_folder=%USERPROFILE%\Documents\My Games\Terraria\tModLoader\Worlds\"

for %%f in (.\system\*.wld) do (
    if exist "!terraria_folder!" (
        echo Copy world to !terraria_folder!%%~nxf
        copy "%%f" "!terraria_folder!%%~nxf"
    )
    if exist "!tmodloader_folder!" (
        echo Copy world to !tmodloader_folder!%%~nxf
        copy "%%f" "!tmodloader_folder!%%~nxf"
    )
)

echo Copy world complete