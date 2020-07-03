
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEngineMediaPlayer on ZegoExpressEngine {

  /// Creates a media player instance.
  ///
  /// Currently, a maximum of 4 instances can be created, after which it will return null. The more instances of a media player, the greater the performance overhead on the device.
  ///
  /// - Returns Media player instance, null will be returned when the maximum number is exceeded.
  Future<ZegoMediaPlayer> createMediaPlayer() async {
    return await ZegoExpressImpl.instance.createMediaPlayer();
  }

  /// Destroys a media player instance.
  ///
  /// - [mediaPlayer] The media player instance object to be destroyed
  Future<void> destroyMediaPlayer(ZegoMediaPlayer mediaPlayer) async {
    return await ZegoExpressImpl.instance.destroyMediaPlayer(mediaPlayer);
  }

}
