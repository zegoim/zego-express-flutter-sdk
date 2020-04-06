
import 'dart:async';
import 'dart:typed_data';
import 'dart:convert';
import 'package:flutter/services.dart';
import 'zego_express_api.dart';
import 'zego_express_defines.dart';

class ZegoExpressImpl {

    /// Method Channel
    static const MethodChannel _channel = const MethodChannel('plugins.zego.im/zego_express_engine');
    static const EventChannel _event = const EventChannel('plugins.zego.im/zego_express_event_handler');

    /// Used to receive the native event stream
    static StreamSubscription<dynamic> _streamSubscription;

    /// Private constructor
    ZegoExpressImpl._internal();

    /// Singleton instance
    static final ZegoExpressImpl instance = ZegoExpressImpl._internal();

    /// Exposing methodChannel to other files
    static MethodChannel get methodChannel => _channel;


    /* Main */

    static Future<void> createEngine(int appID, String appSign, bool isTestEnv, ZegoScenario scenario, {bool enablePlatformView}) async {
        await _channel.invokeMethod('createEngine', {
            'appID': appID,
            'appSign': appSign,
            'isTestEnv': isTestEnv,
            'scenario': scenario.index,
            'enablePlatformView': enablePlatformView ?? false
        });

        _registerEventHandler();

        return null;
    }

    static Future<void> destroyEngine() async {
        await _channel.invokeMethod('destroyEngine');

        _unregisterEventHandler();

        return null;
    }

    static Future<String> getVersion() async {
        return await _channel.invokeMethod('getVersion');
    }

    Future<void> uploadLog() async {
        return await _channel.invokeMethod('uploadLog');
    }

    Future<void> setDebugVerbose(bool enable, ZegoLanguage language) async {
        return await _channel.invokeMethod('setDebugVerbose', {
            'enable': enable,
            'language': language.index
        });
    }


    /* Room */

    Future<void> loginRoom(String roomID, ZegoUser user, {ZegoRoomConfig config}) async {
        return await _channel.invokeMethod('loginRoom', {
            'roomID': roomID,
            'user': user.toMap(),
            'config': config?.toMap() ?? {}
        });
    }

    Future<void> logoutRoom(String roomID) async {
        return await _channel.invokeMethod('logoutRoom', {
            'roomID': roomID
        });
    }


    /* Publisher */

