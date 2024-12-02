#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

// 숫자를 쉼표 포함 형식으로 변환
wstring formatWithCommas(long long value) {
    wstringstream ss;
    ss.imbue(locale(""));
    ss << fixed << value;
    return ss.str();
}

// 드라이브별 하드디스크 용량 가져오기
vector<pair<wstring, wstring>> getDriveCapacities() {
    vector<pair<wstring, wstring>> driveInfo;
    wchar_t drive = L'A';
    wchar_t drivePath[4] = L"A:\\";

    for (; drive <= L'Z'; drive++) {
        drivePath[0] = drive;
        ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

        if (GetDiskFreeSpaceExW(drivePath, &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
            wstringstream ss;
            ss << L"드라이브 " << drive << L":";
            driveInfo.push_back({ ss.str(), formatWithCommas(totalBytes.QuadPart) + L" 바이트" });
        }
    }

    return driveInfo;
}

// GUI 콜백 함수
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CREATE: {
        // 드라이브 정보를 가져와 화면에 표시
        vector<pair<wstring, wstring>> drives = getDriveCapacities();
        int yPos = 10;
        for (const auto& drive : drives) {
            wstring displayText = drive.first + L" " + drive.second;
            CreateWindowW(L"STATIC", displayText.c_str(), WS_VISIBLE | WS_CHILD, 10, yPos, 500, 20, hwnd, NULL, NULL, NULL);
            yPos += 25;
        }
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    // 윈도우 클래스 설정
    const wchar_t CLASS_NAME[] = L"DriveInfoWindow";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // 윈도우 생성
    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"하드디스크 용량 정보", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, NULL, NULL, wc.hInstance, NULL);

    if (hwnd == NULL) {
        wcout << L"창 생성 실패" << endl;
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);

    // 메시지 루프
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
