//
// Created by yangbin on 2022/1/27.
//

#include "base_flutter_window.h"

#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

namespace {
void CenterRectToMonitor(LPRECT prc) {
  HMONITOR hMonitor;
  MONITORINFO mi;
  RECT rc;
  int w = prc->right - prc->left;
  int h = prc->bottom - prc->top;

  //
  // get the nearest monitor to the passed rect.
  //
  hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);

  //
  // get the work area or entire monitor rect.
  //
  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor, &mi);

  rc = mi.rcMonitor;

  prc->left = rc.left + (rc.right - rc.left - w) / 2;
  prc->top = rc.top + (rc.bottom - rc.top - h) / 2;
  prc->right = prc->left + w;
  prc->bottom = prc->top + h;

}

std::wstring Utf16FromUtf8(const std::string &string) {
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, nullptr, 0);
  if (size_needed == 0) {
    return {};
  }
  std::wstring wstrTo(size_needed, 0);
  int converted_length = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, &wstrTo[0], size_needed);
  if (converted_length == 0) {
    return {};
  }
  return wstrTo;
}

}

void BaseFlutterWindow::Center() {
  auto handle = GetWindowHandle();
  if (!handle) {
    return;
  }
  RECT rc;
  GetWindowRect(handle, &rc);
  CenterRectToMonitor(&rc);
  SetWindowPos(handle, nullptr, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void BaseFlutterWindow::SetBounds(double_t x, double_t y, double_t width, double_t height) {
  auto handle = GetWindowHandle();
  if (!handle) {
    return;
  }
  MoveWindow(handle, int32_t(x), int32_t(y),
             static_cast<int>(width),
             static_cast<int>(height),
             TRUE);
}

void BaseFlutterWindow::SetTitle(const std::string &title) {
  auto handle = GetWindowHandle();
  if (!handle) {
    return;
  }
  SetWindowText(handle, Utf16FromUtf8(title).c_str());
}

void BaseFlutterWindow::Close() {
  auto handle = GetWindowHandle();
  if (!handle) {
    return;
  }
  PostMessage(handle, WM_SYSCOMMAND, SC_CLOSE, 0);
}

void BaseFlutterWindow::Show() {
  auto handle = GetWindowHandle();
  if (!handle) {
    return;
  }
  ShowWindow(handle, SW_SHOW);

}

void BaseFlutterWindow::Hide() {
  auto handle = GetWindowHandle();
  if (!handle) {
    return;
  }
  ShowWindow(handle, SW_HIDE);
}

void BaseFlutterWindow::SetTitleBarStyle(TitleBarStyle titleBarStyle, bool windowButtonVisibility) {
  title_bar_style = titleBarStyle;
  // Enables the ability to go from setAsFrameless() to
  // TitleBarStyle.normal/hidden
  is_frameless = false;
  
  MARGINS margins = { 0, 0, 0, 0 };
  HWND hWnd = GetWindowHandle();
  RECT rect;
  GetWindowRect(hWnd, &rect);
  DwmExtendFrameIntoClientArea(hWnd, &margins);
  SetWindowPos(hWnd, nullptr, rect.left, rect.top, 0, 0,
    SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE |
    SWP_FRAMECHANGED);
}

void BaseFlutterWindow::SetOpacity(double_t opacity) {
  HWND hWnd = GetWindowHandle();
  long gwlExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
  SetWindowLong(hWnd, GWL_EXSTYLE, gwlExStyle | WS_EX_LAYERED);
  SetLayeredWindowAttributes(hWnd, 0, static_cast<int8_t>(255 * opacity), LWA_ALPHA);
}

void BaseFlutterWindow::SetBackgroundColor(int32_t backgroundColorA, int32_t backgroundColorR, int32_t backgroundColorG, int32_t backgroundColorB) {
  HWND hWnd = GetWindowHandle();
  const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
  if (hModule) {
    typedef enum _ACCENT_STATE {
      ACCENT_DISABLED = 0,
      ACCENT_ENABLE_GRADIENT = 1,
      ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
      ACCENT_ENABLE_BLURBEHIND = 3,
      ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
      ACCENT_ENABLE_HOSTBACKDROP = 5,
      ACCENT_INVALID_STATE = 6
    } ACCENT_STATE;
    struct ACCENTPOLICY {
      int nAccentState;
      int nFlags;
      int nColor;
      int nAnimationId;
    };
    struct WINCOMPATTRDATA {
      int nAttribute;
      PVOID pData;
      ULONG ulDataSize;
    };
    typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND,
      WINCOMPATTRDATA*);
    const pSetWindowCompositionAttribute SetWindowCompositionAttribute =
      (pSetWindowCompositionAttribute)GetProcAddress(
        hModule, "SetWindowCompositionAttribute");
    if (SetWindowCompositionAttribute) {
      int32_t accent_state = ACCENT_ENABLE_TRANSPARENTGRADIENT;
      ACCENTPOLICY policy = {
          accent_state, 2,
          ((backgroundColorA << 24) + (backgroundColorB << 16) +
           (backgroundColorG << 8) + (backgroundColorR)),
          0 };
      WINCOMPATTRDATA data = { 19, &policy, sizeof(policy) };
      SetWindowCompositionAttribute(hWnd, &data);
    }
    FreeLibrary(hModule);
  }
}

bool BaseFlutterWindow::IsMaximized() {
  HWND mainWindow = GetWindowHandle();
  WINDOWPLACEMENT windowPlacement;
  GetWindowPlacement(mainWindow, &windowPlacement);

  return windowPlacement.showCmd == SW_MAXIMIZE;
}

void BaseFlutterWindow::StartDragging() {
  HWND hWnd = GetWindowHandle();
  ReleaseCapture();
  SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
}

void BaseFlutterWindow::SetMinimumSize(double devicePixelRatio, double width, double height) {
  if (width >= 0 && height >= 0) {
    pixel_ratio_ = devicePixelRatio;
    POINT point = {};
    point.x = static_cast<LONG>(width);
    point.y = static_cast<LONG>(height);
    minimum_size_ = point;
  }
}

void BaseFlutterWindow::SetMaximumSize(double devicePixelRatio, double width, double height) {
  if (width >= 0 && height >= 0) {
    pixel_ratio_ = devicePixelRatio;
    POINT point = {};
    point.x = static_cast<LONG>(width);
    point.y = static_cast<LONG>(height);
    maximum_size_ = point;
  }
}
