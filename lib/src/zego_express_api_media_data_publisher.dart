import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineMediaDataPublisher on ZegoExpressEngine {
  /// Creates a media data publisher instance.
  ///
  /// Available since: 3.4.0
  /// Description: Developers can use this interface to create ZegoMediaDataPublisher objects to push local media resource files to implement server-side streaming scenarios, such as AI classrooms.
  /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
  /// When to call: After calling the [createEngine] function to create an engine.
  /// Caution: It returns the instance object of the specified publish channel. It will return the instance object if it already exists.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [config] Config the media data publisher.
  /// - Returns Media data publisher instance.
  Future<ZegoMediaDataPublisher?> createMediaDataPublisher(
      ZegoMediaDataPublisherConfig config) async {
    return await ZegoExpressImpl.instance.createMediaDataPublisher(config);
  }

  /// Destroys a media data publisher instance.
  ///
  /// Available since: 2.17.0
  /// Description: Destroys a media data publisher instance.
  /// Use cases: Often used in server-side publishing stream scenarios, such as AI classrooms.
  /// When to call: After calling the [createEngine] function to create an engine.
  /// Note: This function is only available in ZegoExpressVideo SDK!
  ///
  /// - [mediaDataPublisher] The media data publisher instance object to be destroyed
  Future<void> destroyMediaDataPublisher(
      ZegoMediaDataPublisher mediaDataPublisher) async {
    return await ZegoExpressImpl.instance
        .destroyMediaDataPublisher(mediaDataPublisher);
  }
}
