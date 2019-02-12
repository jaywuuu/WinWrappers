#ifndef __window_h__
#define __window_h__

#include "wincommon.h"

#include <string>

class Window
{
public:
    Window(HINSTANCE instance, std::wstring className, std::wstring title, int cmdShow, int x, int y, int width, int height);
    
    // OS Window callbacks
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // Window control
    void Show() const;
    void Hide() const;
    void Update() const;

private:
    static const std::string instance_is_null_error_string;
    static const std::string create_window_failed_string;

    struct WindowRect
    {
        int x;
        int y;
        int width;
        int height;
    };

    HINSTANCE instance_;
    std::wstring title_;
    std::wstring className_;
    int cmdShow_;
    HWND hWnd_;

    WindowRect rect_;
};

#endif // __window_h__