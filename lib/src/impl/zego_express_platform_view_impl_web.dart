import 'package:flutter/material.dart';
import 'dart:async';
import 'dart:html';
import 'dart:ui' as ui;

/// Web implementation of [createPlatformView]
class ZegoExpressPlatformViewImpl {
  /// Create a PlatformView and return the view ID
  static Widget? createPlatformView(Function(int viewID) onViewCreated,
      {Key? key}) {
    String webcamPushElement = 'plugins.zego.im/zego_express_view';
    // ignore:undefined_prefixed_name
    ui.platformViewRegistry.registerViewFactory(webcamPushElement, (int id) {
      return DivElement()..id = "zego-view-$id";
    });
    return HtmlElementView(
        key: key,
        viewType: webcamPushElement,
        onPlatformViewCreated: (int viewID) {
          const checkInterval = Duration(milliseconds: 10);
          // Maximum number of checks, maximum time consuming 1.5s
          var checks = 0;
          const maxChecks = 150;
          final elementId = "zego-view-$viewID";
          Timer.periodic(checkInterval, (timer) {
            final div = window.document.getElementById(elementId);
            if (div != null || checks >= maxChecks) {
              // Element found or timeout
              timer.cancel();
              onViewCreated(viewID);
            }
            checks++;
          });
        });
  }
}
