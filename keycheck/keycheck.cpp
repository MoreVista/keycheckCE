#include <windows.h>
#include <stdio.h>
#include <stdafx.h>

// グローバル変数
HINSTANCE g_hInstance;
bool g_running = true;  // アプリの実行状態を管理

// ボタンのID
#define ID_CLOSE_BUTTON 1001

// ウィンドウプロシージャ
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
                g_running = false;  // メインループを抜ける
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

    // ウィンドウクラス登録
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"KeyLoggerWindow";
    RegisterClass(&wc);

    // ウィンドウ作成
    HWND hwnd = CreateWindow(L"KeyLoggerWindow", L"Key Logger", WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
                             NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBox(NULL, L"ウィンドウの作成に失敗しました", L"エラー", MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // メインループ
    while (g_running) {
        // 入力イベントを待機
        MsgWaitForMultipleObjects(0, NULL, FALSE, INFINITE, QS_ALLINPUT);

        // メッセージループ処理
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // WM_QUIT を受け取ったらループを抜ける
            if (msg.message == WM_QUIT) {
                g_running = false;
            }
        }

        // キー入力をチェック（アプリが動作中の場合のみ）
        if (!g_running) break;
        
        for (int vk = 1; vk < 256; vk++) {
            if (GetAsyncKeyState(vk) & 0x8000) {
                // 閉じるボタンの影響を防ぐ
                if (vk == VK_LBUTTON || vk == VK_RBUTTON) continue;

                wchar_t msg[256];
                wsprintf(msg, L"Key Pressed: 0x%X", vk);
                MessageBox(NULL, msg, L"Key Code", MB_OK);
                Sleep(500);  // 連続で表示しないように待機
            }
        }
    }

    return 0;
}
