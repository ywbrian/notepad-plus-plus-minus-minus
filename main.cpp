#include <windows.h>
#include <iostream>

constexpr LPCWSTR NOTEPAD_APP = L"C:\\Windows\\System32\\notepad.exe";

bool launchNotepad(STARTUPINFOW* si, PROCESS_INFORMATION* pi) {
    ZeroMemory(si, sizeof(*si));
    si->cb = sizeof(*si);
    ZeroMemory(pi, sizeof(*pi));

    if (!CreateProcessW(
        NOTEPAD_APP, 
        NULL,
        NULL, NULL, 
        FALSE,
        0,
        NULL, NULL,
        si, pi)) {
            return false;
        }
    return true;
}

int main() {
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}