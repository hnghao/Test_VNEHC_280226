@echo off
setlocal enabledelayedexpansion
title Script Nap ESP32 - Auto Loop

:CHOOSE_COM
cls
echo ======================================================
echo       SCRIPT NAP ESP32 (CH343P - AUTO LOOP)
echo ======================================================
echo.
set /p com_number=">> Nhap so cong COM (chi nhap so, VD: 5): "

if "%com_number%"=="" (
    echo [!] Ban chua nhap so COM. Vui long thu lai.
    pause
    goto CHOOSE_COM
)

set "COM_PORT=COM%com_number%"

echo.
echo [*] Dang tien hanh nap cho cong %COM_PORT%...
echo ------------------------------------------------------

:: --- THAY THE DOAN LENH DUOI DAY BANG LENH CUA BAN ---
:: Luu y: Thay phan "COMx" trong lenh goc bang "%COM_PORT%"

rem "C:\Users\Admin\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\4.5.1/esptool.exe" --chip esp32 --port %COM_PORT% --baud 921600 write_flash -z 0x1000 main.bin
rem "C:\Users\XNgoc\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.0.0/esptool.exe" --chip esp32s3 --port %COM_PORT% --baud 921600  --before default-reset --after hard-reset write-flash  -z --flash-mode keep --flash-freq keep --flash-size keep 0x0 "C:\Users\XNgoc\AppData\Local\arduino\sketches\8553479F515AFB0134E653677F47AC0C/y25m11d20_QC_Firmware_Vietduino_11.ino.bootloader.bin" 0x8000 "C:\Users\XNgoc\AppData\Local\arduino\sketches\8553479F515AFB0134E653677F47AC0C/y25m11d20_QC_Firmware_Vietduino_11.ino.partitions.bin" 0xe000 "C:\Users\XNgoc\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.0/tools/partitions/boot_app0.bin" 0x10000 "C:\Users\XNgoc\AppData\Local\arduino\sketches\8553479F515AFB0134E653677F47AC0C/y25m11d20_QC_Firmware_Vietduino_11.ino.bin"
"C:\Users\XNgoc\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.0.0/esptool.exe" --chip esp32s3 --port %COM_PORT% --baud 921600  --before default-reset --after hard-reset write-flash  -z --flash-mode keep --flash-freq keep --flash-size keep 0x0 "C:\Users\XNgoc\AppData\Local\arduino\sketches\8553479F515AFB0134E653677F47AC0C/y25m11d20_QC_Firmware_Vietduino_11.ino.bootloader.bin" 0x8000 "C:\Users\XNgoc\AppData\Local\arduino\sketches\8553479F515AFB0134E653677F47AC0C/y25m11d20_QC_Firmware_Vietduino_11.ino.partitions.bin" 0xe000 "C:\Users\XNgoc\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.0/tools/partitions/boot_app0.bin" 0x10000 "C:\Users\XNgoc\AppData\Local\arduino\sketches\8553479F515AFB0134E653677F47AC0C/y25m11d20_QC_Firmware_Vietduino_11.ino.bin"

:: ------------------------------------------------------

if %errorlevel% equ 0 (
    echo.
    echo [OK] Nap thanh cong!
) else (
    echo.
    echo [ERROR] Co loi xay ra khi nap. Kiem tra lai ket noi.
)

echo.
echo ------------------------------------------------------
echo Nhan phim bat ky de tiep tuc nap board khac...
pause > nul
goto CHOOSE_COM