    Future<void> startPublishingStream(String streamID, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('startPublishingStream', {
            'streamID': streamID,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> stopPublishingStream({ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('stopPublishingStream', {
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<ZegoPublisherSetStreamExtraInfoResult> setStreamExtraInfo(String extraInfo, {ZegoPublishChannel channel}) async {
        final Map<dynamic, dynamic> map = await _channel.invokeMethod('setStreamExtraInfo', {
            'extraInfo': extraInfo,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });

        return ZegoPublisherSetStreamExtraInfoResult.fromMap(map);
    }

    Future<void> startPreview({ZegoCanvas canvas, ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('startPreview', {
            'canvas': canvas?.toMap() ?? {},
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> stopPreview({ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('stopPreview', {
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> setVideoConfig(ZegoVideoConfig config, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('setVideoConfig', {
            'config': config.toMap(),
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('setVideoMirrorMode', {
            'mirrorMode': mirrorMode.index,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> setAppOrientation(DeviceOrientation orientation, ZegoPublishChannel channel) async {
        return await _channel.invokeMethod('setAppOrientation', {
            'orientation': orientation.index,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> setAudioConfig(ZegoAudioConfig config) async {
        return await _channel.invokeMethod('setAudioConfig', {
            'config': config.toMap()
        });
    }

    Future<void> mutePublishStreamAudio(bool mute, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('mutePublishStreamAudio', {
            'mute': mute,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> mutePublishStreamVideo(bool mute, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('mutePublishStreamVideo', {
            'mute': mute,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> enableTrafficControl(bool enable, int property) async {
        return await _channel.invokeMethod('enableTrafficControl', {
            'enable': enable,
            'property': property
        });
    }

    Future<void> setMinVideoBitrateForTrafficControl(int bitrate, ZegoTrafficControlMinVideoBitrateMode mode) async {
        return await _channel.invokeMethod('setMinVideoBitrateForTrafficControl', {
            'bitrate': bitrate,
            'mode': mode.index
        });
    }

    Future<void> setCaptureVolume(int volume) async {
        return await _channel.invokeMethod('setCaptureVolume', {
            'volume': volume
        });
    }

    Future<ZegoPublisherUpdateCdnUrlResult> addPublishCdnUrl(String streamID, String targetURL) async {
        final Map<dynamic, dynamic> map = await _channel.invokeMethod('addPublishCdnUrl', {
            'streamID': streamID,
            'targetURL': targetURL
        });

        return ZegoPublisherUpdateCdnUrlResult.fromMap(map);
    }

    Future<ZegoPublisherUpdateCdnUrlResult> removePublishCdnUrl(String streamID, String targetURL) async {
        final Map<dynamic, dynamic> map = await _channel.invokeMethod('removePublishCdnUrl', {
            'streamID': streamID,
            'targetURL': targetURL
        });

        return ZegoPublisherUpdateCdnUrlResult.fromMap(map);
    }

    Future<void> enablePublishDirectToCDN(bool enable, ZegoCDNConfig config, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('enablePublishDirectToCDN', {
            'enable': enable,
            'config': config.toMap(),
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> setPublishWatermark({ZegoWatermark watermark, bool isPreviewVisible, ZegoPublishChannel channel}) async {

        Map<String, dynamic> watermarkMap = {};
        if (watermark != null) {
            watermarkMap = {
                'imageURL': watermark.imageURL,
                'left': watermark.layout.left,
                'top': watermark.layout.top,
                'right': watermark.layout.right,
                'bottom': watermark.layout.bottom
            };
        }

        return await _channel.invokeMethod('setPublishWatermark', {
            'watermark': watermarkMap,
            'isPreviewVisible': isPreviewVisible ?? false,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> sendSEI(Uint8List data, int dataLength, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('sendSEI', {
            'data': data,
            'dataLength': dataLength,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> enableHardwareEncoder(bool enable) async {
        return await _channel.invokeMethod('enableHardwareEncoder', {
            'enable': enable
        });
    }

    Future<void> setCapturePipelineScaleMode(ZegoCapturePipelineScaleMode mode) async {
        return await _channel.invokeMethod('setCapturePipelineScaleMode', {
            'mode': mode.index
        });
    }


    /* Player */

    Future<void> startPlayingStream(String streamID, {ZegoCanvas canvas, ZegoPlayerConfig config}) async {
        return await _channel.invokeMethod('startPlayingStream', {
            'streamID': streamID,
            'canvas': canvas?.toMap() ?? {},
            'config': config?.toMap() ?? {}
        });
    }

    Future<void> stopPlayingStream(String streamID) async {
        return await _channel.invokeMethod('stopPlayingStream', {
            'streamID': streamID
        });
    }

    Future<void> setPlayVolume(String streamID, int volume) async {
        return await _channel.invokeMethod('setPlayVolume', {
            'streamID': streamID,
            'volume': volume
        });
    }

    Future<void> mutePlayStreamAudio(String streamID, bool mute) async {
        return await _channel.invokeMethod('mutePlayStreamAudio', {
            'streamID': streamID,
            'mute': mute
        });
    }

    Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
        return await _channel.invokeMethod('mutePlayStreamVideo', {
            'streamID': streamID,
            'mute': mute
        });
    }

    Future<void> enableHardwareDecoder(bool enable) async {
        return await _channel.invokeMethod('enableHardwareDecoder', {
            'enable': enable
        });
    }

    Future<void> enableCheckPoc(bool enable) async {
        return await _channel.invokeMethod('enableCheckPoc', {
            'enable': enable
        });
    }


    /* Mixer */

    Future<ZegoMixerStartResult> startMixerTask(ZegoMixerTask task) async {

        Map<String, dynamic> map = task.toMap();

        List<Map<String, dynamic>> inputList = [];
        for (ZegoMixerInput input in task.inputList) {
            inputList.add(input.toMap());
        }
        map['inputList'] = inputList;

        List<Map<String, dynamic>> outputList = [];
        for (ZegoMixerOutput output in task.outputList) {
            outputList.add(output.toMap());
        }
        map['outputList'] = outputList;

        final Map<dynamic, dynamic> result = await _channel.invokeMethod('startMixerTask', map);

        return ZegoMixerStartResult.fromMap(result);
    }

    Future<ZegoMixerStopResult> stopMixerTask(ZegoMixerTask task) async {

        Map<String, dynamic> map = task.toMap();

        List<Map<String, dynamic>> inputList = [];
        for (ZegoMixerInput input in task.inputList) {
            inputList.add(input.toMap());
        }
        map['inputList'] = inputList;

        List<Map<String, dynamic>> outputList = [];
        for (ZegoMixerOutput output in task.outputList) {
            outputList.add(output.toMap());
        }
        map['outputList'] = outputList;

        final Map<dynamic, dynamic> result = await _channel.invokeMethod('stopMixerTask', map);

        return ZegoMixerStopResult.fromMap(result);
    }


    /* Device */

    Future<void> muteMicrophone(bool mute) async {
        return await _channel.invokeMethod('muteMicrophone', {
            'mute': mute
        });
    }

    Future<void> muteSpeaker(bool mute) async {
        return await _channel.invokeMethod('muteSpeaker', {
            'mute': mute
        });
    }

    Future<void> enableAudioCaptureDevice(bool enable) async {
        return await _channel.invokeMethod('enableAudioCaptureDevice', {
            'enable': enable
        });
    }

    Future<void> setBuiltInSpeakerOn(bool enable) async {
        return await _channel.invokeMethod('setBuiltInSpeakerOn', {
            'enable': enable
        });
    }

    Future<void> enableCamera(bool enable, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('enableCamera', {
            'enable': enable,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> useFrontCamera(bool enable, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('useFrontCamera', {
            'enable': enable,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> startSoundLevelMonitor() async {
        return await _channel.invokeMethod('startSoundLevelMonitor');
    }

    Future<void> stopSoundLevelMonitor() async {
        return await _channel.invokeMethod('stopSoundLevelMonitor');
    }

    Future<void> startAudioSpectrumMonitor() async {
        return await _channel.invokeMethod('startAudioSpectrumMonitor');
    }

    Future<void> stopAudioSpectrumMonitor() async {
        return await _channel.invokeMethod('stopAudioSpectrumMonitor');
    }


    /* PreProcess */

    Future<void> enableAEC(bool enable) async {
        return await _channel.invokeMethod('enableAEC', {
            'enable': enable
        });
    }

    Future<void> setAECMode(ZegoAECMode mode) async {
        return await _channel.invokeMethod('setAECMode', {
            'mode': mode.index
        });
    }

    Future<void> enableAGC(bool enable) async {
        return await _channel.invokeMethod('enableAGC', {
            'enable': enable
        });
    }

    Future<void> enableANS(bool enable) async {
        return await _channel.invokeMethod('enableANS', {
            'enable': enable
        });
    }

    Future<void> enableBeautify(int featureBitmask, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('enableBeautify', {
            'featureBitmask': featureBitmask,
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }

    Future<void> setBeautifyOption(ZegoBeautifyOption option, {ZegoPublishChannel channel}) async {
        return await _channel.invokeMethod('setBeautifyOption', {
            'option': option.toMap(),
            'channel': channel?.index ?? ZegoPublishChannel.Main.index
        });
    }


    /* IM */

    Future<ZegoIMSendBroadcastMessageResult> sendBroadcastMessage(String roomID, String message) async {
        final Map<dynamic, dynamic> map = await _channel.invokeMethod('sendBroadcastMessage', {
            'roomID': roomID,
            'message': message
        });

        return ZegoIMSendBroadcastMessageResult.fromMap(map);
    }

    Future<ZegoIMSendBarrageMessageResult> sendBarrageMessage(String roomID, String message) async {
        final Map<dynamic, dynamic> map = await _channel.invokeMethod('sendBarrageMessage', {
            'roomID': roomID,
            'message': message
        });

        return ZegoIMSendBarrageMessageResult.fromMap(map);
    }

    Future<ZegoIMSendCustomCommandResult> sendCustomCommand(String roomID, String command, List<ZegoUser> toUserList) async {

        List<Map<String, dynamic>> userMapList = [];

        for (ZegoUser user in toUserList) {
            userMapList.add(user.toMap());
        }

        final Map<dynamic, dynamic> map = await _channel.invokeMethod('sendCustomCommand', {
            'roomID': roomID,
            'command': command,
            'toUserList': userMapList
        });

        return ZegoIMSendCustomCommandResult.fromMap(map);
    }


    /* EventHandler */

    static void _registerEventHandler() async {
        _streamSubscription = _event.receiveBroadcastStream().listen(_eventListener);
    }

    static void _unregisterEventHandler() async {
        await _streamSubscription?.cancel();
        _streamSubscription = null;
    }

    static void _eventListener(dynamic data) {
        final Map<dynamic, dynamic> map = data;
        switch(map['method']) {

            case 'onDebugError':
                if (ZegoExpressEngine.onDebugError == null) return;

                ZegoExpressEngine.onDebugError(
                    map['errorCode'],
                    map['funcName'],
                    map['info']
                );
                break;


            /* Room */

            case 'onRoomStateUpdate':
                if (ZegoExpressEngine.onRoomStateUpdate == null) return;

                Map<String, dynamic> extendedData = jsonDecode(map['extendedData']);

                ZegoExpressEngine.onRoomStateUpdate(
                    map['roomID'],
                    ZegoRoomState.values[map['state']],
                    map['errorCode'],
                    extendedData
                );
                break;

            case 'onRoomUserUpdate':
                if (ZegoExpressEngine.onRoomUserUpdate == null) return;

                List<Map<String, dynamic>> userMapList = map['userList'];
                List<ZegoUser> userList = [];
                for (Map<String, dynamic> userMap in userMapList) {
                    ZegoUser user = ZegoUser.fromMap(userMap);
                    userList.add(user);
                }

                ZegoExpressEngine.onRoomUserUpdate(
                    map['roomID'],
                    ZegoUpdateType.values[map['updateType']],
                    userList
                );
                break;

            case 'onRoomStreamUpdate':
                if (ZegoExpressEngine.onRoomStreamUpdate == null) return;

                List<Map<String, dynamic>> streamMapList = map['streamList'];
                List<ZegoStream> streamList = [];
                for (Map<String, dynamic> streamMap in streamMapList) {
                    ZegoStream stream = ZegoStream.fromMap(streamMap);
                    streamList.add(stream);
                }

                ZegoExpressEngine.onRoomStreamUpdate(
                    map['roomID'],
                    ZegoUpdateType.values[map['updateType']],
                    streamList
                );
                break;

            case 'onRoomStreamExtraInfoUpdate':
                if (ZegoExpressEngine.onRoomStreamExtraInfoUpdate == null) return;

                List<Map<String, dynamic>> streamMapList = map['streamList'];
                List<ZegoStream> streamList = [];
                for (Map<String, dynamic> streamMap in streamMapList) {
                    ZegoStream stream = ZegoStream.fromMap(streamMap);
                    streamList.add(stream);
                }

                ZegoExpressEngine.onRoomStreamExtraInfoUpdate(
                    map['roomID'],
                    streamList
                );
                break;


            /* Publisher */

            case 'onPublisherStateUpdate':
                if (ZegoExpressEngine.onPublisherStateUpdate == null) return;

                Map<String, dynamic> extendedData = jsonDecode(map['extendedData']);

                ZegoExpressEngine.onPublisherStateUpdate(
                    map['streamID'],
                    ZegoPublisherState.values[map['state']],
                    map['errorCode'],
                    extendedData
                );
                break;

            case 'onPublisherQualityUpdate':
                if (ZegoExpressEngine.onPublisherQualityUpdate == null) return;

                ZegoExpressEngine.onPublisherQualityUpdate(
                    map['streamID'],
                    ZegoPublishStreamQuality.fromMap(map['quality'])
                );
                break;

            case 'onPublisherCapturedAudioFirstFrame':
                if (ZegoExpressEngine.onPublisherCapturedAudioFirstFrame == null) return;

                ZegoExpressEngine.onPublisherCapturedAudioFirstFrame();
                break;

            case 'onPublisherCapturedVideoFirstFrame':
                if (ZegoExpressEngine.onPublisherCapturedVideoFirstFrame == null) return;

                ZegoExpressEngine.onPublisherCapturedVideoFirstFrame(
                    map['channel']
                );
                break;

            case 'onPublisherVideoSizeChanged':
                if (ZegoExpressEngine.onPublisherVideoSizeChanged == null) return;

                ZegoExpressEngine.onPublisherVideoSizeChanged(
                    map['width'],
                    map['height'],
                    map['channel']
                );
                break;

            case 'onPublisherRelayCDNStateUpdate':
                if (ZegoExpressEngine.onPublisherRelayCDNStateUpdate == null) return;

                List<Map<String, dynamic>> infoMapList = map['infoList'];
                List<ZegoStreamRelayCDNInfo> infoList = [];
                for (Map<String, dynamic> infoMap in infoMapList) {
                    ZegoStreamRelayCDNInfo info = ZegoStreamRelayCDNInfo.fromMap(infoMap);
                    infoList.add(info);
                }

                ZegoExpressEngine.onPublisherRelayCDNStateUpdate(
                    map['stream'],
                    infoList
                );
                break;


            /* Player */

            case 'onPlayerStateUpdate':
                if (ZegoExpressEngine.onPlayerStateUpdate == null) return;

                Map<String, dynamic> extendedData = jsonDecode(map['extendedData']);

                ZegoExpressEngine.onPlayerStateUpdate(
                    map['streamID'],
                    ZegoPlayerState.values[map['state']],
                    map['errorCode'],
                    extendedData
                );
                break;

            case 'onPlayerQualityUpdate':
                if (ZegoExpressEngine.onPlayerQualityUpdate == null) return;

                ZegoExpressEngine.onPlayerQualityUpdate(
                    map['streamID'],
                    ZegoPlayStreamQuality.fromMap(map['quality'])
                );
                break;

            case 'onPlayerMediaEvent':
                if (ZegoExpressEngine.onPlayerMediaEvent == null) return;

                ZegoExpressEngine.onPlayerMediaEvent(
                    map['streamID'],
                    ZegoPlayerMediaEvent.values[map['event']]
                );
                break;

            case 'onPlayerRecvAudioFirstFrame':
                if (ZegoExpressEngine.onPlayerRecvAudioFirstFrame == null) return;

                ZegoExpressEngine.onPlayerRecvAudioFirstFrame(
                    map['streamID']
                );
                break;

            case 'onPlayerRecvVideoFirstFrame':
                if (ZegoExpressEngine.onPlayerRecvVideoFirstFrame == null) return;

                ZegoExpressEngine.onPlayerRecvVideoFirstFrame(
                    map['streamID']
                );
                break;

            case 'onPlayerRenderVideoFirstFrame':
                if (ZegoExpressEngine.onPlayerRenderVideoFirstFrame == null) return;

                ZegoExpressEngine.onPlayerRenderVideoFirstFrame(
                    map['streamID']
                );
                break;

            case 'onPlayerVideoSizeChanged':
                if (ZegoExpressEngine.onPlayerVideoSizeChanged == null) return;

                ZegoExpressEngine.onPlayerVideoSizeChanged(
                    map['streamID'],
                    map['width'],
                    map['height']
                );
                break;

            case 'onPlayerRecvSEI':
                if (ZegoExpressEngine.onPlayerRecvSEI == null) return;

                ZegoExpressEngine.onPlayerRecvSEI(
                    map['streamID'],
                    map['data']
                );
                break;


            /* Mixer*/

            case 'onMixerRelayCDNStateUpdate':
                if (ZegoExpressEngine.onMixerRelayCDNStateUpdate == null) return;

                List<Map<String, dynamic>> infoMapList = map['infoList'];
                List<ZegoStreamRelayCDNInfo> infoList = [];
                for (Map<String, dynamic> infoMap in infoMapList) {
                    ZegoStreamRelayCDNInfo info = ZegoStreamRelayCDNInfo.fromMap(infoMap);
                    infoList.add(info);
                }

                ZegoExpressEngine.onMixerRelayCDNStateUpdate(
                    map['taskID'],
                    infoList
                );
                break;

            case 'onMixerSoundLevelUpdate':
                if (ZegoExpressEngine.onMixerSoundLevelUpdate == null) return;

                ZegoExpressEngine.onMixerSoundLevelUpdate(
                    map['soundLevels']
                );
                break;


            /* Device */

            case 'onAudioDeviceStateChanged':
                if (ZegoExpressEngine.onAudioDeviceStateChanged == null) return;

                List<Map<String, dynamic>> infoMapList = map['deviceInfo'];
                List<ZegoDeviceInfo> infoList = [];
                for (Map<String, dynamic> infoMap in infoMapList) {
                    ZegoDeviceInfo info = ZegoDeviceInfo.fromMap(infoMap);
                    infoList.add(info);
                }

                ZegoExpressEngine.onAudioDeviceStateChanged(
                    ZegoUpdateType.values[map['updateType']],
                    ZegoAudioDeviceType.values[map['deviceType']],
                    infoList
                );
                break;

            case 'onVideoDeviceStateChanged':
                if (ZegoExpressEngine.onVideoDeviceStateChanged == null) return;

                List<Map<String, dynamic>> infoMapList = map['deviceInfo'];
                List<ZegoDeviceInfo> infoList = [];
                for (Map<String, dynamic> infoMap in infoMapList) {
                    ZegoDeviceInfo info = ZegoDeviceInfo.fromMap(infoMap);
                    infoList.add(info);
                }

                ZegoExpressEngine.onVideoDeviceStateChanged(
                    ZegoUpdateType.values[map['updateType']],
                    infoList
                );
                break;

            case 'onCapturedSoundLevelUpdate':
                if (ZegoExpressEngine.onCapturedSoundLevelUpdate == null) return;

                ZegoExpressEngine.onCapturedSoundLevelUpdate(
                    map['soundLevel']
                );
                break;

            case 'onRemoteSoundLevelUpdate':
                if (ZegoExpressEngine.onRemoteSoundLevelUpdate == null) return;

                ZegoExpressEngine.onRemoteSoundLevelUpdate(
                    map['soundLevels']
                );
                break;

            case 'onCapturedAudioSpectrumUpdate':
                if (ZegoExpressEngine.onCapturedAudioSpectrumUpdate == null) return;

                ZegoExpressEngine.onCapturedAudioSpectrumUpdate(
                    map['audioSpectrum']
                );
                break;

            case 'onRemoteAudioSpectrumUpdate':
                if (ZegoExpressEngine.onRemoteAudioSpectrumUpdate == null) return;

                ZegoExpressEngine.onRemoteAudioSpectrumUpdate(
                    map['audioSpectrums']
                );
                break;

            case 'onDeviceError':
                if (ZegoExpressEngine.onDeviceError == null) return;

                ZegoExpressEngine.onDeviceError(
                    map['errorCode'],
                    map['deviceName']
                );
                break;

            case 'onRemoteCameraStateUpdate':
                if (ZegoExpressEngine.onRemoteCameraStateUpdate == null) return;

                ZegoExpressEngine.onRemoteCameraStateUpdate(
                    map['streamID'],
                    ZegoRemoteDeviceState.values[map['state']]
                );
                break;

            case 'onRemoteMicStateUpdate':
                if (ZegoExpressEngine.onRemoteMicStateUpdate == null) return;

                ZegoExpressEngine.onRemoteMicStateUpdate(
                    map['streamID'],
                    ZegoRemoteDeviceState.values[map['state']]
                );
                break;


            /* IM */

            case 'onIMRecvBroadcastMessage':
                if (ZegoExpressEngine.onIMRecvBroadcastMessage == null) return;

                List<Map<String, dynamic>> messageMapList = map['deviceInfo'];
                List<ZegoBroadcastMessageInfo> messageList = [];
                for (Map<String, dynamic> messageMap in messageMapList) {
                    ZegoBroadcastMessageInfo message = ZegoBroadcastMessageInfo.fromMap(messageMap);
                    messageList.add(message);
                }

                ZegoExpressEngine.onIMRecvBroadcastMessage(
                    map['roomID'],
                    messageList
                );
                break;

            case 'onIMRecvBarrageMessage':
                if (ZegoExpressEngine.onIMRecvBarrageMessage == null) return;

                List<Map<String, dynamic>> messageMapList = map['deviceInfo'];
                List<ZegoBarrageMessageInfo> messageList = [];
                for (Map<String, dynamic> messageMap in messageMapList) {
                    ZegoBarrageMessageInfo message = ZegoBarrageMessageInfo.fromMap(messageMap);
                    messageList.add(message);
                }

                ZegoExpressEngine.onIMRecvBarrageMessage(
                    map['roomID'],
                    messageList
                );
                break;

            case 'onIMRecvCustomCommand':
                if (ZegoExpressEngine.onIMRecvCustomCommand == null) return;

                ZegoExpressEngine.onIMRecvCustomCommand(
                    map['roomID'],
                    ZegoUser.fromMap(map['fromUser']),
                    map['command']
                );
                break;

            default:
                break;
        }

    }
}