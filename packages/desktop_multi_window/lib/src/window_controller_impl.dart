import 'dart:io';
import 'dart:ui';

import 'package:flutter/services.dart';

import 'channels.dart';
import 'title_bar_style.dart';
import 'window_controller.dart';

class WindowControllerMainImpl extends WindowController {
  final MethodChannel _channel = multiWindowChannel;

  // the id of this window
  final int _id;

  WindowControllerMainImpl(this._id);

  @override
  int get windowId => _id;

  @override
  Future<void> close() {
    return _channel.invokeMethod('close', _id);
  }

  @override
  Future<void> hide() {
    return _channel.invokeMethod('hide', _id);
  }

  @override
  Future<void> show() {
    return _channel.invokeMethod('show', _id);
  }

  @override
  Future<void> center() {
    return _channel.invokeMethod('center', _id);
  }

  @override
  Future<void> setFrame(Rect frame) {
    return _channel.invokeMethod('setFrame', <String, dynamic>{
      'windowId': _id,
      'left': frame.left,
      'top': frame.top,
      'width': frame.width,
      'height': frame.height,
    });
  }

  @override
  Future<void> setTitle(String title) {
    return _channel.invokeMethod('setTitle', <String, dynamic>{
      'windowId': _id,
      'title': title,
    });
  }

  @override
  Future<void> resizable(bool resizable) {
    if (Platform.isMacOS) {
      return _channel.invokeMethod('resizable', <String, dynamic>{
        'windowId': _id,
        'resizable': resizable,
      });
    } else {
      throw MissingPluginException(
        'This functionality is only available on macOS',
      );
    }
  }

  @override
  Future<void> setFrameAutosaveName(String name) {
    return _channel.invokeMethod('setFrameAutosaveName', <String, dynamic>{
      'windowId': _id,
      'name': name,
    });
  }

  @override
  Future<void> setTitleBarStyle(TitleBarStyle titleBarStyle,
      {bool windowButtonVisibility = true}) {
    return _channel.invokeMethod('setTitleBarStyle', <String, dynamic>{
      'windowId': _id,
      'titleBarStyle': titleBarStyle.name,
      'windowButtonVisibility': windowButtonVisibility,
    });
  }

  @override
  Future<void> setOpacity(double opacity) {
    return _channel.invokeMethod('setOpacity', <String, dynamic>{
      'windowId': _id,
      'opacity': opacity,
    });
  }

  @override
  Future<void> setBackgroundColor(Color backgroundColor) {
    return _channel.invokeMethod('setBackgroundColor', <String, dynamic>{
      'windowId': _id,
      'backgroundColorA': backgroundColor.alpha,
      'backgroundColorR': backgroundColor.red,
      'backgroundColorG': backgroundColor.green,
      'backgroundColorB': backgroundColor.blue,
    });
  }

  @override
  Future<bool> isMaximized() {
    // TODO: implement isMaximized
    throw UnimplementedError();
  }

  @override
  Future<bool> isMinimized() {
    // TODO: implement isMinimized
    throw UnimplementedError();
  }

  @override
  Future<void> maximize({bool vertically = false}) {
    // TODO: implement maximize
    throw UnimplementedError();
  }

  @override
  Future<void> maximizeOrRestore() {
    // TODO: implement maximizeOrRestore
    throw UnimplementedError();
  }

  @override
  Future<void> minimize() {
    // TODO: implement minimize
    throw UnimplementedError();
  }

  @override
  Future<void> restore() {
    // TODO: implement restore
    throw UnimplementedError();
  }

  @override
  Future<void> startDragging() {
    return _channel.invokeMethod('startDragging', _id);
  }

  @override
  Future<void> unmaximize() {
    // TODO: implement unmaximize
    throw UnimplementedError();
  }
}
