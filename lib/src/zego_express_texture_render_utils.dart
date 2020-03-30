
import 'zego_express_impl.dart';
import 'zego_express_api.dart';

extension ZegoExpressTextureRenderUtils on ZegoExpressEngine {

    /// Create a Texture renderer and return the texture ID
    Future<int> createTextureRenderer(int width, int height) async {
        final int textureID = await ZegoExpressImpl.methodChannel.invokeMethod('createTextureRenderer', {
            'width': width,
            'height': height
        });

        return textureID;
    }

    /// Update the size of the Texture renderer
    Future<void> updateTextureRendererSize(int textureID, int width, int height) async {
        return await ZegoExpressImpl.methodChannel.invokeMethod('updateTextureRendererSize', {
            'textureID': textureID,
            'width': width,
            'height': height
        });
    }

    /// Destroys the Texture renderer and releases its resources
    Future<void> destroyTextureRenderer(int textureID) async {
        return await ZegoExpressImpl.methodChannel.invokeMethod('destroyTextureRenderer', {
            'textureID': textureID
        });
    }
}