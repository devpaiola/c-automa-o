#ifndef TESTEDIGITA_H
#define TESTEDIGITA_H

#include <windows.h>

HWND FindWindowByTitle(LPCTSTR title);
void SendStringToWindow(const char* windowTitle, const char* textToSend);
void SendSpecialKey(WORD key);
void InserirCredenciais(); // Adicionado para inserir credenciais

#endif // TESTEDIGITA_H