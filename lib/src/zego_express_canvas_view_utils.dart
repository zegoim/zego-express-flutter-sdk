//
//  zego_express_canvas_view_utils.dart
//  flutter
//
//  Created by Patrick Fu on 2022/08/30.
//  Copyright © 2022 Zego. All rights reserved.
//

import 'package:flutter/material.dart';
import 'impl/zego_express_canvas_view_impl.dart';
import 'zego_express_api.dart';

extension ZegoExpressCanvasViewUtils on ZegoExpressEngine {
  /// Create a canvas view.
  ///
  /// When you need to render local preview video through [startPreview],
  /// or render remote playing stream's video through [startPlayingStream],
  /// you need to call this function to create a canvas view first.
  ///
  /// This function will create a canvas view (actully a [PlatformView] or
  /// a [ExternalTexture] in native (or web) side), and return a corresponding
  /// flutter widget which can be added into the flutter render tree.
  ///
  /// You should also listen to the [onViewCreated] callback, which will be
  /// called back when the canvas view has been created, then you can use
  /// the [viewID] to create a [ZegoCanvas] object for video rendering.
  Future<Widget?> createCanvasView(Function(int viewID) onViewCreated,
      {Key? key}) async {
    return await ZegoExpressCanvasViewImpl.createCanvasView(onViewCreated,
        key: key);
  }

  /// Destroy a canvas view.
  ///
  /// When you no longer need to use a canvas view, you need to call
  /// this function to destroy it to release resources,
  /// otherwise it will cause memory leaks.
  ///
  /// If it returns false, it's probably because the canvas view
  /// (in native or web side) to be destroyed doesn't exist or
  /// the ZegoExpressEngine instance is not created.
  ///
  /// The [viewID] is the id of the corresponding canvas view,
  /// which can be obtained from the [onViewCreated] callback of
  /// the [createCanvasView] function.
  Future<bool> destroyCanvasView(int viewID) async {
    return await ZegoExpressCanvasViewImpl.destroyCanvasView(viewID);
  }
}
