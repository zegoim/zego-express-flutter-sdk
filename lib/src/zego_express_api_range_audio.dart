import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineRangeAudio on ZegoExpressEngine {
  /// Creates a range audio instance.
  ///
  /// Available since: 2.11.0
  /// Description: Creates a range audio instance.
  /// Use case: Often used in game voice scenarios, users can use related functions by creating range audio instance objects.
  /// When to call: It can be called after the engine by [createEngine] has been initialized.
  /// Restrictions: Currently, a maximum of 1 instances can be created, after which it will return null.
  /// Impacts on other APIs: If you use the range audio module, you cannot use the basic push-pull stream [startPublishingStream], [startPlayingStream] interfaces and related callbacks.
  ///
  /// - Returns range audio instance, null will be returned when the maximum number is exceeded.
  Future<ZegoRangeAudio?> createRangeAudio() async {
    return await ZegoExpressImpl.instance.createRangeAudio();
  }

  /// Destroys a range audio instance.
  ///
  /// Available since: 2.11.0
  /// Description: Destroys a range audio instance.
  ///
  /// - [rangeAudio] The range audio instance object to be destroyed.
  Future<void> destroyRangeAudio(ZegoRangeAudio rangeAudio) async {
    return await ZegoExpressImpl.instance.destroyRangeAudio(rangeAudio);
  }
}
