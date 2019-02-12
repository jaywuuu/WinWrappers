#include "inc/window.h"

#include <stdexcept>

const std::string Window::instance_is_null_error_string = "Input parameter 'instance' is null.";
const std::string Window::create_window_failed_string = "Failed to create window.";

Window::Window(HINSTANCE instance, std::wstring className, std::wstring title, int cmdShow, int x, int y, int width, int height) :
    instance_(instance),
    className_(className),
    title_(title),
    cmdShow_(cmdShow),
    hWnd_(NULL)
{
    if (instance == nullptr)
    {
        throw std::runtime_error(instance_is_null_error_string);
    }

    rect_.x = x;
    rect_.y = y;
    rect_.width = width;
    rect_.height = height;

    hWnd_ = CreateWindowW(className.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, x, y, width, height, nullptr, nullptr, instance, nullptr);

    if (hWnd_ == nullptr)
    {
        DWORD error = GetLastError();
        throw std::runtime_error(create_window_failed_string);
    }

    // Save this to user data.
    SetWindowLongPtr(hWnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

// OS Window callbacks
LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Window control
void Window::Show() const
{
    ShowWindow(hWnd_, SW_SHOW);
}

void Window::Hide() const
{
    ShowWindow(hWnd_, SW_HIDE);
}

void Window::Update() const
{
    UpdateWindow(hWnd_);
}