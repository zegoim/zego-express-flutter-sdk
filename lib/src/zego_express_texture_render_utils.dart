import 'dart:async';
import 'dart:io';

import 'zego_express_impl.dart';
import 'zego_express_api.dart';

extension ZegoExpressTextureRenderUtils on ZegoExpressEngine {
  /// Create a Texture renderer and return the texture ID
  Future<int> createTextureRenderer(int width, int height) async {
    final int textureID = await ZegoExpressImpl.methodChannel.invokeMethod(
        'createTextureRenderer', {'width': width, 'height': height});

    return textureID;
  }

  /// Update the size of the Texture renderer
  /// If it returns false, it's probably because the `textureID` to be updated doesn't exist.
  Future<bool> updateTextureRendererSize(
      int textureID, int width, int height) async {
    if (Platform.isAndroid) {
      return await ZegoExpressImpl.methodChannel.invokeMethod(
          'updateTextureRendererSize',
          {'textureID': textureID, 'width': width, 'height': height});
    } else {
      return true;
    }
  }

  /// Destroys the Texture renderer and releases its resources
  /// If it returns false, it's probably because the `textureID` to be destroyed doesn't exist.
  Future<bool> destroyTextureRenderer(int textureID) async {
    return await ZegoExpressImpl.methodChannel
        .invokeMethod('destroyTextureRenderer', {'textureID': textureID});
  }
}
