import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'zego_express_impl.dart';
import 'zego_express_api.dart';
import 'dart:html';
import 'dart:ui' as ui;

extension ZegoExpressPlatformViewUtils on ZegoExpressEngine {

    /// Create a PlatformView and return the view ID
    Widget? createPlatformView(Function(int viewID) onViewCreated, {Key? key}) {
        if (TargetPlatform.iOS == defaultTargetPlatform) {
            return UiKitView(
                key: key,
                viewType: 'plugins.zego.im/zego_express_view',
                onPlatformViewCreated: (int viewID) {
                      onViewCreated(viewID);
                }
            );
        } else if (TargetPlatform.android == defaultTargetPlatform) {

            return AndroidView(
                key: key,
                viewType: 'plugins.zego.im/zego_express_view',
                onPlatformViewCreated: (int viewID) {
                      onViewCreated(viewID);
                }
            );
        } else if (kIsWeb) {
          String webcamPushElement = 'plugins.zego.im/zego_express_view';
          // ignore:undefined_prefixed_name
          ui.platformViewRegistry.registerViewFactory(webcamPushElement, (int id) =>
            VideoElement()
            ..id = "zego-view-${id}"
            ..autoplay = true
            ..controls = true
          );

          return HtmlElementView(
            viewType: webcamPushElement,
            onPlatformViewCreated: (int viewID) {
              onViewCreated(viewID);
            }
          );
        }

        return null;
    }

    /// Destroy the PlatformView and releases its resources
    /// If it returns false, it's probably because the `viewID` to be destroyed doesn't exist.
    Future<bool> destroyPlatformView(int viewID) async {
        return await ZegoExpressImpl.methodChannel.invokeMethod('destroyPlatformView', {
            'viewID': viewID
        });
    }
}