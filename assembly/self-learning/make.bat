@echo off
set INCLUDE=C:\masm32\include
set LIB=C:\masm32\lib
echo ################# make file ################
if "%1"=="" (
    pause
    exit /b 1
)
echo [+] assembling the source code.
ml /c /coff %1.asm

if errorlevel 1 (
    echo Assembly failed! Check your code.
    pause
    exit /b 1
    
)

link /subsystem:console /LIBPATH:"C:\masm32\lib" %1.obj
echo [+] linking the object files.
if errorlevel 1 (
    echo Linking failed!
    pause
    exit /b 1
)

echo Build Complete! Executable: %1.exe
