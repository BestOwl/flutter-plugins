import 'package:flutter/widgets.dart';

import '../../desktop_multi_window.dart';

class MoveWindow extends StatelessWidget {
  MoveWindow(
      {super.key, required this.controller, this.child, this.onDoubleTap});
  final WindowController controller;
  final Widget? child;
  final VoidCallback? onDoubleTap;
  @override
  Widget build(BuildContext context) {
    return GestureDetector(
        behavior: HitTestBehavior.translucent,
        onPanStart: (details) {
          controller.startDragging();
        },
        onDoubleTap: onDoubleTap ?? () => controller.maximizeOrRestore(),
        child: child ?? SizedBox.shrink());
  }
}
