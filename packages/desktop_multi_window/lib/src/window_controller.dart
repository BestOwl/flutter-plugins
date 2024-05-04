import 'dart:ui';

import '../desktop_multi_window.dart';
import 'window_controller_impl.dart';

/// The [WindowController] instance that is used to control this window.
abstract class WindowController {
  WindowController();

  factory WindowController.fromWindowId(int id) {
    return WindowControllerMainImpl(id);
  }

  factory WindowController.main() {
    return WindowControllerMainImpl(0);
  }

  /// The id of the window.
  /// 0 means the main window.
  int get windowId;

  /// Close the window.
  Future<void> close();

  /// Show the window.
  Future<void> show();

  /// Hide the window.
  Future<void> hide();

  /// Set the window frame rect.
  Future<void> setFrame(Rect frame);

  /// Center the window on the screen.
  Future<void> center();

  /// Set the window's title.
  Future<void> setTitle(String title);

  /// Whether the window can be resized. Available only on macOS.
  ///
  /// Most useful for ensuring windows *cannot* be resized. Windows are
  /// resizable by default, so there is no need to explicitly define a window
  /// as resizable by calling this function.
  Future<void> resizable(bool resizable);

  /// Available only on macOS.
  Future<void> setFrameAutosaveName(String name);

  /// Changes the title bar style of native window.
  Future<void> setTitleBarStyle(
    TitleBarStyle titleBarStyle, {
    bool windowButtonVisibility = true,
  });

  /// Sets the opacity of the window.
  Future<void> setOpacity(double opacity);

  /// Sets the background color of the window.
  Future<void> setBackgroundColor(Color backgroundColor);

  /// Starts a window drag based on the specified mouse-down event.
  Future<void> startDragging();

  Future<void> maximizeOrRestore();

  /// Returns `bool` - Whether the window is maximized.
  Future<bool> isMaximized();

  /// Maximizes the window. `vertically` simulates aero snap, only works on Windows
  Future<void> maximize({bool vertically = false});

  /// Unmaximizes the window.
  Future<void> unmaximize();

  /// Returns `bool` - Whether the window is minimized.
  Future<bool> isMinimized();

  /// Minimizes the window. On some platforms the minimized window will be shown in the Dock.
  Future<void> minimize();

  /// Restores the window from minimized state to its previous state.
  Future<void> restore();
}
