#include "nppmm_window.h"

NPPMMWindow::NPPMMWindow() {
    ZeroMemory(&si_, sizeof(si_));
    si_.cb = sizeof(si_);

    si_.dwFlags = STARTF_USESHOWWINDOW;
    si_.wShowWindow = SW_SHOWMAXIMIZED;

    ZeroMemory(&pi_, sizeof(pi_));
}

NPPMMWindow::~NPPMMWindow() {
    if (pi_.hProcess) {
        CloseHandle(pi_.hProcess);
    }
    if (pi_.hThread) {
        CloseHandle(pi_.hThread);
    }
}

bool NPPMMWindow::is_running() const {
    if (pi_.hProcess == nullptr) {
        return false;
    }

    DWORD exitCode;
    if (GetExitCodeProcess(pi_.hProcess, &exitCode)) {
        return exitCode == STILL_ACTIVE;
    }

    return false;
}

void NPPMMWindow::launch() {
    launchNotepad();
    getNotepadHandle();
    getEditHandle();
}

bool NPPMMWindow::launchNotepad() {
    if (!CreateProcessW(NOTEPAD_APP, NULL, NULL, NULL, FALSE, 0, NULL, NULL,
                        &si_, &pi_)) {
        std::cerr << "Failed to launch Notepad. Error: " << GetLastError() << '\n';
        return false;
    }
    return true;
}

void NPPMMWindow::getNotepadHandle() {
    WindowData data = {pi_.dwProcessId, nullptr};

    for (int i = 0; i < MAX_WINDOW_WAIT_ATTEMPTS && !data.hwnd; i++) {
        EnumWindows(
            [](HWND hwnd, LPARAM lParam) -> BOOL {
                auto* pData = reinterpret_cast<WindowData*>(lParam);
                DWORD wpid;
                GetWindowThreadProcessId(hwnd, &wpid);
                if (wpid == pData->pid) {
                    pData->hwnd = hwnd;
                    return FALSE; // stop enumeration
                }
                return TRUE; // continue enumeration
            },
            reinterpret_cast<LPARAM>(&data));
        
        if (!data.hwnd) {
            Sleep(WINDOW_WAIT_DELAY_MS);
        }
    }

    hwndNotepad_ = data.hwnd;

    if (hwndNotepad_ == nullptr) {
        std::cerr << "Failed to get Notepad handle\n";
    }
}

void NPPMMWindow::getEditHandle() {
    hwndEdit_ = FindWindowExW(hwndNotepad_, NULL, L"Edit", NULL);
    if (hwndEdit_ == nullptr) {
        std::cerr << "Failed to get edit handle\n";
    }
}