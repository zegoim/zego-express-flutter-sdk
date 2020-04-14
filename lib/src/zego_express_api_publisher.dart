
import 'dart:typed_data';
import 'package:flutter/services.dart';
import 'zego_express_api.dart';
import 'zego_express_impl.dart';
import 'zego_express_defines.dart';

extension ZegoExpressEnginePublisher on ZegoExpressEngine {

    /// Start publishing stream, you can call this api to publish the another stream.
    ///
    /// This interface allows users to publish their local audio and video streams to the ZEGO real-time audio and video cloud. Other users in the same room can use the streamID to play the audio and video streams for intercommunication.
    /// Before you start to publish the stream, you need to join the room first by calling [loginRoom]. Other users in the same room can get the streamID by monitoring the [onRoomStreamUpdate] event callback after the local user publishing stream successfully.
    /// In the case of poor network quality, user publish may be interrupted, and the SDK will attempt to reconnect. You can learn about the current state and error information of the stream published by monitoring the [onPublisherStateUpdate] event.
    ///
    /// - [streamID] Stream ID, a string of up to 256 characters, needs to be globally unique within the entire AppID. If in the same AppID, different users publish each stream and the stream ID is the same, which will cause the user to publish the stream failure. You cannot include URL keywords, otherwise publishing stream and playing stream will fails. Only support numbers, English characters and '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '=', '-', '`', ';', '’', ',', '.', '<', '>', '/', '\'.
    /// - [channel] Publish stream channel
    Future<void> startPublishingStream(String streamID, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.startPublishingStream(streamID, channel: channel);
    }

