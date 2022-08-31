import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

/// Native implementation of [createPlatformView]
class ZegoExpressPlatformViewImpl {
  /// Create a PlatformView and return the view ID
  static Widget? createPlatformView(Function(int viewID) onViewCreated,
      {Key? key}) {
    if (TargetPlatform.iOS == defaultTargetPlatform ||
        TargetPlatform.macOS == defaultTargetPlatform) {
      return UiKitView(
          key: key,
          viewType: 'plugins.zego.im/zego_express_view',
          onPlatformViewCreated: (int viewID) {
            onViewCreated(viewID);
          });
    } else if (TargetPlatform.android == defaultTargetPlatform) {
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
