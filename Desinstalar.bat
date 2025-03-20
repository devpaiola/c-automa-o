@echo off
echo Desinstalando e removendo arquivos e registros...

:: Mata os processos
taskkill /F /IM spa.exe
taskkill /F /IM xspa.exe
taskkill /F /IM work.exe

:: Remove a pasta do %appdata%
if exist "%appdata%\spa" (
    rmdir /S /Q "%appdata%\spa"
    echo Pasta "spa" removida do %appdata%.
) else (
    echo Pasta "spa" não encontrada no %appdata%.
)

if exist "%temp%\install_done.flag" (
    del /S /Q "%temp%\install_done.flag"
    echo Arquivo Flag removido do %temp%.
) else (
    echo Arquivo "Flag" não encontrada no %temp%.
)

if exist "%temp%\install.exe" (
    del /S /Q "%temp%\install.exe"
    echo Arquivo Install.exe removido do %temp%.
) else (
    echo Pasta "spa" não encontrada no %temp%.
)

:: Remove a chave do registro
reg delete "HKEY_CURRENT_USER\Software\SmartComputadores" /f

echo Processo concluído.
pause