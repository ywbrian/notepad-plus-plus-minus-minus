#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <iostream>

class NPPMMWindow {
  public:
    NPPMMWindow();
    ~NPPMMWindow();

    void launch();
    HWND hwnd() const {
        return hwndNotepad_;
    }
    DWORD pid() const {
        return pi_.dwProcessId;
    }
    bool is_running() const;

  private:
    struct WindowData {
        DWORD pid;
        HWND hwnd;
    };

    inline static constexpr LPCWSTR NOTEPAD_APP =
        L"C:\\Windows\\System32\\notepad.exe";
    static constexpr int MAX_WINDOW_WAIT_ATTEMPTS = 50;
    static constexpr int WINDOW_WAIT_DELAY_MS = 20;

    STARTUPINFOW si_{};
    PROCESS_INFORMATION pi_{};
    HWND hwndNotepad_ = nullptr;
    HWND hwndEdit_ = nullptr;

    /**
     * @brief Launch Notepad.
     */
    bool launchNotepad();

    /**
     * @brief Retrieve the window handle to the Notepad instance of this program
     *        and store it in hwndNotepad.
     */
    void getNotepadHandle();

    /**
     * @brief Retrieve the edit handle to the Notepad instance of this program
     *        and store it in hwndEdit.
     */
    void getEditHandle();
};

#endif