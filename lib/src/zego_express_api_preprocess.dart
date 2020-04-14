
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePreprocess on ZegoExpressEngine {

    /// On/off echo cancellation
    ///
    /// Turning on echo cancellation, the SDK filters the collected audio data to reduce the echo component in the audio. It needs to be set before starting the publish, and the setting is invalid after the start of the publish.
    ///
    /// - [enable] Whether to enable echo cancellation, true: enable echo cancellation, false: disable echo cancellation
    Future<void> enableAEC(bool enable) async {
        return await ZegoExpressImpl.instance.enableAEC(enable);
    }

    /// Set echo cancellation mode
    ///
    /// Switch different echo cancellation modes to control the extent to which echo data is eliminated. Need to be set before starting the publish.
    ///
    /// - [mode] Echo cancellation mode
    Future<void> setAECMode(ZegoAECMode mode) async {
        return await ZegoExpressImpl.instance.setAECMode(mode);
    }

    /// On/off automatic gain control
    ///
    /// When the auto gain is turned on, the sound will be amplified, but it will affect the sound quality to some extent. Need to be set before starting the publish.
    ///
    /// - [enable] Whether to enable automatic gain control, true: enable AGC, false: disable AGC
    Future<void> enableAGC(bool enable) async {
        return await ZegoExpressImpl.instance.enableAGC(enable);
    }

    /// On/off noise suppression
    ///
    /// Turning on the noise suppression switch can reduce the noise in the audio data and make the human voice clearer. Need to be set before starting the publish.
    ///
    /// - [enable] Whether to enable noise suppression, true: enable AGC, false: disable AGC
    Future<void> enableANS(bool enable) async {
        return await ZegoExpressImpl.instance.enableANS(enable);
    }

    /// On/off beauty
    ///
    /// Identify the portraits in the video for beauty. It can be set before and after the start of the publish.
    ///
    /// - [featureBitmask] Beauty features, bitmask format, you can choose to enable several features in [ZegoBeautifyFeature] at the same time
    /// - [channel] Publishing stream channel.
    Future<void> enableBeautify(int featureBitmask, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.enableBeautify(featureBitmask, channel: channel);
    }

    /// Set beauty parameters
    ///
    /// - [option] Beauty configuration options
    /// - [channel] Publishing stream channel.
    Future<void> setBeautifyOption(ZegoBeautifyOption option, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setBeautifyOption(option, channel: channel);
    }


}
