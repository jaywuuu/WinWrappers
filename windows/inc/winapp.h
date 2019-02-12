#ifndef __winapp_h_
#define __winapp_h_

#include "wincommon.h"
#include "window.h"

#include <string>
#include <unordered_map>
#include <vector>

class WinApp
{
public:
    enum { RootWindowIndex = 0 };

    static WinApp* instance;
    static WinApp* Create(HINSTANCE instance, std::wstring cmdLine, int cmdShow, std::wstring appName, std::wstring className);
    static void Destroy();

    int Run(); 
    LRESULT WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // Window management
    bool AddWindow(std::wstring className, std::wstring title);
    bool AddWindow(std::wstring className, std::wstring title, int x, int y, int width, int height);
    Window* GetWindow(unsigned int id);

    const Window& GetRootWindow() const;

private:
    static const std::string class_registration_failure_string;

    HINSTANCE instance_;
    std::wstring cmdLine_;
    int cmdShow_;

    std::wstring appName_;
    std::wstring className_;

    ATOM registeredClass_;

    // Window management
    std::vector<Window> windowList_;

private:
    WinApp(HINSTANCE instance, std::wstring cmdLine, int cmdShow, std::wstring appName, std::wstring className);

    bool isWindowIdInBounds(unsigned int id) const;
};

#endif