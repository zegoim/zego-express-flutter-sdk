import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEnginePictureCapturer on ZegoExpressEngine {
  /// Create picture capturer instance.
  ///
  /// Available since: 3.22.0
  /// Description: Creates a picture capturer instance.
  /// Use case: Often used in pushing static images.
  /// When to call: It can be called after the SDK by [createEngine] has been initialized.
  /// Restrictions: None.
  /// Related APIs: User can call [destroyPictureCapturer] function to destroy a picture capturer instance. Use [setVideoSource] to set the picture capturer as the push stream video source.
  ///
  /// - Returns Picture capturer instance.
  Future<ZegoPictureCapturer?> createPictureCapturer() async {
    return await ZegoExpressImpl.instance.createPictureCapturer();
  }

  /// Destroys a picture capturer instance.
  ///
  /// Available since: 3.22.0
  /// Description: Destroys the picture capturer instance.
  /// Related APIs: User can call [createPictureCapturer] function to create a picture capturer instance.
  ///
  /// - [pictureCapturer] The picture capturer instance to be destroyed.
  Future<void> destroyPictureCapturer(
      ZegoPictureCapturer pictureCapturer) async {
    return await ZegoExpressImpl.instance
        .destroyPictureCapturer(pictureCapturer);
  }
}
