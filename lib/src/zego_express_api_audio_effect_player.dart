import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineAudioEffectPlayer on ZegoExpressEngine {
  /// Creates a audio effect player instance.
  ///
  /// Available since: 1.16.0
  /// Description: Creates a audio effect player instance.
  /// Use cases: When you need to play short sound effects, such as applause, cheers, etc., you can use audioEffectPlayer to achieve.
  /// When to call: It can be called after [createEngine].
  /// Restrictions: Currently, a maximum of 1 instances can be created, after which it will return null.
  /// Related APIs: [destroyAudioEffectPlayer].
  ///
  /// - Returns audio effect player instance, null will be returned when the maximum number is exceeded.
  Future<ZegoAudioEffectPlayer?> createAudioEffectPlayer() async {
    return await ZegoExpressImpl.instance.createAudioEffectPlayer();
  }

  /// Destroys a audio effect player instance.
  ///
  /// Available since: 1.16.0
  /// Description: Destroys the specified audio effect player instance.
  /// When to call: It can be called after [createAudioEffectPlayer].
  /// Restrictions: None.
  /// Related APIs: [createAudioEffectPlayer].
  ///
  /// - [audioEffectPlayer] The audio effect player instance object to be destroyed.
  Future<void> destroyAudioEffectPlayer(
      ZegoAudioEffectPlayer audioEffectPlayer) async {
    return await ZegoExpressImpl.instance
        .destroyAudioEffectPlayer(audioEffectPlayer);
  }
}
