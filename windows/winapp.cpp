#include "inc/winapp.h"

#include <stdexcept>

// windows callback
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE ||
        message == WM_NCCREATE) // WM_CREATE and WM_NCCreate needs to be handled
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    else if (WinApp::instance != nullptr)
    {
        return WinApp::instance->WndProcHandler(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Strings
const std::string WinApp::class_registration_failure_string = "Failed to register window class.";

// Global
WinApp* WinApp::instance = nullptr;

WinApp* WinApp::Create(HINSTANCE instance, std::wstring cmdLine, int cmdShow, std::wstring appName, std::wstring className)
{
    if (WinApp::instance != nullptr)
    {
        return WinApp::instance;
    }

    WinApp::instance = new WinApp{ instance, cmdLine, cmdShow, appName, className };

    return WinApp::instance;
}

void WinApp::Destroy()
{
    if (WinApp::instance != nullptr)
    {
        WinApp* winApp = WinApp::instance;
        WinApp::instance = nullptr;
        delete winApp;
    }
}

WinApp::WinApp(HINSTANCE instance, std::wstring cmdLine, int cmdShow, std::wstring appName, std::wstring className) :
    instance_(instance),
    cmdLine_(cmdLine),
    cmdShow_(cmdShow),
    appName_(appName),
    className_(className),
    registeredClass_(NULL)
{
    // Register class
    WNDCLASSEXW wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = className.c_str();

    registeredClass_ = RegisterClassExW(&wcex);

    if (registeredClass_ == NULL)
    {
        // throw exception due to error
        throw std::runtime_error(class_registration_failure_string);
    }

    AddWindow(className, appName);
    GetRootWindow().Show();
    GetRootWindow().Update();
}

int WinApp::Run()
{
    MSG msg = { 0 };

    // Main message loop:
    while (msg.message != WM_QUIT)
    {
        // Check to see if any messages are waiting in the queue
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Translate the message and dispatch it to WindowProc()
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

LRESULT WinApp::WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT res = 0;

    // Dispatch events to window class associated with hWnd.
    LONG_PTR windowPtr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (windowPtr)
    {
        Window* w = reinterpret_cast<Window*>(windowPtr);
        w->WndProc(hWnd, message, wParam, lParam);
    }

    return res;
}

bool WinApp::AddWindow(std::wstring className, std::wstring title)
{
    return AddWindow(className, title, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0);
}

bool WinApp::AddWindow(std::wstring className, std::wstring title, int x, int y, int width, int height)
{
    windowList_.push_back(Window{ instance_, className, title, cmdShow_, x, y, width, height });

    return true;
}

Window* WinApp::GetWindow(unsigned int id)
{
    if (isWindowIdInBounds(id))
    {
        return &(windowList_[id]);
    }

    return nullptr;
}

// cannot fail
const Window& WinApp::GetRootWindow() const
{
    return windowList_.at(WinApp::RootWindowIndex);
}

// Private functions:
bool WinApp::isWindowIdInBounds(unsigned int id) const
{
    return id < windowList_.size();
}