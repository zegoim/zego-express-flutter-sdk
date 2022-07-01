import 'package:flutter/material.dart';
import 'zego_express_impl.dart';
import 'zego_express_api.dart';
import 'zego_express_platform_view_impl.dart'
    if (dart.library.html) 'zego_express_platform_view_impl_web.dart';

extension ZegoExpressPlatformViewUtils on ZegoExpressEngine {
  /// Create a PlatformView and return the view ID
  Widget? createPlatformView(Function(int viewID) onViewCreated, {Key? key}) {
    return ZegoExpressPlatformViewImpl.createPlatformView(onViewCreated,
        key: key);
  }

  /// Destroy the PlatformView and releases its resources
  /// If it returns false, it's probably because the `viewID` to be destroyed doesn't exist.
  Future<bool> destroyPlatformView(int viewID) async {
    return await ZegoExpressImpl.methodChannel
        .invokeMethod('destroyPlatformView', {'viewID': viewID});
  }
}
