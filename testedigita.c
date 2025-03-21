#include <windows.h>
#include <stdio.h>
#include "testedigita.h"

HWND FindWindowByTitle(LPCTSTR title) {
    return FindWindow(NULL, title);
}

void SendStringToWindow(const char* windowTitle, const char* textToSend) {
    HWND hwnd = FindWindowByTitle(windowTitle);
    if (hwnd == NULL) {
        printf("Janela não encontrada.\n");
        return;
    }

    // Define a janela encontrada como a janela ativa
    SetForegroundWindow(hwnd);

    // Envia cada caractere da string como entrada de teclado
    for (const char* p = textToSend; *p; ++p) {
        SHORT vk = VkKeyScan(*p);
        if (vk == -1) {
            continue;
        }

        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = vk;

        SendInput(1, &input, sizeof(INPUT));

        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));

        Sleep(50);
    }
}

void SendSpecialKey(WORD key) {
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;

    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void InserirCredenciais() {
    const char* windowTitle = "Dados da Conta";
    const char* UsuarioSend = "teste";
    const char* SenhaSend = "teste1234";
    const char* AmbienteSend = "testeteste";

    HWND hwnd = FindWindowByTitle(windowTitle);

    if (hwnd == NULL) {
        printf("Janela não encontrada.\n");
        return;
    }

    SetForegroundWindow(hwnd);
    SendSpecialKey(VK_TAB);

    SendStringToWindow(windowTitle, AmbienteSend);

    SendSpecialKey(VK_TAB);

    SendStringToWindow(windowTitle, SenhaSend);

    SendSpecialKey(VK_TAB);

    SendStringToWindow(windowTitle, UsuarioSend);

    SendSpecialKey(VK_RETURN);
}