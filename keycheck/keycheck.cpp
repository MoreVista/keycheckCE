#include <windows.h>
#include <stdio.h>
#include <stdafx.h>

// �O���[�o���ϐ�
HINSTANCE g_hInstance;
bool g_running = true;  // �A�v���̎��s��Ԃ��Ǘ�

// �{�^����ID
#define ID_CLOSE_BUTTON 1001

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            CreateWindow(L"BUTTON", L"Exit", WS_VISIBLE | WS_CHILD,
                         60, 50, 100, 40, hwnd, (HMENU)ID_CLOSE_BUTTON,
                         g_hInstance, NULL);
            break;
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_CLOSE_BUTTON) {
                g_running = false;  // ���C�����[�v�𔲂���
                PostQuitMessage(0);
            }
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            ExtTextOut(hdc, 10, 10, 0, NULL, L"Key Logger Running...", 21, NULL);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            g_running = false;
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    g_hInstance = hInstance;

    // �E�B���h�E�N���X�o�^
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"KeyLoggerWindow";
    RegisterClass(&wc);

    // �E�B���h�E�쐬
    HWND hwnd = CreateWindow(L"KeyLoggerWindow", L"Key Logger", WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
                             NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBox(NULL, L"�E�B���h�E�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // ���C�����[�v
    while (g_running) {
        // ���̓C�x���g��ҋ@
        MsgWaitForMultipleObjects(0, NULL, FALSE, INFINITE, QS_ALLINPUT);

        // ���b�Z�[�W���[�v����
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // WM_QUIT ���󂯎�����烋�[�v�𔲂���
            if (msg.message == WM_QUIT) {
                g_running = false;
            }
        }

        // �L�[���͂��`�F�b�N�i�A�v�������쒆�̏ꍇ�̂݁j
        if (!g_running) break;
        
        for (int vk = 1; vk < 256; vk++) {
            if (GetAsyncKeyState(vk) & 0x8000) {
                // ����{�^���̉e����h��
                if (vk == VK_LBUTTON || vk == VK_RBUTTON) continue;

                wchar_t msg[256];
                wsprintf(msg, L"Key Pressed: 0x%X", vk);
                MessageBox(NULL, msg, L"Key Code", MB_OK);
                Sleep(500);  // �A���ŕ\�����Ȃ��悤�ɑҋ@
            }
        }
    }

    return 0;
}
