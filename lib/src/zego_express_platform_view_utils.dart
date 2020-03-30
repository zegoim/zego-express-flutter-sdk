
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'zego_express_impl.dart';
import 'zego_express_api.dart';

extension ZegoExpressPlatformViewUtils on ZegoExpressEngine {

    /// Create a PlatformView and return the view ID
    Widget createPlatformView(Key key, Function(int viewID) onViewCreated) {
        if (TargetPlatform.iOS == defaultTargetPlatform) {
            return UiKitView(
                key: key,
                viewType: 'plugins.zego.im/zego_express_view',
                onPlatformViewCreated: (int viewID) {
                    if (onViewCreated != null) {
                        onViewCreated(viewID);
                    }
                }
            );
        } else if (TargetPlatform.android == defaultTargetPlatform) {

            return AndroidView(
                key: key,
                viewType: 'plugins.zego.im/zego_express_view',
                onPlatformViewCreated: (int viewID) {
                    if (onViewCreated != null) {
                        onViewCreated(viewID);
                    }
                }
            );
        }

        return null;
    }

    /// Destroy the PlatformView and releases its resources
    Future<bool> destroyPlatformView(int viewID) async {
        final bool success = await ZegoExpressImpl.methodChannel.invokeMethod('destroyPlatformView', {
            'viewID': viewID
        });

        return success;
    }
}