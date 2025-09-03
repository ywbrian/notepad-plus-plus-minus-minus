#include <iostream>
#include <windows.h>

constexpr LPCWSTR NOTEPAD_APP = L"C:\\Windows\\System32\\notepad.exe";
constexpr int MAX_WINDOW_WAIT_ATTEMPTS = 50;
constexpr int WINDOW_WAIT_DELAY_MS;

void setOptions(STARTUPINFOW* si) {
    si->dwFlags = STARTF_USESHOWWINDOW;
    si->wShowWindow = SW_SHOWMAXIMIZED;
}

bool launchNotepad(STARTUPINFOW* si, PROCESS_INFORMATION* pi) {
    ZeroMemory(si, sizeof(*si));
    si->cb = sizeof(*si);
    ZeroMemory(pi, sizeof(*pi));

    if (!CreateProcessW(NOTEPAD_APP, NULL, NULL, NULL, FALSE, 0, NULL, NULL, si,
                        pi)) {
        return false;
    }
    return true;
}
struct FindWindowData {
    DWORD pid;
    HWND hwnd;
};

HWND getNotepadHandle(DWORD pid) {
    FindWindowData data = {pid, NULL};
    EnumWindows([](HWND h, LPARAM lParam) -> BOOL {
        FindWindowData* pData = reinterpret_cast<FindWindowData*>(lParam);
        DWORD wpid;
        GetWindowThreadProcessId(h, &wpid);
        if (wpid == pData->pid) {
            pData->hwnd = h;
            return FALSE;  // Stop enumeration
        }
        return TRUE;  // Continue enumeration
    }, reinterpret_cast<LPARAM>(&data));
    return data.hwnd;
}

int main() {
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    setOptions(&si);

    if (!launchNotepad(&si, &pi)) {
        std::cerr << "Failed to launch Notepad.\n";
        return EXIT_FAILURE;
    }

    HWND hwndNotepad = nullptr;
    for (int i = 0; i < MAX_WINDOW_WAIT_ATTEMPTS; i++) {
        hwndNotepad = getNotepadHandle(pi.dwProcessId);
        if (hwndNotepad) break;
        Sleep(WINDOW_WAIT_DELAY_MS);
    }

    if (!hwndNotepad) {
        std::cerr << "Failed to find Notepad window.\n";
        return EXIT_FAILURE;
    }

    ShowWindow(hwndNotepad, SW_MAXIMIZE);

    HWND hwndEdit = FindWindowExW(hwndNotepad, NULL, L"Edit", NULL);
    const wchar_t* text = L"Hello, Notepad!";
    SendMessageW(hwndEdit, WM_SETTEXT, 0, (LPARAM)text);

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return EXIT_SUCCESS;
}