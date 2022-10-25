import 'package:flutter/material.dart';
import 'dart:html';
import 'dart:ui' as ui;

/// Web implementation of [createPlatformView]
class ZegoExpressPlatformViewImpl {
  /// Create a PlatformView and return the view ID
  static Widget? createPlatformView(Function(int viewID) onViewCreated,
      {Key? key}) {
    String webcamPushElement = 'plugins.zego.im/zego_express_view';
    // ignore:undefined_prefixed_name
    ui.platformViewRegistry.registerViewFactory(
        webcamPushElement,
        (int id) => VideoElement()
          ..id = "zego-view-$id"
          ..autoplay = true);
    return HtmlElementView(
        key: key,
        viewType: webcamPushElement,
        onPlatformViewCreated: (int viewID) {
          onViewCreated(viewID);
        });
  }
}