    /// Stop publishing stream of the specified channel
    ///
    /// This interface allows the user to stop sending local audio and video streams and end the call.
    /// If the user has initiated publish flow, this interface must be called to stop the publish of the current stream before publishing the new stream (new streamID), otherwise the new stream publish will return a failure.
    /// After stopping streaming, the developer should stop the local preview based on whether the business situation requires it.
    /// Use this API to stop publishing stream of aux channel.
    ///
    /// - [channel] Publish stream channel
    Future<void> stopPublishingStream({ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.stopPublishingStream(channel: channel);
    }

    /// Set stream additional information. You can call this api to set params when publishing another streams
    ///
    /// User this interface to set the extra info of the stream, the result will be notified via the [ZegoPublisherSetStreamExtraInfoResult].
    /// The stream extra information is an extra information identifier of the stream ID. Unlike the stream ID, which cannot be modified during the publishing process, the stream extra information can be modified midway through the stream corresponding to the stream ID.
    /// Developers can synchronize variable content related to stream IDs based on stream additional information.
    ///
    /// - [extraInfo] Stream extra information, a string of up to 1024 characters.
    /// - [channel] Publish stream channel
    /// - Returns Set stream extra information execution result notification
    Future<ZegoPublisherSetStreamExtraInfoResult> setStreamExtraInfo(String extraInfo, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setStreamExtraInfo(extraInfo, channel: channel);
    }

    /// Start/Update local preview. You can call this api to set params when publishing another streams
    ///
    /// The user can see his own local image by calling this interface. The preview function does not require you to log in to the room or publish the stream first. But after exiting the room, SDK internally actively stops previewing by default.
    /// Local view and preview modes can be updated by calling this interface again.
    /// You can set the mirror mode of the preview by calling the [setVideoMirrorMode] interface. The default preview setting is image mirrored.
    /// When this api is called, the audio and video engine module inside SDK will start really, and it will start to try to collect audio and video. In addition to calling this api normally to preview the local screen, developers can also pass [null] to the canvas parameter, in conjunction with ZegoExpressEngine's sound wave function, in order to achieve the purpose of detecting whether the audio equipment is working properly before logging in to the room.
    ///
    /// - [canvas] The view used to display the preview image. If the view is set to null, no preview will be made.
    /// - [channel] Publish stream channel
    Future<void> startPreview({ZegoCanvas canvas, ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.startPreview(canvas: canvas, channel: channel);
    }

    /// Stop local preview
    ///
    /// This api can be called to stop previewing when there is no need to see the preview locally.
    ///
    /// - [channel] Publish stream channel
    Future<void> stopPreview({ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.stopPreview(channel: channel);
    }

    /// Set up video configuration. You can call this api to publish another streams
    ///
    /// This api can be used to set the video frame rate, bit rate, video capture resolution, and video encoding output resolution. If you do not call this api, the default resolution is 360p, the bit rate is 600 kbps, and the frame rate is 15 fps.
    /// It is necessary to set the relevant video configuration before publishing the stream, and only support the modification of the encoding resolution and the bit rate after publishing the stream.
    /// Developers should note that the wide and high resolution of the mobile end is opposite to the wide and high resolution of the PC. For example, in the case of 360p, the resolution of the mobile end is 360x640, and the resolution of the PC end is 640x360.
    ///
    /// - [config] Video configuration, the SDK provides a common setting combination of resolution, frame rate and bit rate, they also can be customized.
    /// - [channel] Publish stream channel
    Future<void> setVideoConfig(ZegoVideoConfig config, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setVideoConfig(config, channel: channel);
    }

    /// Set mirror mode. You can call this api to set params when publishing another streams
    ///
    /// This interface can be called to set whether the local preview video and the published video have mirror mode enabled.
    ///
    /// - [mirrorMode] Mirror mode for previewing or publishing the stream
    /// - [channel] Publish stream channel
    Future<void> setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setVideoMirrorMode(mirrorMode, channel: channel);
    }

    /// Set the orientation of the captured video. You can call this api to set params when publishing another streams.
    ///
    /// This interface sets the orientation of the video. The captued image is rotated according to the value of the parameter [DeviceOrientation] compared to the forward direction of the phone. After rotation, it will be automatically adjusted to adapt the encoded image resolution.
    ///
    /// - [orientation] Video orientation
    /// - [channel] Publish stream channel
    Future<void> setAppOrientation(DeviceOrientation orientation, ZegoPublishChannel channel) async {
        return await ZegoExpressImpl.instance.setAppOrientation(orientation, channel);
    }

    /// set audio config
    ///
    /// You can set the combined value of the audio codec, bit rate, and audio channel through this interface. If this interface is not called, the default is standard quality mode. Should be used before publishing.
    /// If the preset value cannot meet the developer's scenario, the developer can set the parameters according to the business requirements.
    ///
    /// - [config] Audio config
    Future<void> setAudioConfig(ZegoAudioConfig config) async {
        return await ZegoExpressImpl.instance.setAudioConfig(config);
    }

    /// Stop or resume sending a audio stream. You can call this api to set params when publishing another streams
    ///
    /// This interface can be called when publishing the stream to publish only the video stream without publishing the audio. The SDK still collects and processes the audio, but does not send the audio data to the network. It can be set before publishing.
    /// If you stop sending audio streams, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteMicStateUpdate] callbacks,
    ///
    /// - [mute] Whether to stop sending audio streams, true means that only the video stream is sent without sending the audio stream, and false means that the audio and video streams are sent simultaneously. The default is false.
    /// - [channel] Publish stream channel
    Future<void> mutePublishStreamAudio(bool mute, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.mutePublishStreamAudio(mute, channel: channel);
    }

    /// Stop or resume sending a video stream. You can call this api to set params when publishing another streams
    ///
    /// When publishing the stream, this interface can be called to publish only the audio stream without publishing the video stream. The local camera can still work normally, and can normally capture, preview and process the video picture, but does not send the video data to the network. It can be set before publishing.
    /// If you stop sending video streams locally, the remote user that play stream of local user publishing stream can receive `Mute` status change notification by monitoring [onRemoteCameraStateUpdate] callbacks,
    ///
    /// - [mute] Whether to stop sending video streams, true means that only the audio stream is sent without sending the video stream, and false means that the audio and video streams are sent at the same time. The default is false.
    /// - [channel] Publish stream channel
    Future<void> mutePublishStreamVideo(bool mute, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.mutePublishStreamVideo(mute, channel: channel);
    }

    /// Enable or disable traffic control
    ///
    /// Traffic control enables SDK to dynamically adjust the bitrate of audio and video streaming according to its own and peer current network environment status.
    /// Automatically adapt to the current network environment and fluctuations, so as to ensure the smooth publishing of stream.
    ///
    /// - [enable] Whether to enable traffic control. The default is ture.
    /// - [property] Adjustable property of traffic control, bitmask format. Should be one or the combinations of [ZegoTrafficControlProperty] enumeration. [AdaptiveFPS] as default.
    Future<void> enableTrafficControl(bool enable, int property) async {
        return await ZegoExpressImpl.instance.enableTrafficControl(enable, property);
    }

    /// Set the minimum video bitrate for traffic control
    ///
    /// Set how should SDK send video data when the network conditions are poor and the minimum video bitrate cannot be met.
    /// When this api is not called, the SDK will automatically adjust the sent video data frames according to the current network uplink conditions by default.
    ///
    /// - [bitrate] Minimum video bitrate (kbps)
    /// - [mode] Video sending mode below the minimum bitrate.
    Future<void> setMinVideoBitrateForTrafficControl(int bitrate, ZegoTrafficControlMinVideoBitrateMode mode) async {
        return await ZegoExpressImpl.instance.setMinVideoBitrateForTrafficControl(bitrate, mode);
    }

    /// Set the captured volume for publishing stream
    ///
    /// This interface is used to set the audio collection volume. The local user can control the volume of the audio stream sent to the far end. It can be set before publishing.
    ///
    /// - [volume] Volume percentage. The range is 0 to 100. Default value is 100.
    Future<void> setCaptureVolume(int volume) async {
        return await ZegoExpressImpl.instance.setCaptureVolume(volume);
    }

    /// Add URL to relay to CDN
    ///
    /// You can call this api to publish the audio and video streams that have been published to the ZEGO real-time audio and video cloud to a custom CDN content distribution network that has high latency but supports high concurrent playing stream.
    /// Because this called api is essentially a dynamic relay of the audio and video streams published to the ZEGO audio and video cloud to different CDNs, this api needs to be called after the audio and video stream is published to ZEGO real-time cloud successfully.
    /// Since ZEGO's audio and video cloud service itself can be configured to support CDN(content distribution networks), this api is mainly used by developers who have CDN content distribution services themselves.
    /// You can use ZEGO's CDN audio and video streaming content distribution service at the same time by calling this interface and then use the developer who owns the CDN content distribution service.
    /// This interface supports dynamic relay to the CDN content distribution network, so developers can use this api as a disaster recovery solution for CDN content distribution services.
    /// When the [enablePublishDirectToCDN] api is set to true to publish the stream straight to the CDN, then calling this interface will have no effect.
    ///
    /// - [streamID] Stream ID
    /// - [targetURL] CDN relay address, supported address format rtmp.
    /// - Returns The execution result notification of the relay CDN operation, and proceed to the next step according to the execution result.
    Future<ZegoPublisherUpdateCdnUrlResult> addPublishCdnUrl(String streamID, String targetURL) async {
        return await ZegoExpressImpl.instance.addPublishCdnUrl(streamID, targetURL);
    }

    /// Delete the URL relayed to the CDN
    ///
    /// This api is called when a CDN relayed address has been added and needs to stop propagating the stream to the CDN.
    /// This api does not stop publishing audio and video stream to the ZEGO audio and video cloud.
    ///
    /// - [streamID] Stream ID
    /// - [targetURL] CDN relay address, supported address format rtmp, flv, hls
    /// - Returns The execution result notification of the relay CDN operation, and proceed to the next step according to the execution result.
    Future<ZegoPublisherUpdateCdnUrlResult> removePublishCdnUrl(String streamID, String targetURL) async {
        return await ZegoExpressImpl.instance.removePublishCdnUrl(streamID, targetURL);
    }

    /// Whether to publish stream directly to CDN without passing through Zego real-time video cloud server. You can call this api to set params when publishing another streams
    ///
    /// This api needs to be set before start publishing stream.
    /// After calling this api to publish the audio and video stream directly to the CDN, calling [addPublishCdnUrl] and [removePublishCdnUrl] to dynamically repost to the CDN no longer takes effect, because these two API relay or stop the audio and video stream from the ZEGO real-time audio and video cloud If it is published to CDN, if the direct audio and video stream is directly published to the CDN, the audio and video stream cannot be dynamically relay to the CDN through the ZEGO real-time audio and video cloud.
    ///
    /// - [enable] Whether to enable direct publish CDN, true: enable direct publish CDN, false: disable direct publish CDN
    /// - [config] CDN configuration, if null, use Zego's background default configuration
    /// - [channel] Publish stream channel
    Future<void> enablePublishDirectToCDN(bool enable, ZegoCDNConfig config, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.enablePublishDirectToCDN(enable, config, channel: channel);
    }

    /// Set publish watermark. You can call this api to set params when publishing another streams
    ///
    /// Set before publishing. The layout of the watermark cannot exceed the video encoding resolution of stream.
    ///
    /// - [watermark] The upper left corner of the watermark layout is the origin of the coordinate system, and the area cannot exceed the size set by the encoding resolution. If it is null, the watermark is cancelled.
    /// - [isPreviewVisible] the watermark is visible on local preview
    /// - [channel] Publish stream channel
    Future<void> setPublishWatermark({ZegoWatermark watermark, bool isPreviewVisible, ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.setPublishWatermark(watermark: watermark, isPreviewVisible: isPreviewVisible, channel: channel);
    }

    /// Send SEI. You can call this api to set params when publishing another streams
    ///
    /// This interface can synchronize some other additional information while the developer publishes streaming audio and video streaming data while sending streaming media enhancement supplementary information.
    /// Generally, for scenarios such as synchronizing music lyrics or precise layout of video canvas, you can choose to use this api.
    /// After the anchor sends the SEI, the audience can obtain the SEI content by monitoring the callback of [onPlayerRecvSEI].
    /// Since SEI information follows video frames or audio frames, and because of network problems, frames may be dropped, so SEI information may also be dropped. To solve this situation, it should be sent several times within the limited frequency.
    /// Limit frequency: Do not exceed 30 times per second.
    /// Note: This api is effective only when there is video data published. SEI information will not be sent without publishing video data.
    /// The SEI data length is limited to 4096 bytes.
    ///
    /// - [data] SEI data
    /// - [dataLength] SEI data length
    /// - [channel] Publish stream channel
    Future<void> sendSEI(Uint8List data, int dataLength, {ZegoPublishChannel channel}) async {
        return await ZegoExpressImpl.instance.sendSEI(data, dataLength, channel: channel);
    }

    /// On/off hardware encoding
    ///
    /// Whether to use the hardware encoding function when publishing the stream, the GPU is used to encode the stream and to reduce the CPU usage. The setting can take effect before the stream published. If it is set after the stream published, the stream should be stopped first before it takes effect.
    /// Because hard-coded support is not particularly good for a few models, SDK uses software encoding by default. If the developer finds that the device is hot when publishing a high-resolution audio and video stream during testing of some models, you can consider calling this interface to enable hard coding.
    ///
    /// - [enable] Whether to enable hardware encoding, true: enable hardware encoding, false: disable hardware encoding
    Future<void> enableHardwareEncoder(bool enable) async {
        return await ZegoExpressImpl.instance.enableHardwareEncoder(enable);
    }

    /// set capture pipeline scale mode. Whether the video data is scaled immediately when it is acquired or scaled when it is encoded.
    ///
    /// This interface needs to be set before previewing or streaming.
    /// The main effect is whether the local preview is affected when the acquisition resolution is different from the encoding resolution.
    ///
    /// - [mode] capture scale mode
    Future<void> setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) async {
        return await ZegoExpressImpl.instance.setCapturePipelineScaleMode(mode);
    }


}
