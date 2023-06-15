import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineMediaPlayer on ZegoExpressEngine {
  /// Creates a media player instance.
  ///
  /// Available since: 2.1.0
  /// Description: Creates a media player instance.
  /// Use case: It is often used to play media resource scenes, For example, play video files, push the video of media resources in combination with custom video acquisition, and the remote end can pull the stream for viewing.
  /// When to call: It can be called after the SDK by [createEngine] has been initialized.
  /// Restrictions: Currently, a maximum of 4 instances can be created, after which it will return null.
  /// Caution: The more instances of a media player, the greater the performance overhead on the device.
  /// Related APIs: User can call [destroyMediaPlayer] function to destroy a media player instance.
  ///
  /// - Returns Media player instance, null will be returned when the maximum number is exceeded.
  Future<ZegoMediaPlayer?> createMediaPlayer() async {
    return await ZegoExpressImpl.instance.createMediaPlayer();
  }

  /// Destroys a media player instance.
  ///
  /// Available since: 2.1.0
  /// Description: Destroys a media player instance.
  /// Related APIs: User can call [createMediaPlayer] function to create a media player instance.
  ///
  /// - [mediaPlayer] The media player instance object to be destroyed.
  Future<void> destroyMediaPlayer(ZegoMediaPlayer mediaPlayer) async {
    return await ZegoExpressImpl.instance.destroyMediaPlayer(mediaPlayer);
  }
}
