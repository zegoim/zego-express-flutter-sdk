import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineCopyrightedMusic on ZegoExpressEngine {
  /// Creates a copyrighted music instance.
  ///
  /// Available since: 2.13.0
  /// Description: Creates a copyrighted music instance.
  /// Use case: Often used in online KTV chorus scenarios, users can use related functions by creating copyrighted music instance objects.
  /// When to call: It can be called after the engine by [createEngine] has been initialized.
  /// Restrictions: The SDK only supports the creation of one instance of CopyrightedMusic. Multiple calls to this function return the same object.
  ///
  /// - Returns copyrighted music instance, multiple calls to this function return the same object.
  Future<ZegoCopyrightedMusic?> createCopyrightedMusic() async {
    return await ZegoExpressImpl.instance.createCopyrightedMusic();
  }

  /// Destroys a copyrighted music instance.
  ///
  /// Available since: 2.13.0
  /// Description: Destroys a copyrighted music instance.
  /// When to call: It can be called before the engine by [destroyEngine]
  ///
  /// - [copyrightedMusic] The copyrighted music instance object to be destroyed.
  Future<void> destroyCopyrightedMusic(
      ZegoCopyrightedMusic copyrightedMusic) async {
    return await ZegoExpressImpl.instance
        .destroyCopyrightedMusic(copyrightedMusic);
  }
}
