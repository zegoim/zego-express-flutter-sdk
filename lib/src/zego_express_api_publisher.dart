
import 'dart:typed_data';
import 'package:flutter/services.dart';
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePublisher on ZegoExpressEngine {

  /// Starts publishing a stream. Support multi-room mode.
  ///
  /// This function allows users to publish their local audio and video streams to the ZEGO RTC server. Other users in the same room can use the streamID to play the audio and video streams for intercommunication.
  /// Before you start to publish the stream, you need to join the room first by calling [loginRoom]. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
  /// In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
  /// After the first publish stream failure due to network reasons or the publish stream is interrupted, the default time for SDK reconnection is 20min.
  /// To call [SetRoomMode] function to select multiple rooms, the room ID must be specified explicitly.
  ///
  /// - [streamID] Stream ID, a string of up to 256 characters, needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
  /// - [config] Advanced publish configuration
  /// - [channel] Publish stream channel
  Future<void> startPublishingStream(String streamID, {ZegoPublisherConfig? config, ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.startPublishingStream(streamID, config: config, channel: channel);
  }

  /// Stops publishing a stream (for the specified channel).
  ///
  /// This function allows the user to stop sending local audio and video streams and end the call.
  /// If the user has initiated publish flow, this function must be called to stop the publish of the current stream before publishing the new stream (new streamID), otherwise the new stream publish will return a failure.
  /// After stopping streaming, the developer should stop the local preview based on whether the business situation requires it.
  /// Use this function to stop publishing stream of aux channel.
  ///
  /// - [channel] Publish stream channel
  Future<void> stopPublishingStream({ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.stopPublishingStream(channel: channel);
  }

  /// Sets the extra information of the stream being published for the specified publish channel.
  ///
  /// Available since: 1.1.0
  /// Description: Use this function to set the extra info of the stream. The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID. Developers can synchronize variable content related to stream IDs based on stream additional information.
  /// When to call: After the engine is created [createEngine], Called before and after [startPublishingStream] can both take effect.
  /// Restrictions: None.
  /// Related callbacks: Users can obtain the execution result of the function through [ZegoPublisherSetStreamExtraInfoResult] callback.
  ///
  /// - [extraInfo] Stream extra information, a string of up to 1024 characters.
  /// - [channel] Publish stream channel.
  /// - Returns Set stream extra information execution result notification.
  Future<ZegoPublisherSetStreamExtraInfoResult> setStreamExtraInfo(String extraInfo, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setStreamExtraInfo(extraInfo, channel: channel);
  }

  /// Starts/Updates the local video preview (for the specified channel).
  ///
  /// The user can see his own local image by calling this function. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, SDK internally actively stops previewing by default.
  /// Local view and preview modes can be updated by calling this function again.
  /// You can set the mirror mode of the preview by calling the [setVideoMirrorMode] function. The default preview setting is image mirrored.
  /// When this function is called, the audio and video engine module inside SDK will start really, and it will start to try to collect audio and video. In addition to calling this function normally to preview the local screen, developers can also pass [null] to the canvas parameter, in conjunction with ZegoExpressEngine's sound wave function, in order to achieve the purpose of detecting whether the audio equipment is working properly before logging in to the room.
  ///
  /// - [canvas] The view used to display the preview image. If the view is set to null, no preview will be made.
  /// - [channel] Publish stream channel
  Future<void> startPreview({ZegoCanvas? canvas, ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.startPreview(canvas: canvas, channel: channel);
  }

  /// Stops the local video preview (for the specified channel).
  ///
  /// This function can be called to stop previewing when there is no need to see the preview locally.
  ///
  /// - [channel] Publish stream channel
  Future<void> stopPreview({ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.stopPreview(channel: channel);
  }

  /// Sets up the video configurations (for the specified channel).
  ///
  /// This function can be used to set the video frame rate, bit rate, video capture resolution, and video encoding output resolution. If you do not call this function, the default resolution is 360p, the bit rate is 600 kbps, and the frame rate is 15 fps.
  /// It is necessary to set the relevant video configuration before publishing the stream, and only support the modification of the encoding resolution and the bit rate after publishing the stream.
  /// Developers should note that the wide and high resolution of the mobile end is opposite to the wide and high resolution of the PC. For example, in the case of 360p, the resolution of the mobile end is 360x640, and the resolution of the PC end is 640x360.
  ///
  /// - [config] Video configuration, the SDK provides a common setting combination of resolution, frame rate and bit rate, they also can be customized.
  /// - [channel] Publish stream channel
  Future<void> setVideoConfig(ZegoVideoConfig config, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setVideoConfig(config, channel: channel);
  }

  /// Gets the current video configurations (for the specified channel).
  ///
  /// This function can be used to get the specified publish channel's current video frame rate, bit rate, video capture resolution, and video encoding output resolution.
  ///
  /// - [channel] Publish stream channel
  /// - Returns Video configuration object
  Future<ZegoVideoConfig> getVideoConfig({ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.getVideoConfig(channel: channel);
  }

  /// Sets the video mirroring mode (for the specified channel).
  ///
  /// This function can be called to set whether the local preview video and the published video have mirror mode enabled.
  ///
  /// - [mirrorMode] Mirror mode for previewing or publishing the stream
  /// - [channel] Publish stream channel
  Future<void> setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setVideoMirrorMode(mirrorMode, channel: channel);
  }

  /// Sets the video orientation (for the specified channel).
  ///
  /// This function sets the orientation of the video. The captured image is rotated according to the value of the parameter [DeviceOrientation] compared to the forward direction of the phone. After rotation, it will be automatically adjusted to adapt the encoded image resolution.
  ///
  /// - [orientation] Video orientation
  /// - [channel] Publish stream channel
  Future<void> setAppOrientation(DeviceOrientation orientation, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setAppOrientation(orientation, channel: channel);
  }

  /// Sets up the audio configurations for the specified publish channel.
  ///
  /// Available since: 1.3.4
  /// Description: You can set the combined value of the audio codec, bit rate, and audio channel through this function. If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
  /// Default value: The default audio config refers to the default value of [ZegoAudioConfig]. 
  /// When to call: After the engine is created [createEngine], and before publishing [startPublishingStream].
  /// Restrictions: None.
  /// Related APIs: [getAudioConfig].
  ///
  /// - [config] Audio config.
  /// - [channel] Publish stream channel.
  Future<void> setAudioConfig(ZegoAudioConfig config, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setAudioConfig(config, channel: channel);
  }

  /// Gets the current audio configurations.
  ///
  /// Available since: 1.8.0
  /// Description: You can get the current audio codec, bit rate, and audio channel through this function.
  /// When to call: After the engine is created [createEngine].
  /// Restrictions: None.
  /// Caution: Act on the main publish channel ZegoPublishChannel.Main.
  /// Related APIs: [setAudioConfig].
  ///
  /// - Returns Audio config.
  Future<ZegoAudioConfig> getAudioConfig() async {
    return await ZegoExpressImpl.instance.getAudioConfig();
  }

  /// Set encryption key for the publishing stream for the specified publish channel.
  ///
  /// Available since: 1.19.0
  /// Description: Support calling this function to update the encryption key while publishing stream.
  /// When to call: After the engine is created [createEngine], Called before and after [startPublishingStream] can both take effect.
  /// Restrictions: This function is only valid when publishing stream to the Zego RTC server.
  /// Caution: Note that developers need to update the player's decryption key before updating the publisher's encryption key. 
  /// Related APIs: Calling [stopPublishingStream] or [logoutRoom] will clear the encryption key.
  ///
  /// - [key] The encryption key, note that the key length only supports 16/24/32 bytes.
  /// - [channel] Publish stream channel.
  Future<void> setPublishStreamEncryptionKey(String key, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setPublishStreamEncryptionKey(key, channel: channel);
  }

  /// Take a snapshot of the publishing stream for the specified publish channel.
  ///
  /// Available since: 1.17.0
  /// Description: Take a snapshot of the publishing stream.
  /// When to call: Called this function after calling [startPublishingStream] or [startPreview].
  /// Restrictions: None.
  /// Caution: The resolution of the snapshot is the encoding resolution set in [setVideoConfig]. If you need to change it to capture resolution, please call [setCapturePipelineScaleMode] to change the capture pipeline scale mode to [Post].
  /// Related callbacks: The screenshot result will be called back through [ZegoPublisherTakeSnapshotCallback].
  /// Related APIs: [takePlayStreamSnapshot].
  ///
  /// - [channel] Publish stream channel.
  /// - Returns Results of take publish stream snapshot.
  Future<ZegoPublisherTakeSnapshotResult> takePublishStreamSnapshot({ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.takePublishStreamSnapshot(channel: channel);
  }

  /// Stops or resumes sending the audio part of a stream for the specified channel.
  ///
  /// Available since: 1.1.0
  /// Description: This function can be called when publishing the stream to realize not publishing the audio data stream. The SDK still collects and processes the audio, but does not send the audio data to the network.
  /// When to call: Called after the engine is created [createEngine] can take effect.
  /// Restrictions: None.
  /// Related callbacks: If you stop sending audio streams, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteMicStateUpdate] callbacks.
  /// Related APIs: [mutePublishStreamVideo].
  ///
  /// - [mute] Whether to stop sending audio streams, true means not to send audio stream, and false means sending audio stream. The default is false.
  /// - [channel] Publish stream channel.
  Future<void> mutePublishStreamAudio(bool mute, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.mutePublishStreamAudio(mute, channel: channel);
  }

  /// Stops or resumes sending the video part of a stream for the specified channel.
  ///
  /// Available since: 1.1.0
  /// Description: This function can be called when publishing the stream to realize not publishing the video stream. The local camera can still work normally, can capture, preview and process video images normally, but does not send the video data to the network.
  /// When to call: Called after the engine is created [createEngine] can take effect.
  /// Restrictions: None.
  /// Related callbacks: If you stop sending video streams locally, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteCameraStateUpdate] callbacks.
  /// Related APIs: [mutePublishStreamAudio].
  ///
  /// - [mute] Whether to stop sending video streams, true means not to send video stream, and false means sending video stream. The default is false.
  /// - [channel] Publish stream channel.
  Future<void> mutePublishStreamVideo(bool mute, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.mutePublishStreamVideo(mute, channel: channel);
  }

  /// Enable or disable the stream mixing precision alignment function.
  ///
  /// Available since: 2.11.0.
  /// Description: Use this interface to enable stream alignment, the SDK will attach network time information to the stream when publishing it for accurate stream alignment.
  /// Use case: Generally used in scenarios such as KTV where stream mixing alignment is required.
  /// When to call: After the engine is created [createEngine].
  /// Caution: If mixed flow need time for alignment, the flow through the network push flow need to call startPublishingStream and ZegoPublisherConfig.forceSynchronousNetworkTime = 1, for open network time synchronization.
  /// Related APIs: [startMixerTask], [startAutoMixerTask]
  ///
  /// - [alignment] Whether to enable the stream mixing precision alignment function.
  /// - [channel] Publish stream channel
  Future<void> setStreamAlignmentProperty(int alignment, ZegoPublishChannel channel) async {
    return await ZegoExpressImpl.instance.setStreamAlignmentProperty(alignment, channel);
  }

  /// Enables or disables traffic control.
  ///
  /// Available since: 1.5.0
  /// Description: Enabling traffic control allows the SDK to adjust the audio and video streaming bitrate according to the current upstream network environment conditions, or according to the counterpart's downstream network environment conditions in a one-to-one interactive scenario, to ensure smooth results. At the same time, you can further specify the attributes of traffic control to adjust the corresponding control strategy.
  /// Default value: Enable.
  /// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
  /// Restrictions: Only support RTC publish.
  /// Caution: Act on the main publish channel ZegoPublishChannel.Main.
  ///
  /// - [enable] Whether to enable traffic control. The default is ture.
  /// - [property] Adjustable property of traffic control, bitmask format. Should be one or the combinations of [ZegoTrafficControlProperty] enumeration. [AdaptiveFPS] as default.
  Future<void> enableTrafficControl(bool enable, int property) async {
    return await ZegoExpressImpl.instance.enableTrafficControl(enable, property);
  }

  /// Sets the minimum video bitrate for traffic control for the specified publish channel.
  ///
  /// Available since: 1.1.0
  /// Description: Set the control strategy when the video bitrate reaches the lowest threshold during flow control. When the bitrate is lower than the minimum threshold, you can choose not to send video data or send it at a very low frame bitrate.
  /// Default value: There is no control effect of the lowest threshold of video bitrate.
  /// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
  /// Restrictions: The traffic control must be turned on [enableTrafficControl].
  /// Related APIs: [enableTrafficControl].
  ///
  /// - [bitrate] Minimum video bitrate (kbps).
  /// - [mode] Video sending mode below the minimum bitrate.
  /// - [channel] Publish stream channel.
  Future<void> setMinVideoBitrateForTrafficControl(int bitrate, ZegoTrafficControlMinVideoBitrateMode mode, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setMinVideoBitrateForTrafficControl(bitrate, mode, channel: channel);
  }

  /// Set the factors of concern that trigger traffic control for the specified publish channel.
  ///
  /// Available since: 2.4.0
  /// Description: Use this interface to control whether to start traffic control due to poor remote network conditions.
  /// Default value: Default is disable.
  /// When to call: After the engine is created [createEngine], Called before [startPublishingStream] can take effect.
  /// Restrictions: The traffic control must be turned on [enableTrafficControl].
  /// Related APIs: [enableTrafficControl.
  ///
  /// - [mode] When LOCAL_ONLY is selected, only the local network status is concerned. When choosing REMOTE, also take into account the remote network.
  /// - [channel] Publish stream channel.
  Future<void> setTrafficControlFocusOn(ZegoTrafficControlFocusOnMode mode, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setTrafficControlFocusOn(mode, channel: channel);
  }

  /// Sets the audio recording volume for stream publishing.
  ///
  /// Available since: 1.13.0
  /// Description: This function is used to perform gain processing based on the device's collected volume. The local user can control the sound level of the audio stream sent to the remote end.
  /// Default value: Default is 100.
  /// When to call: After creating the engine [createEngine], before starting to push the stream [startPublishingStream].
  /// Restrictions: None.
  /// Related APIs: Set the playing stream volume [setPlayVolume].
  ///
  /// - [volume] The volume gain percentage, the range is 0 ~ 200, and the default value is 100, which means 100% of the original collection volume of the device.
  Future<void> setCaptureVolume(int volume) async {
    return await ZegoExpressImpl.instance.setCaptureVolume(volume);
  }

  /// Set audio capture stereo mode.
  ///
  /// Available since: 1.15.0
  /// Description: This function is used to set the audio capture channel mode. When the developer turns on the two-channel capture, using a special two-channel capture device, the two-channel audio data can be collected and streamed.
  /// Use cases: In some professional scenes, users are particularly sensitive to sound effects, such as voice radio and musical instrument performance. At this time, support for dual-channel and high-quality sound is required.
  /// Default value: The default is None, which means mono capture.
  /// When to call: It needs to be called after [createEngine]， before [startPublishingStream], [startPlayingStream], [startPreview], [createMediaPlayer] and [createAudioEffectPlayer].
  /// Restrictions: None.
  /// Related APIs: When streaming, you need to enable the dual-channel audio encoding function through the [setAudioConfig] interface at the same time.
  ///
  /// - [mode] Audio stereo capture mode.
  Future<void> setAudioCaptureStereoMode(ZegoAudioCaptureStereoMode mode) async {
    return await ZegoExpressImpl.instance.setAudioCaptureStereoMode(mode);
  }

  /// Adds a target CDN URL to which the stream will be relayed from ZEGO RTC server.
  ///
  /// Available since: 1.1.0
  /// Description: Forward audio and video streams from ZEGO RTC servers to custom CDN content distribution networks with high latency but support for high concurrent pull streams.
  /// Use cases: 1. It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay. 2. Since ZEGO RTC server itself can be configured to support CDN(content distribution networks), this function is mainly used by developers who have CDN content distribution services themselves. 3. This function supports dynamic relay to the CDN content distribution network, so developers can use this function as a disaster recovery solution for CDN content distribution services.
  /// When to call: After calling the [createEngine] function to create the engine.
  /// Restrictions: When the [enablePublishDirectToCDN] function is set to true to publish the stream straight to the CDN, then calling this function will have no effect.
  /// Related APIs: Remove URLs that are re-pushed to the CDN [removePublishCdnUrl].
  ///
  /// - [streamID] Stream ID.
  /// - [targetURL] CDN relay address, supported address format is rtmp.
  /// - Returns The execution result of update the relay CDN operation.
  Future<ZegoPublisherUpdateCdnUrlResult> addPublishCdnUrl(String streamID, String targetURL) async {
    return await ZegoExpressImpl.instance.addPublishCdnUrl(streamID, targetURL);
  }

  /// Deletes the specified CDN URL, which is used for relaying streams from ZEGO RTC server to CDN.
  ///
  /// Available since: 1.1.0
  /// Description: When a CDN forwarding address has been added via [addPublishCdnUrl], this function is called when the stream needs to be stopped.
  /// When to call: After calling the [createEngine] function to create the engine.
  /// Restrictions: When the [enablePublishDirectToCDN] function is set to true to publish the stream straight to the CDN, then calling this function will have no effect.
  /// Caution: This function does not stop publishing audio and video stream to the ZEGO ZEGO RTC server.
  /// Related APIs: Add URLs that are re-pushed to the CDN [addPublishCdnUrl].
  ///
  /// - [streamID] Stream ID.
  /// - [targetURL] CDN relay address, supported address format rtmp.
  /// - Returns The execution result of update the relay CDN operation.
  Future<ZegoPublisherUpdateCdnUrlResult> removePublishCdnUrl(String streamID, String targetURL) async {
    return await ZegoExpressImpl.instance.removePublishCdnUrl(streamID, targetURL);
  }

  /// Whether to directly push to CDN (without going through the ZEGO RTC server), for the specified channel.
  ///
  /// Available since: 1.5.0
  /// Description: Whether to publish streams directly from the client to CDN without passing through Zego RTC server.
  /// Use cases: It is often used in large-scale live broadcast scenes that do not have particularly high requirements for delay.
  /// Default value: The default is false, and direct push is not enabled.
  /// When to call: After creating the engine [createEngine], before starting to push the stream [startPublishingStream].
  /// Caution: The Direct Push CDN feature does not pass through the ZEGO Real-Time Audio and Video Cloud during network transmission, so you cannot use ZEGO's ultra-low latency audio and video services.
  /// Related APIs: Dynamic re-push to CDN function [addPublishCdnUrl], [removePublishCdnUrl].
  ///
  /// - [enable] Whether to enable direct publish CDN, true: enable direct publish CDN, false: disable direct publish CDN.
  /// - [config] CDN configuration, if null, use Zego's background default configuration.
  /// - [channel] Publish stream channel.
  Future<void> enablePublishDirectToCDN(bool enable, {ZegoCDNConfig? config, ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.enablePublishDirectToCDN(enable, config: config, channel: channel);
  }

  /// Sets up the stream watermark before stream publishing (for the specified channel).
  ///
  /// Available since: 1.1.0
  /// Description: Set watermark for publish stream screen.
  /// Use cases: It is often used to identify the source of the publish.
  /// When to call: After creating the engine through [createEngine] function.
  /// Caution: The layout of the watermark cannot exceed the video encoding resolution of the stream. It can be set at any time before or during the publishing stream.
  ///
  /// - [watermark] The upper left corner of the watermark layout is the origin of the coordinate system, and the area cannot exceed the size set by the encoding resolution. If it is null, the watermark is cancelled. It supports Flutter assets resources, just set the 'imageURL' parameter of the watermark object to the prefix of 'flutter-asset://' and append the relative path of the resource file declared in 'pubspec.yaml'. For example, assuming that 'assets: - images/logo.png' is declared in 'pubspec.yaml', then set 'imageURL' parameter to 'flutter-asset://images/logo.png'.
  /// - [isPreviewVisible] the watermark is visible on local preview
  /// - [channel] Publish stream channel.
  Future<void> setPublishWatermark({ZegoWatermark? watermark, bool? isPreviewVisible, ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.setPublishWatermark(watermark: watermark, isPreviewVisible: isPreviewVisible, channel: channel);
  }

  /// Set the Supplemental Enhancement Information type.
  ///
  /// Available since: 1.18.0
  /// Description: By default, the SDK wraps the data with ZEGO's self-defined SEI type, which is not specified by the SEI standard. When the developer needs to use a third-party decoder to decode the SEI, the correct SEI will not be decoded and the [setSEIConfig] interface needs to be called to change the type of the SEI sent by the SDK to UserUnregister type.
  /// Use cases: This function needs to be executed when the developer uses a third-party decoder to decode the SEI.
  /// When to call: After creating the engine [createEngine], before starting to push the stream [startPublishingStream].
  /// Restrictions: None.
  ///
  /// - [config] SEI configuration. The SEI defined by ZEGO is used by default.
  Future<void> setSEIConfig(ZegoSEIConfig config) async {
    return await ZegoExpressImpl.instance.setSEIConfig(config);
  }

  /// Sends Supplemental Enhancement Information.
  ///
  /// Available since: 1.1.0
  /// Description: While pushing the stream to transmit the audio and video stream data, the stream media enhancement supplementary information is sent to synchronize some other additional information.
  /// Use cases: Generally used in scenes such as synchronizing music lyrics or precise video layout, you can choose to send SEI.
  /// When to call: After starting to push the stream [startPublishingStream].
  /// Restrictions: Do not exceed 30 times per second, and the SEI data length is limited to 4096 bytes.
  /// Caution: Since the SEI information follows the video frame, there may be frame loss due to network problems, so the SEI information may also be lost. In order to solve this situation, it should be sent several times within the restricted frequency.
  /// Related APIs: After the pusher sends the SEI, the puller can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
  ///
  /// - [data] SEI data.
  /// - [dataLength] SEI data length.
  /// - [channel] Publish stream channel.
  Future<void> sendSEI(Uint8List data, int dataLength, {ZegoPublishChannel? channel}) async {
    return await ZegoExpressImpl.instance.sendSEI(data, dataLength, channel: channel);
  }

  /// Enables or disables hardware encoding.
  ///
  /// Available since: 1.1.0
  /// Description: Whether to use the hardware encoding function when publishing the stream, the GPU is used to encode the stream and to reduce the CPU usage.
  /// When to call: The setting can take effect before the stream published. If it is set after the stream published, the stream should be stopped first before it takes effect.
  /// Caution: Because hard-coded support is not particularly good for a few models, SDK uses software encoding by default. If the developer finds that the device is hot when publishing a high-resolution audio and video stream during testing of some models, you can consider calling this function to enable hard coding.
  ///
  /// - [enable] Whether to enable hardware encoding, true: enable hardware encoding, false: disable hardware encoding.
  Future<void> enableHardwareEncoder(bool enable) async {
    return await ZegoExpressImpl.instance.enableHardwareEncoder(enable);
  }

  /// Sets the timing of video scaling in the video capture workflow. You can choose to do video scaling right after video capture (the default value) or before encoding.
  ///
  /// Available since: 1.1.0
  /// When to call: This function needs to be set before call [startPreview] or [startPublishingStream].
  /// Caution: The main effect is Whether the local preview is affected when the acquisition resolution is different from the encoding resolution.
  ///
  /// - [mode] The capture scale timing mode.
  Future<void> setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) async {
    return await ZegoExpressImpl.instance.setCapturePipelineScaleMode(mode);
  }

  /// Whether to enable H.265 encoding to automatically downgrade to H.264 encoding.
  ///
  /// Available since: 2.12.0
  /// Description: When using H.265 encoding to push the stream, whether to enable the strategy of automatically degrading H.265 encoding to H.264 encoding under abnormal circumstances.After enabling automatic downgrade, when H.265 encoding is not supported or H.265 encoding fails, the SDK will try to downgrade and use H.264 encoding to push the stream.After turning off automatic downgrade, when H.265 encoding is not supported or H.265 encoding fails, the direct streaming fails.
  /// Use cases: In the Co-hosting and Showroom Live Streaming scenarios, use H265 encoding to push the stream to save CDN traffic without degrading the picture quality.
  /// Default Value: When this interface is not called, the default is yes, which means that H.265 encoding is turned on and automatically downgraded to H.264 encoding.
  /// When to call: After creating the engine, call the [startPublishingStream] function before pushing the stream.
  /// Related callbacks: When the H.265 encoding is automatically downgraded to the H.264 encoding strategy, the [onPublisherVideoEncoderChanged] callback will be triggered when the encoding method changes.
  /// Caution: When downgrading from H.265 to H.264 encoding occurs during the streaming process, if you are recording local video or cloud recording, multiple recording files will be generated, which needs to be dealt with.
  ///
  /// - [enable] Whether to enable H.265 coding automatically fallback to H.264 coding, true: enable, false: disable, and the default value is true
  Future<void> enableH265EncodeFallback(bool enable) async {
    return await ZegoExpressImpl.instance.enableH265EncodeFallback(enable);
  }

  /// Whether the specified video encoding type is supported.
  ///
  /// Available since: 2.12.0 and above
  /// Description: Whether the specified video encoding is supported depends on the following aspects, whether the hardware model supports hard encoding, whether the performance of the hardware model supports soft encoding, and whether the SDK has the encoding module.
  /// When to call: After creating the engine.
  /// Caution: It is recommended that users call this interface to obtain H.265 encoding support capability before publish stream with H.265 encoding, if not supported, you can use other encodings for publish, such as H.264.
  ///
  /// - [codecID] Video codec id. Required: Yes.
  /// - Returns Whether the specified video encoding is supported.Value range: true means support, you can use this encoding format for publish; false means the is not supported, and the encoding format cannot be used for publish.
  Future<bool> isVideoEncoderSupported(ZegoVideoCodecID codecID) async {
    return await ZegoExpressImpl.instance.isVideoEncoderSupported(codecID);
  }

}
