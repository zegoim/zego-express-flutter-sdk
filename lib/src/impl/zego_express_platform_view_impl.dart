import 'package:flutter/material.dart';
import '../utils/zego_express_utils.dart';

/// Native implementation of [createPlatformView]
class ZegoExpressPlatformViewImpl {
  static Widget? Function(Function(int viewID))? onWindowsPlatformViewCreated;

  /// Create a PlatformView and return the view ID
  static Widget? createPlatformView(Function(int viewID) onViewCreated,
      {Key? key}) {
    if (kIsIOS || kIsMacOS) {
      return UiKitView(
          key: key,
          viewType: 'plugins.zego.im/zego_express_view',
          onPlatformViewCreated: (int viewID) {
            onViewCreated(viewID);
          });
    } else if (kIsAndroid) {
      return AndroidView(
          key: key,
          viewType: 'plugins.zego.im/zego_express_view',
          onPlatformViewCreated: (int viewID) {
            onViewCreated(viewID);
          });
    } else if (kIsWindows) {
      if (onWindowsPlatformViewCreated != null) {
        return onWindowsPlatformViewCreated!(onViewCreated);
      }
    }
    return null;
  }
}
