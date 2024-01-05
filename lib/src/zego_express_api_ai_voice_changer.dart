import 'zego_express_api.dart';
import 'impl/zego_express_impl.dart';
import 'zego_express_defines.dart';

// ignore_for_file: deprecated_member_use_from_same_package

extension ZegoExpressEngineAIVoiceChanger on ZegoExpressEngine {
  /// Creates an AI voice changer instance.
  ///
  /// Available since: 3.10.0
  /// Description: Creates an AI voice changer instance.
  /// Use case: Often used in live broadcasting, voice chatroom and KTV.
  /// When to call: It can be called after the SDK by [createEngine] has been initialized.
  /// Restrictions: Currently, a maximum of 1 instance can be created, after which it will return null.
  /// Related APIs: User can call [destroyAIVoiceChanger] function to destroy an AI voice changer instance.
  ///
  /// - Returns AI voice changer instance.
  Future<ZegoAIVoiceChanger?> createAIVoiceChanger() async {
    return await ZegoExpressImpl.instance.createAIVoiceChanger();
  }

  /// Destroys an AI voice changer instance.
  ///
  /// Available since: 3.10.0
  /// Description: Destroys the AI voice changer instance.
  /// Related APIs: User can call [createAIVoiceChanger] function to create an AI voice changer instance.
  ///
  /// - [aiVoiceChanger] The AI voice changer instance object to be destroyed.
  Future<void> destroyAIVoiceChanger(ZegoAIVoiceChanger aiVoiceChanger) async {
    return await ZegoExpressImpl.instance.destroyAIVoiceChanger(aiVoiceChanger);
  }
}
