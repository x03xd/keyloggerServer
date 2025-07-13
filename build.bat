@echo off
setlocal

set CC=gcc
set CFLAGS=-I modules -I src
set LDFLAGS=-lws2_32
set OUT=server.exe

:: Compile server.exe
echo [*] Compiling %OUT%...
%CC% .\src\main.c .\modules\core\server-core.c .\modules\servers\logging-server.c .\modules\servers\payload-server.c %CFLAGS% -o %OUT% %LDFLAGS%

if errorlevel 1 (
    echo [!] Error while compiling %OUT%
    goto end
)

echo.
echo Build complete. Output: %OUT%

:end
endlocal
pause
