#include <Windows.h>
#include <WinUser.h>
#include <winerror.h>
#include <stdio.h>

int upper_case_char_diff = 'a' - 'A';

void pressKey(WORD key) {
    INPUT inputs[1] = {};
    ZeroMemory(inputs, 1);

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0;
    inputs[0].ki.wScan = MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
    inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;

    UINT uSent = SendInput(1, inputs, sizeof(INPUT));
    if (uSent != 1)
    {
        printf("pressKey failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}

void releaseKey(WORD key) {
    INPUT inputs[1] = {};
    ZeroMemory(inputs, 1);

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0;
    inputs[0].ki.wScan = MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
    inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    UINT uSent = SendInput(1, inputs, sizeof(INPUT));
    if (uSent != 2)
    {
        printf("releaseKey failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}

void typeKey(WORD key) {
    INPUT inputs[2] = {};
    ZeroMemory(inputs, 1);

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0;
    inputs[0].ki.wScan = MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
    inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0;
    inputs[1].ki.wScan = MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
    inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    UINT uSent = SendInput(2, inputs, sizeof(INPUT));
    if (uSent != 2)
    {
        printf("typeKey failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}

void writeText(const char* str) {

    int i = 0;

    while (str[i] != '\0') {
        
        if (str[i] >= 'a' && str[i] <= 'z') {
            typeKey(str[i] - upper_case_char_diff);
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            pressKey(VK_SHIFT);
            typeKey(str[i]);
            releaseKey(VK_SHIFT);
        } else if (str[i] == '\0') {
            return;
        } else {
            printf("Unrecognized character: %c%n", str[i]);
            return;
        }

        i++;
    }

}

void toggleConsole() {
    INPUT inputs[4] = {};
    ZeroMemory(inputs, 4);

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0;
    inputs[0].ki.wScan = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
    inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0;
    inputs[1].ki.wScan = MapVirtualKeyA(VK_OEM_3, MAPVK_VK_TO_VSC);
    inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 0;
    inputs[2].ki.wScan = MapVirtualKeyA(VK_OEM_3, MAPVK_VK_TO_VSC);
    inputs[2].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = 0;
    inputs[3].ki.wScan = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
    inputs[3].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    UINT uSent = SendInput(4, inputs, sizeof(INPUT));
    if (uSent != 4)
    {
        printf("toggleConsole failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}

void writeCommand( char *command[], int size) {

    toggleConsole();

    for (int i = 0; i < size - 1; i++) {
        writeText(command[i]);
        typeKey(VK_SPACE);
    }

    writeText(command[size-1]);
    typeKey(VK_RETURN);
    toggleConsole();
    
}

int main(int argc, char *argv[]) {
    
    writeCommand(argv+1, argc-1);
}