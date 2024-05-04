//
// Created by yangbin on 2022/1/27.
//

#ifndef MULTI_WINDOW_WINDOWS_BASE_FLUTTER_WINDOW_H_
#define MULTI_WINDOW_WINDOWS_BASE_FLUTTER_WINDOW_H_

#include "window_channel.h"

enum TitleBarStyle {
  normal,
  hidden,
};

class BaseFlutterWindow {

 public:

  virtual ~BaseFlutterWindow() = default;

  virtual WindowChannel *GetWindowChannel() = 0;

  void Show();

  void Hide();

  void Close();

  void SetTitle(const std::string &title);

  void SetBounds(double_t x, double_t y, double_t width, double_t height);

  void Center();

  void SetTitleBarStyle(TitleBarStyle titleBarStyle, bool windowButtonVisibility);

  void SetOpacity(double_t opacity);

  void SetBackgroundColor(int32_t backgroundColorA, int32_t backgroundColorR, int32_t backgroundColorG, int32_t backgroundColorB);

  bool IsMaximized();

 protected:

  virtual HWND GetWindowHandle() = 0;

  bool is_frameless;
  TitleBarStyle title_bar_style;
};

#endif //MULTI_WINDOW_WINDOWS_BASE_FLUTTER_WINDOW_H_
