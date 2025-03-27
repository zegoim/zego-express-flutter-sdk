import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineScreenCapture on ZegoExpressEngine {
  /// Get a list of screens or windows in a screen.
  ///
  /// Available since: 3.1.0
  /// Description: Get a list of screens or windows in a screen.
  /// Restrictions: Only support in Windows/macOS.
  ///
  /// - [thumbnailWidth] Get the thumbnail width corresponding to the window, the thumbnail can be used to draw on the window selection interface. (unit is pixel)
  /// - [thumbnailHeight] Get the thumbnail height corresponding to the window, the thumbnail can be used to draw on the window selection interface. (unit is pixel)
  /// - [iconWidth] Get the width of the icon corresponding to the program. (unit is pixel)
  /// - [iconHeight] Get the height of the icon corresponding to the program. (unit is pixel)
  /// - Returns List of capture source info objects.
  Future<List<ZegoScreenCaptureSourceInfo>> getScreenCaptureSources(
      int thumbnailWidth,
      int thumbnailHeight,
      int iconWidth,
      int iconHeight) async {
    return await ZegoExpressImpl.instance.getScreenCaptureSources(
        thumbnailWidth, thumbnailHeight, iconWidth, iconHeight);
  }

  /// Create the screen capture source
  ///
  /// Available: since 3.1.0
  /// Description: Creates a screen capture source object based on the provided source ID and source type.
  /// Use cases: It is used when you need to record and share the screen or window.
  /// When to call: It needs to be called after [createEngine].
  /// Platform differences: Only supports Windows and macOS.
  ///
  /// - [sourceId] The specified screen ID or window ID. Only macOS and windows platforms need to pass in this parameter.
  /// - [sourceType] The specified screen source type. Only macOS and windows platforms need to pass in this parameter.
  /// - Returns The screen capture instance, null will be returned when the maximum number is exceeded.
  Future<ZegoScreenCaptureSource?> createScreenCaptureSource(
      {int? sourceId, ZegoScreenCaptureSourceType? sourceType}) async {
    return await ZegoExpressImpl.instance
        .createScreenCaptureSource(sourceId: sourceId, sourceType: sourceType);
  }

  /// Destroy the screen capture source instance
  ///
  /// Available: since 3.1.0
  /// Description: Destroy the [ZegoScreenCaptureSource] instance object.
  /// Use cases: When you no longer need to use the screen capture function, you can use this function to destroy the instance object created by the [createScreenCaptureSource] function
  /// When to call: When you need to the screen capture source object needs to be destroyed
  /// Restrictions: After destroy the instance, you need to release the [ZegoScreenCaptureSource] instance object you hold by yourself, and don’t call the function of this instance object after the destruction.
  /// Platform differences: Only supports Windows and macOS.
  ///
  /// - [source] The screen capture source instance to be destroyed.
  Future<void> destroyScreenCaptureSource(
      ZegoScreenCaptureSource source) async {
    return await ZegoExpressImpl.instance.destroyScreenCaptureSource(source);
  }
}
