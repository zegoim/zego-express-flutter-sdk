import 'dart:io';
import 'package:flutter/material.dart';

/// Native implementation of [createPlatformView]
class ZegoExpressPlatformViewImpl {
  /// Create a PlatformView and return the view ID
  static Widget? createPlatformView(Function(int viewID) onViewCreated,
      {Key? key}) {
    if (Platform.isIOS || Platform.isMacOS) {
      return UiKitView(
          key: key,
          viewType: 'plugins.zego.im/zego_express_view',
          onPlatformViewCreated: (int viewID) {
            onViewCreated(viewID);
          });
    } else if (Platform.isAndroid) {
      return AndroidView(
          key: key,
          viewType: 'plugins.zego.im/zego_express_view',
          onPlatformViewCreated: (int viewID) {
            onViewCreated(viewID);
          });
    }
    return null;
  }
}
