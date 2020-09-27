
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineAudioEffectPlayer on ZegoExpressEngine {

  /// Creates a audio effect player instance.
  ///
  /// Currently, a maximum of 1 instances can be created, after which it will return null.
  ///
  /// - Returns audio effect player instance, null will be returned when the maximum number is exceeded.
  Future<ZegoAudioEffectPlayer> createAudioEffectPlayer() async {
    return await ZegoExpressImpl.instance.createAudioEffectPlayer();
  }

  /// Destroys a audio effect player instance.
  ///
  /// - [audioEffectPlayer] The audio effect player instance object to be destroyed
  Future<void> destroyAudioEffectPlayer(ZegoAudioEffectPlayer audioEffectPlayer) async {
    return await ZegoExpressImpl.instance.destroyAudioEffectPlayer(audioEffectPlayer);
  }

}
