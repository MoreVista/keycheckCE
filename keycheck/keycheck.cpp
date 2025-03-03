#include <windows.h>
#include <stdio.h>
#include <stdafx.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    while (true) {
        for (int vk = 1; vk < 256; vk++) {
            if (GetAsyncKeyState(vk) & 0x8000) {
                wchar_t msg[256];
                wsprintf(msg, L"Key Pressed: 0x%X", vk);
                MessageBox(NULL, msg, L"Key Code", MB_OK);
                Sleep(500);  // ˜A‘±‚Å•\Ž¦‚µ‚È‚¢‚æ‚¤‚É‘Ò‹@
            }
        }
        Sleep(50);
    }
    return 0;
}
