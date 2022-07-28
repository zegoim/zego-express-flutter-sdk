import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'zego_express_texture_render_utils.dart';
import 'zego_express_platform_view_utils.dart';
import 'zego_express_impl.dart';
import 'zego_express_api.dart';

extension ZegoExpressCanvasViewImpl on ZegoExpressEngine {
  /// Create a Canvas View and return the callback ZegoCanvas's viewID
  Future<Widget?> createCanvasView(Function(int viewID) onViewCreated,
      {Key? key}) async {
    Widget? widget;
    if (ZegoExpressImpl.isCreateEngine) {
      /// web only has PlatformView, windows only has TextureRenderer
      bool usePlatformView =
          ZegoExpressImpl.enablePlatformView && !Platform.isWindows;
      usePlatformView = usePlatformView || kIsWeb;
      if (usePlatformView) {
        widget = ZegoExpressEngine.instance.createPlatformView(onViewCreated);
      } else {
        final int textureID =
            await ZegoExpressEngine.instance.createTextureRenderer(0, 0);
        widget = LayoutBuilder(builder: ((context, constraints) {
          final double pixelRatio = MediaQuery.of(context).devicePixelRatio;
          ZegoExpressEngine.instance.updateTextureRendererSize(
              textureID,
              (constraints.biggest.width * pixelRatio).toInt(),
              (constraints.biggest.height * pixelRatio).toInt());
          return Texture(textureId: textureID);
        }));
        onViewCreated.call(textureID);
      }
    }

    return widget;
  }

  /// Destroys the Canvas View and releases its resources
  /// If it returns false, it's probably because the `textureID` to be destroyed doesn't exist or ZegoExpressEngine not create.
  Future<bool> destroyCanvasView(int viewID) async {
    if (ZegoExpressImpl.isCreateEngine) {
      if (ZegoExpressImpl.enablePlatformView) {
        return ZegoExpressEngine.instance.destroyTextureRenderer(viewID);
      } else {
        return ZegoExpressEngine.instance.destroyPlatformView(viewID);
      }
    }
    return false;
  }
}
