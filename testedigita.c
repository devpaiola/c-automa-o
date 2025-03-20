#include <windows.h>
#include <stdio.h>

int main() {
    // Encontrando a janela do Notepad (com o título padrão)
    HWND hwnd = FindWindow(NULL, "Sem título - Bloco de notas"); // Título padrão do Notepad
    if (hwnd == NULL) {
        printf("Janela do Notepad não encontrada!\n");
        return 1;
    }

    // Enviar o caractere 'A' para a janela do Notepad
    SendMessage(hwnd, WM_CHAR, 'A', 0);

    return 0;
}
