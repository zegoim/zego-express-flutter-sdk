import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'zego_express_texture_renderer_impl.dart';
import '../zego_express_api.dart';
import '../zego_express_defines.dart';
import '../zego_express_enum_extension.dart';
import '../utils/zego_express_utils.dart';

// ignore_for_file: deprecated_member_use_from_same_package, curly_braces_in_flow_control_structures

class Global {
  static String pluginVersion = "3.0.0";
}

class ZegoExpressImpl {
  /// Method Channel
  static const MethodChannel _channel =
      MethodChannel('plugins.zego.im/zego_express_engine');
  static const EventChannel _event =
      EventChannel('plugins.zego.im/zego_express_event_handler');

  /// Used to receive the native event stream
  static StreamSubscription<dynamic>? _streamSubscription;

  /// Private constructor
  ZegoExpressImpl._internal();

  /// Singleton instance
  static final ZegoExpressImpl instance = ZegoExpressImpl._internal();

  /// Exposing methodChannel to other files
  static MethodChannel get methodChannel => _channel;

  // is create engine
  static bool isEngineCreated = false;

  // enablePlatformView
  static bool _enablePlatformView = false;

  static bool shouldUsePlatformView() {
    bool use = ZegoExpressImpl._enablePlatformView;
    // Web only supports PlatformView
    use |= kIsWeb;
    // TODO: PlatformView support on Windows has not yet been implemented
    // Ref: https://github.com/flutter/flutter/issues/31713
    use &= !kIsWindows;
    // TODO: PlatformView support on macOS has a crash issue, don't use it now
    // Ref: https://github.com/flutter/flutter/issues/96668
    use &= !kIsMacOS;
    return use;
  }

  /* Main */

  static Future<void> createEngineWithProfile(ZegoEngineProfile profile) async {
    _registerEventHandler();

    _enablePlatformView = profile.enablePlatformView ?? false;

    await _channel.invokeMethod('createEngineWithProfile', {
      'profile': {
        'appID': profile.appID,
        'appSign': profile.appSign,
        'scenario': profile.scenario.index,
        'enablePlatformView': shouldUsePlatformView()
      }
    });

    await _channel
        .invokeMethod('setPluginVersion', {'version': Global.pluginVersion});

    isEngineCreated = true;
  }

  static Future<void> createEngine(
      int appID, String appSign, bool isTestEnv, ZegoScenario scenario,
      {bool? enablePlatformView}) async {
    _registerEventHandler();

    _enablePlatformView = enablePlatformView ?? false;

    await _channel.invokeMethod('createEngine', {
      'appID': appID,
      'appSign': appSign,
      'isTestEnv': isTestEnv,
      'scenario': scenario.index,
      'enablePlatformView': shouldUsePlatformView()
    });

    await _channel
        .invokeMethod('setPluginVersion', {'version': Global.pluginVersion});

    isEngineCreated = true;
  }

  static Future<void> destroyEngine() async {
    await _channel.invokeMethod('destroyEngine');

    _unregisterEventHandler();

    isEngineCreated = false;
  }

  static Future<void> setEngineConfig(ZegoEngineConfig config) async {
    return await _channel.invokeMethod('setEngineConfig', {
      'config': {
        'logConfig': config.logConfig != null
            ? {
                'logPath': config.logConfig?.logPath,
                'logSize': config.logConfig?.logSize
              }
            : {},
        'advancedConfig': config.advancedConfig ?? {}
      }
    });
  }

  static Future<void> setLogConfig(ZegoLogConfig config) async {
    return await _channel.invokeMethod('setLogConfig', {
      'config': {'logPath': config.logPath, 'logSize': config.logSize}
    });
  }

  static Future<void> setRoomMode(ZegoRoomMode mode) async {
    return await _channel.invokeMethod('setRoomMode', {'mode': mode.index});
  }

  static Future<String> getVersion() async {
    return await _channel.invokeMethod('getVersion');
  }

  static Future<bool> isFeatureSupported(ZegoFeatureType featureType) async {
    return await _channel
        .invokeMethod('isFeatureSupported', {'featureType': featureType.index});
  }

  Future<void> setRoomScenario(ZegoScenario scenario) async {
    return await _channel
        .invokeMethod('setRoomScenario', {'scenario': scenario.index});
  }

  Future<void> uploadLog() async {
    return await _channel.invokeMethod('uploadLog');
  }

  Future<void> enableDebugAssistant(bool enable) async {
    return await _channel
        .invokeMethod('enableDebugAssistant', {'enable': enable});
  }

  Future<String> callExperimentalAPI(String params) async {
    return await _channel
        .invokeMethod('callExperimentalAPI', {'params': params});
  }

  Future<void> setDummyCaptureImagePath(
      String filePath, ZegoPublishChannel channel) async {
    return await _channel.invokeMethod('setDummyCaptureImagePath',
        {'filePath': filePath, 'channel': channel.index});
  }

  /* Room */

  Future<ZegoRoomLoginResult> loginRoom(String roomID, ZegoUser user,
      {ZegoRoomConfig? config}) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod('loginRoom', {
      'roomID': roomID,
      'user': {'userID': user.userID, 'userName': user.userName},
      'config': config?.toMap() ?? {}
    });
    Map<String, dynamic> extendedData = jsonDecode(map['extendedData']);
    return ZegoRoomLoginResult(
        map['errorCode'], Map<String, dynamic>.from(extendedData));
  }

  Future<ZegoRoomLogoutResult> logoutRoom([String? roomID]) async {
    final Map<dynamic, dynamic> map =
        await _channel.invokeMethod('logoutRoom', {'roomID': roomID});
    Map<String, dynamic> extendedData = jsonDecode(map['extendedData']);
    return ZegoRoomLogoutResult(
        map['errorCode'], Map<String, dynamic>.from(extendedData));
  }

  Future<void> switchRoom(String fromRoomID, String toRoomID,
      {ZegoRoomConfig? config}) async {
    return await _channel.invokeMethod('switchRoom', {
      'fromRoomID': fromRoomID,
      'toRoomID': toRoomID,
      'config': config?.toMap() ?? {}
    });
  }

  Future<void> renewToken(String roomID, String token) async {
    return await _channel
        .invokeMethod('renewToken', {'roomID': roomID, 'token': token});
  }

  Future<ZegoRoomSetRoomExtraInfoResult> setRoomExtraInfo(
      String roomID, String key, String value) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'setRoomExtraInfo', {'roomID': roomID, 'key': key, 'value': value});

    return ZegoRoomSetRoomExtraInfoResult(map['errorCode']);
  }

  /* Publisher */

  Future<void> startPublishingStream(String streamID,
      {ZegoPublisherConfig? config, ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('startPublishingStream', {
      'streamID': streamID,
      'config': config != null
          ? {
              'roomID': config.roomID ?? '',
              'forceSynchronousNetworkTime':
                  config.forceSynchronousNetworkTime ?? 0,
              'streamCensorshipMode': config.streamCensorshipMode?.index ??
                  ZegoStreamCensorshipMode.None.index
            }
          : {},
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> stopPublishingStream({ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('stopPublishingStream',
        {'channel': channel?.index ?? ZegoPublishChannel.Main.index});
  }

  Future<ZegoPublisherSetStreamExtraInfoResult> setStreamExtraInfo(
      String extraInfo,
      {ZegoPublishChannel? channel}) async {
    final Map<dynamic, dynamic> map =
        await _channel.invokeMethod('setStreamExtraInfo', {
      'extraInfo': extraInfo,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });

    return ZegoPublisherSetStreamExtraInfoResult(map['errorCode']);
  }

  Future<void> startPreview(
      {ZegoCanvas? canvas, ZegoPublishChannel? channel}) async {
    if (canvas != null && !shouldUsePlatformView()) {
      ZegoExpressTextureRenderer()
          .setViewMode(canvas.view, canvas.viewMode ?? ZegoViewMode.AspectFit);
      ZegoExpressTextureRenderer()
          .setBackgroundColor(canvas.view, canvas.backgroundColor ?? 0x000000);
    }

    return await _channel.invokeMethod('startPreview', {
      'canvas': canvas != null
          ? {
              'view': canvas.view,
              'viewMode':
                  canvas.viewMode?.index ?? ZegoViewMode.AspectFit.index,
              'backgroundColor': canvas.backgroundColor ?? 0x000000
            }
          : {},
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> stopPreview({ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('stopPreview',
        {'channel': channel?.index ?? ZegoPublishChannel.Main.index});
  }

  Future<void> setVideoConfig(ZegoVideoConfig config,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setVideoConfig', {
      'config': {
        'captureWidth': config.captureWidth,
        'captureHeight': config.captureHeight,
        'encodeWidth': config.encodeWidth,
        'encodeHeight': config.encodeHeight,
        'fps': config.fps,
        'bitrate': config.bitrate,
        'codecID': config.codecID.index,
        'keyFrameInterval': config.keyFrameInterval ?? 2
      },
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<ZegoVideoConfig> getVideoConfig({ZegoPublishChannel? channel}) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'getVideoConfig',
        {'channel': channel?.index ?? ZegoPublishChannel.Main.index});

    ZegoVideoConfig config = ZegoVideoConfig(
        map['captureWidth'],
        map['captureHeight'],
        map['encodeWidth'],
        map['encodeHeight'],
        map['fps'],
        map['bitrate'],
        ZegoVideoCodecID.values[map['codecID']],
        keyFrameInterval: map['keyFrameInterval'] ?? 2);

    return config;
  }

  Future<void> setVideoMirrorMode(ZegoVideoMirrorMode mirrorMode,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setVideoMirrorMode', {
      'mirrorMode': mirrorMode.index,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setAppOrientation(DeviceOrientation orientation,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setAppOrientation', {
      'orientation': orientation.index,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setAudioConfig(ZegoAudioConfig config,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setAudioConfig', {
      'config': {
        'bitrate': config.bitrate,
        'channel': config.channel.index,
        'codecID': config.codecID.index
      },
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<ZegoAudioConfig> getAudioConfig({ZegoPublishChannel? channel}) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'getAudioConfig',
        {'channel': channel?.index ?? ZegoPublishChannel.Main.index});

    return ZegoAudioConfig(
        map['bitrate'],
        ZegoAudioChannel.values[map['channel']],
        ZegoAudioCodecID.values[map['codecID']]);
  }

  Future<void> setPublishStreamEncryptionKey(String key,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setPublishStreamEncryptionKey', {
      'key': key,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<ZegoPublisherTakeSnapshotResult> takePublishStreamSnapshot(
      {ZegoPublishChannel? channel}) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'takePublishStreamSnapshot',
        {'channel': channel?.index ?? ZegoPublishChannel.Main.index});

    return ZegoPublisherTakeSnapshotResult(map['errorCode'],
        image: map['image'] != null ? MemoryImage(map['image']) : null);
  }

  Future<void> mutePublishStreamAudio(bool mute,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('mutePublishStreamAudio', {
      'mute': mute,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> mutePublishStreamVideo(bool mute,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('mutePublishStreamVideo', {
      'mute': mute,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setStreamAlignmentProperty(
      int alignment, ZegoPublishChannel channel) async {
    return await _channel.invokeMethod('setStreamAlignmentProperty',
        {'alignment': alignment, 'channel': channel.index});
  }

  Future<void> enableTrafficControl(bool enable, int property) async {
    return await _channel.invokeMethod(
        'enableTrafficControl', {'enable': enable, 'property': property});
  }

  Future<void> setMinVideoBitrateForTrafficControl(
      int bitrate, ZegoTrafficControlMinVideoBitrateMode mode,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setMinVideoBitrateForTrafficControl', {
      'bitrate': bitrate,
      'mode': mode.index,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setMinVideoFpsForTrafficControl(int fps,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setMinVideoFpsForTrafficControl', {
      'fps': fps,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setMinVideoResolutionForTrafficControl(int width, int height,
      {ZegoPublishChannel? channel}) async {
    return await _channel
        .invokeMethod('setMinVideoResolutionForTrafficControl', {
      'width': width,
      'height': height,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setTrafficControlFocusOn(ZegoTrafficControlFocusOnMode mode,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setTrafficControlFocusOn', {
      'mode': mode.index,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setCaptureVolume(int volume) async {
    return await _channel.invokeMethod('setCaptureVolume', {'volume': volume});
  }

  Future<void> setAudioCaptureStereoMode(
      ZegoAudioCaptureStereoMode mode) async {
    return await _channel
        .invokeMethod('setAudioCaptureStereoMode', {'mode': mode.index});
  }

  Future<ZegoPublisherUpdateCdnUrlResult> addPublishCdnUrl(
      String streamID, String targetURL) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'addPublishCdnUrl', {'streamID': streamID, 'targetURL': targetURL});

    return ZegoPublisherUpdateCdnUrlResult(map['errorCode']);
  }

  Future<ZegoPublisherUpdateCdnUrlResult> removePublishCdnUrl(
      String streamID, String targetURL) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'removePublishCdnUrl', {'streamID': streamID, 'targetURL': targetURL});

    return ZegoPublisherUpdateCdnUrlResult(map['errorCode']);
  }

  Future<void> enablePublishDirectToCDN(bool enable,
      {ZegoCDNConfig? config, ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('enablePublishDirectToCDN', {
      'enable': enable,
      'config': config != null
          ? {
              'url': config.url,
              'authParam': config.authParam ?? '',
              'protocol': config.protocol ?? '',
              'quicVersion': config.quicVersion ?? ''
            }
          : {},
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setPublishWatermark(
      {ZegoWatermark? watermark,
      bool? isPreviewVisible,
      ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setPublishWatermark', {
      'watermark': watermark?.toMap() ?? {},
      'isPreviewVisible': isPreviewVisible ?? false,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setSEIConfig(ZegoSEIConfig config) async {
    return await _channel.invokeMethod('setSEIConfig', {
      'config': {'type': config.type.index}
    });
  }

  Future<void> sendSEI(Uint8List data, int dataLength,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('sendSEI', {
      'data': data,
      'dataLength': dataLength,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> sendAudioSideInfo(Uint8List data, double timeStampMs,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('sendAudioSideInfo', {
      'data': data,
      'timeStampMs': timeStampMs,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> enableHardwareEncoder(bool enable) async {
    return await _channel
        .invokeMethod('enableHardwareEncoder', {'enable': enable});
  }

  Future<void> setCapturePipelineScaleMode(
      ZegoCapturePipelineScaleMode mode) async {
    return await _channel
        .invokeMethod('setCapturePipelineScaleMode', {'mode': mode.index});
  }

  Future<void> enableH265EncodeFallback(bool enable) async {
    return await _channel
        .invokeMethod('enableH265EncodeFallback', {'enable': enable});
  }

  Future<int> isVideoEncoderSupported(ZegoVideoCodecID codecID,
      {ZegoVideoCodecBackend? codecBackend}) async {
    return await _channel.invokeMethod('isVideoEncoderSupported',
        {'codecID': codecID.index, 'codecBackend': codecBackend?.index});
  }

  Future<void> setAppOrientationMode(ZegoOrientationMode mode) async {
    return await _channel
        .invokeMethod('setAppOrientationMode', {'mode': mode.index});
  }

  Future<void> setLowlightEnhancement(ZegoLowlightEnhancementMode mode,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setLowlightEnhancement', {
      'mode': mode.index,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  /* Player */

  Future<void> startPlayingStream(String streamID,
      {ZegoCanvas? canvas, ZegoPlayerConfig? config}) async {
    if (canvas != null && !shouldUsePlatformView()) {
      ZegoExpressTextureRenderer()
          .setViewMode(canvas.view, canvas.viewMode ?? ZegoViewMode.AspectFit);
      ZegoExpressTextureRenderer()
          .setBackgroundColor(canvas.view, canvas.backgroundColor ?? 0x000000);
    }

    return await _channel.invokeMethod('startPlayingStream', {
      'streamID': streamID,
      'canvas': canvas != null
          ? {
              'view': canvas.view,
              'viewMode':
                  canvas.viewMode?.index ?? ZegoViewMode.AspectFit.index,
              'backgroundColor': canvas.backgroundColor ?? 0x000000
            }
          : {},
      'config': config != null
          ? {
              'resourceMode': config.resourceMode.index,
              'cdnConfig': config.cdnConfig != null
                  ? {
                      'url': config.cdnConfig?.url,
                      'authParam': config.cdnConfig?.authParam ?? "",
                      'protocol': config.cdnConfig?.protocol ?? "",
                      'quicVersion': config.cdnConfig?.quicVersion ?? ""
                    }
                  : {},
              'roomID': config.roomID ?? '',
              'videoCodecID':
                  config.videoCodecID?.index ?? ZegoVideoCodecID.Default.index,
              'sourceResourceType': config.sourceResourceType?.index ??
                  ZegoResourceType.RTC.index,
              'codecTemplateID': config.codecTemplateID ?? 0
            }
          : {}
    });
  }

  Future<void> stopPlayingStream(String streamID) async {
    return await _channel
        .invokeMethod('stopPlayingStream', {'streamID': streamID});
  }

  Future<void> setPlayStreamDecryptionKey(String streamID, String key) async {
    return await _channel.invokeMethod(
        'setPlayStreamDecryptionKey', {'streamID': streamID, 'key': key});
  }

  Future<void> setPlayStreamCrossAppInfo(
      String streamID, ZegoCrossAppInfo info) async {
    return await _channel.invokeMethod('setPlayStreamCrossAppInfo', {
      'streamID': streamID,
      'info': {'appID': info.appID, 'token': info.token}
    });
  }

  Future<ZegoPlayerTakeSnapshotResult> takePlayStreamSnapshot(
      String streamID) async {
    final Map<dynamic, dynamic> map = await _channel
        .invokeMethod('takePlayStreamSnapshot', {'streamID': streamID});

    return ZegoPlayerTakeSnapshotResult(map['errorCode'],
        image: map['image'] != null ? MemoryImage(map['image']) : null);
  }

  Future<void> setPlayVolume(String streamID, int volume) async {
    return await _channel.invokeMethod(
        'setPlayVolume', {'streamID': streamID, 'volume': volume});
  }

  Future<void> setAllPlayStreamVolume(int volume) async {
    return await _channel
        .invokeMethod('setAllPlayStreamVolume', {'volume': volume});
  }

  Future<void> setPlayStreamVideoType(
      String streamID, ZegoVideoStreamType streamType) async {
    return await _channel.invokeMethod('setPlayStreamVideoType',
        {'streamID': streamID, 'streamType': streamType.index});
  }

  Future<void> setPlayStreamBufferIntervalRange(
      String streamID, int minBufferInterval, int maxBufferInterval) async {
    return await _channel.invokeMethod('setPlayStreamBufferIntervalRange', {
      'streamID': streamID,
      'minBufferInterval': minBufferInterval,
      'maxBufferInterval': maxBufferInterval
    });
  }

  Future<void> setPlayStreamFocusOn(String streamID) async {
    return await _channel
        .invokeMethod('setPlayStreamFocusOn', {'streamID': streamID});
  }

  Future<void> mutePlayStreamAudio(String streamID, bool mute) async {
    return await _channel.invokeMethod(
        'mutePlayStreamAudio', {'streamID': streamID, 'mute': mute});
  }

  Future<void> mutePlayStreamVideo(String streamID, bool mute) async {
    return await _channel.invokeMethod(
        'mutePlayStreamVideo', {'streamID': streamID, 'mute': mute});
  }

  Future<void> muteAllPlayStreamAudio(bool mute) async {
    return await _channel
        .invokeMethod('muteAllPlayStreamAudio', {'mute': mute});
  }

  Future<void> muteAllPlayStreamVideo(bool mute) async {
    return await _channel
        .invokeMethod('muteAllPlayStreamVideo', {'mute': mute});
  }

  Future<void> enableHardwareDecoder(bool enable) async {
    return await _channel
        .invokeMethod('enableHardwareDecoder', {'enable': enable});
  }

  Future<void> enableCheckPoc(bool enable) async {
    return await _channel.invokeMethod('enableCheckPoc', {'enable': enable});
  }

  Future<int> isVideoDecoderSupported(ZegoVideoCodecID codecID,
      {ZegoVideoCodecBackend? codecBackend}) async {
    return await _channel.invokeMethod('isVideoDecoderSupported',
        {'codecID': codecID.index, 'codecBackend': codecBackend?.index});
  }

  Future<void> setPlayStreamsAlignmentProperty(
      ZegoStreamAlignmentMode mode) async {
    return await _channel
        .invokeMethod('setPlayStreamsAlignmentProperty', {'mode': mode.index});
  }

  Future<void> enableVideoSuperResolution(String streamID, bool enable) async {
    return await _channel.invokeMethod(
        'enableVideoSuperResolution', {'streamID': streamID, 'enable': enable});
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
      if (output.videoConfig != null) {
        outputList.add({
          'target': output.target,
          'videoConfig': {
            'videoCodecID': output.videoConfig!.videoCodecID.index,
            'bitrate': output.videoConfig!.bitrate,
            'encodeLatency': output.videoConfig!.encodeLatency,
            'encodeProfile': output.videoConfig!.encodeProfile.index
          }
        });
      } else {
        outputList.add({'target': output.target});
      }
    }
    map['outputList'] = outputList;

    final Map<dynamic, dynamic> result =
        await _channel.invokeMethod('startMixerTask', map);

    return ZegoMixerStartResult(
        result['errorCode'], jsonDecode(result['extendedData']));
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
      if (output.videoConfig != null) {
        outputList.add({
          'target': output.target,
          'videoConfig': {
            'videoCodecID': output.videoConfig!.videoCodecID.index,
            'bitrate': output.videoConfig!.bitrate,
            'encodeLatency': output.videoConfig!.encodeLatency,
            'encodeProfile': output.videoConfig!.encodeProfile.index
          }
        });
      } else {
        outputList.add({'target': output.target});
      }
    }
    map['outputList'] = outputList;

    final Map<dynamic, dynamic> result =
        await _channel.invokeMethod('stopMixerTask', map);

    return ZegoMixerStopResult(
      result['errorCode'],
    );
  }

  Future<ZegoMixerStartResult> startAutoMixerTask(
      ZegoAutoMixerTask task) async {
    Map<String, dynamic> map = task.toMap();

    List<Map<String, dynamic>> outputList = [];
    for (ZegoMixerOutput output in task.outputList) {
      if (output.videoConfig != null) {
        outputList.add({
          'target': output.target,
          'videoConfig': {
            'videoCodecID': output.videoConfig!.videoCodecID.index,
            'bitrate': output.videoConfig!.bitrate,
            'encodeLatency': output.videoConfig!.encodeLatency,
            'encodeProfile': output.videoConfig!.encodeProfile.index
          }
        });
      } else {
        outputList.add({'target': output.target});
      }
    }
    map['outputList'] = outputList;

    final Map<dynamic, dynamic> result =
        await _channel.invokeMethod('startAutoMixerTask', map);

    return ZegoMixerStartResult(
        result['errorCode'], jsonDecode(result['extendedData']));
  }

  Future<ZegoMixerStopResult> stopAutoMixerTask(ZegoAutoMixerTask task) async {
    Map<String, dynamic> map = task.toMap();

    List<Map<String, dynamic>> outputList = [];
    for (ZegoMixerOutput output in task.outputList) {
      if (output.videoConfig != null) {
        outputList.add({
          'target': output.target,
          'videoConfig': {
            'videoCodecID': output.videoConfig!.videoCodecID.index,
            'bitrate': output.videoConfig!.bitrate,
            'encodeLatency': output.videoConfig!.encodeLatency,
            'encodeProfile': output.videoConfig!.encodeProfile.index
          }
        });
      } else {
        outputList.add({'target': output.target});
      }
    }
    map['outputList'] = outputList;

    final Map<dynamic, dynamic> result =
        await _channel.invokeMethod('stopAutoMixerTask', map);

    return ZegoMixerStopResult(
      result['errorCode'],
    );
  }

  /* Device */

  Future<void> muteMicrophone(bool mute) async {
    return await _channel.invokeMethod('muteMicrophone', {'mute': mute});
  }

  Future<bool> isMicrophoneMuted() async {
    return await _channel.invokeMethod('isMicrophoneMuted');
  }

  Future<void> muteSpeaker(bool mute) async {
    return await _channel.invokeMethod('muteSpeaker', {'mute': mute});
  }

  Future<bool> isSpeakerMuted() async {
    return await _channel.invokeMethod('isSpeakerMuted');
  }

  Future<int> getAudioDeviceVolume(
      ZegoAudioDeviceType deviceType, String deviceID) async {
    return await _channel.invokeMethod('getAudioDeviceVolume',
        {'deviceType': deviceType.index, 'deviceID': deviceID});
  }

  Future<void> setAudioDeviceVolume(
      ZegoAudioDeviceType deviceType, String deviceID, int volume) async {
    return await _channel.invokeMethod('setAudioDeviceVolume', {
      'deviceType': deviceType.index,
      'deviceID': deviceID,
      'volume': volume
    });
  }

  Future<void> startAudioDeviceVolumeMonitor(
      ZegoAudioDeviceType deviceType, String deviceID) async {
    return await _channel.invokeMethod('startAudioDeviceVolumeMonitor',
        {'deviceType': deviceType.index, 'deviceID': deviceID});
  }

  Future<void> stopAudioDeviceVolumeMonitor(
      ZegoAudioDeviceType deviceType, String deviceID) async {
    return await _channel.invokeMethod('stopAudioDeviceVolumeMonitor',
        {'deviceType': deviceType.index, 'deviceID': deviceID});
  }

  Future<void> muteAudioDevice(
      ZegoAudioDeviceType deviceType, String deviceID, bool mute) async {
    return await _channel.invokeMethod('muteAudioDevice',
        {'deviceType': deviceType.index, 'deviceID': deviceID, 'mute': mute});
  }

  Future<bool> isAudioDeviceMuted(
      ZegoAudioDeviceType deviceType, String deviceID) async {
    return await _channel.invokeMethod('isAudioDeviceMuted', {
      'deviceType': deviceType.index,
      'deviceID': deviceID,
    });
  }

  Future<void> setAudioDeviceMode(ZegoAudioDeviceMode deviceMode) async {
    return await _channel
        .invokeMethod('setAudioDeviceMode', {'deviceMode': deviceMode.value});
  }

  Future<void> enableAudioCaptureDevice(bool enable) async {
    return await _channel
        .invokeMethod('enableAudioCaptureDevice', {'enable': enable});
  }

  Future<void> setAudioRouteToSpeaker(bool defaultToSpeaker) async {
    return await _channel.invokeMethod(
        'setAudioRouteToSpeaker', {'defaultToSpeaker': defaultToSpeaker});
  }

  Future<ZegoAudioRoute> getAudioRouteType() async {
    int typeIdx = await _channel.invokeMethod('getAudioRouteType');
    return ZegoAudioRoute.values[typeIdx];
  }

  Future<void> enableCamera(bool enable, {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('enableCamera', {
      'enable': enable,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> useFrontCamera(bool enable,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('useFrontCamera', {
      'enable': enable,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<bool> isCameraFocusSupported({ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('isCameraFocusSupported',
        {'channel': channel?.index ?? ZegoPublishChannel.Main.index});
  }

  Future<void> setCameraFocusMode(ZegoCameraFocusMode mode,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setCameraFocusMode', {
      'mode': mode.index,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setCameraFocusPointInPreview(double x, double y,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setCameraFocusPointInPreview', {
      'x': x,
      'y': y,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setCameraExposureMode(ZegoCameraExposureMode mode,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setCameraExposureMode', {
      'mode': mode.index,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setCameraExposurePointInPreview(double x, double y,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setCameraExposurePointInPreview', {
      'x': x,
      'y': y,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setCameraExposureCompensation(double value,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setCameraExposureCompensation', {
      'value': value,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<List<ZegoDeviceInfo>> getAudioDeviceList(
      ZegoAudioDeviceType deviceType) async {
    List deviceInfoMapList = await _channel
        .invokeMethod('getAudioDeviceList', {'type': deviceType.index});
    List<ZegoDeviceInfo> deviceInfoList = [];

    for (var deviceInfoMap in deviceInfoMapList) {
      deviceInfoList.add(ZegoDeviceInfo(
          deviceInfoMap["deviceID"], deviceInfoMap["deviceName"]));
    }

    return deviceInfoList;
  }

  Future<String> getDefaultAudioDeviceID(ZegoAudioDeviceType deviceType) async {
    return await _channel
        .invokeMethod('getDefaultAudioDeviceID', {'type': deviceType.index});
  }

  Future<void> useAudioDevice(
      ZegoAudioDeviceType deviceType, String deviceID) async {
    return await _channel.invokeMethod(
        'useAudioDevice', {'type': deviceType.index, 'deviceID': deviceID});
  }

  Future<void> useVideoDevice(String deviceID,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('useVideoDevice', {
      'deviceID': deviceID,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<List<ZegoDeviceInfo>> getVideoDeviceList() async {
    var resultList = await _channel.invokeMethod('getVideoDeviceList', {});
    print(resultList);
    var retList = <ZegoDeviceInfo>[];
    for (dynamic info in resultList) {
      print(info['deviceID']);
      retList.add(ZegoDeviceInfo(info['deviceID'], info['deviceName']));
    }
    return retList;
  }

  Future<void> useAudioOutputDevice(int mediaID, String deviceID) async {
    return await _channel.invokeMethod(
        'useAudioOutputDevice', {'mediaID': mediaID, 'deviceID': deviceID});
  }

  Future<String> getDefaultVideoDeviceID() async {
    return await _channel.invokeMethod('getDefaultVideoDeviceID', {});
  }

  Future<Map<dynamic, dynamic>> enumDevices() async {
    return await _channel.invokeMethod('enumDevices');
  }

  Future<List> getCameras() async {
    return await _channel.invokeMethod('getCameras');
  }

  Future<List> getMicrophones() async {
    return await _channel.invokeMethod('getMicrophones');
  }

  Future<List> getSpeakers() async {
    return await _channel.invokeMethod('getSpeakers');
  }

  Future<void> setCameraZoomFactor(double factor,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setCameraZoomFactor', {
      'factor': factor,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<double> getCameraMaxZoomFactor({ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('getCameraMaxZoomFactor',
        {'channel': channel?.index ?? ZegoPublishChannel.Main.index});
  }

  Future<void> enableCameraAdaptiveFPS(
      bool enable, int minFPS, int maxFPS, ZegoPublishChannel channel) async {
    return await _channel.invokeMethod('enableCameraAdaptiveFPS', {
      'enable': enable,
      'minFPS': minFPS,
      'maxFPS': maxFPS,
      'channel': channel.index
    });
  }

  Future<void> startSoundLevelMonitor({ZegoSoundLevelConfig? config}) async {
    return await _channel.invokeMethod('startSoundLevelMonitor', {
      'config': {
        'millisecond': config?.millisecond ?? 100,
        'enableVAD': config?.enableVAD ?? false
      }
    });
  }

  Future<void> stopSoundLevelMonitor() async {
    return await _channel.invokeMethod('stopSoundLevelMonitor');
  }

  Future<void> startAudioSpectrumMonitor({int? millisecond}) async {
    return await _channel.invokeMethod(
        'startAudioSpectrumMonitor', {'millisecond': millisecond ?? 100});
  }

  Future<void> stopAudioSpectrumMonitor() async {
    return await _channel.invokeMethod('stopAudioSpectrumMonitor');
  }

  Future<void> enableHeadphoneMonitor(bool enable) async {
    return await _channel
        .invokeMethod('enableHeadphoneMonitor', {'enable': enable});
  }

  Future<void> setHeadphoneMonitorVolume(int volume) async {
    return await _channel
        .invokeMethod('setHeadphoneMonitorVolume', {'volume': volume});
  }

  Future<void> enableMixSystemPlayout(bool enable) async {
    return await _channel
        .invokeMethod('enableMixSystemPlayout', {'enable': enable});
  }

  Future<void> setMixSystemPlayoutVolume(int volume) async {
    return await _channel
        .invokeMethod('setMixSystemPlayoutVolume', {'volume': volume});
  }

  Future<void> enableMixEnginePlayout(bool enable) async {
    return await _channel
        .invokeMethod('enableMixEnginePlayout', {'enable': enable});
  }

  Future<void> startAudioVADStableStateMonitor(
      ZegoAudioVADStableStateMonitorType type,
      {int? millisecond}) async {
    return await _channel.invokeMethod('startAudioVADStableStateMonitor',
        {'type': type.index, 'millisecond': millisecond});
  }

  Future<void> stopAudioVADStableStateMonitor(
      ZegoAudioVADStableStateMonitorType type) async {
    return await _channel
        .invokeMethod('stopAudioVADStableStateMonitor', {'type': type.index});
  }

  Future<ZegoDeviceInfo> getCurrentAudioDevice(
      ZegoAudioDeviceType deviceType) async {
    var resultMap = await _channel.invokeMethod(
        'getCurrentAudioDevice', {'deviceType': deviceType.index});
    return ZegoDeviceInfo(resultMap['deviceID'], resultMap['deviceName']);
  }

  /* PreProcess */

  Future<void> enableAEC(bool enable) async {
    return await _channel.invokeMethod('enableAEC', {'enable': enable});
  }

  Future<void> enableHeadphoneAEC(bool enable) async {
    return await _channel
        .invokeMethod('enableHeadphoneAEC', {'enable': enable});
  }

  Future<void> setAECMode(ZegoAECMode mode) async {
    return await _channel.invokeMethod('setAECMode', {'mode': mode.index});
  }

  Future<void> enableAGC(bool enable) async {
    return await _channel.invokeMethod('enableAGC', {'enable': enable});
  }

  Future<void> enableANS(bool enable) async {
    return await _channel.invokeMethod('enableANS', {'enable': enable});
  }

  Future<void> enableTransientANS(bool enable) async {
    return await _channel
        .invokeMethod('enableTransientANS', {'enable': enable});
  }

  Future<void> setANSMode(ZegoANSMode mode) async {
    return await _channel.invokeMethod('setANSMode', {'mode': mode.index});
  }

  Future<void> startEffectsEnv() async {
    return await _channel.invokeMethod('startEffectsEnv');
  }

  Future<void> stopEffectsEnv() async {
    return await _channel.invokeMethod('stopEffectsEnv');
  }

  Future<void> enableEffectsBeauty(bool enable) async {
    return await _channel
        .invokeMethod('enableEffectsBeauty', {'enable': enable});
  }

  Future<void> setEffectsBeautyParam(ZegoEffectsBeautyParam param) async {
    return await _channel.invokeMethod('setEffectsBeautyParam', {
      'param': {
        'rosyIntensity': param.rosyIntensity,
        'sharpenIntensity': param.sharpenIntensity,
        'smoothIntensity': param.smoothIntensity,
        'whitenIntensity': param.whitenIntensity
      }
    });
  }

  Future<void> enableBeautify(int featureBitmask,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('enableBeautify', {
      'featureBitmask': featureBitmask,
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setBeautifyOption(ZegoBeautifyOption option,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('setBeautifyOption', {
      'option': {
        'polishStep': option.polishStep,
        'whitenFactor': option.whitenFactor,
        'sharpenFactor': option.sharpenFactor
      },
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> setAudioEqualizerGain(int bandIndex, double bandGain) async {
    return await _channel.invokeMethod('setAudioEqualizerGain',
        {'bandIndex': bandIndex, 'bandGain': bandGain});
  }

  Future<void> setVoiceChangerPreset(ZegoVoiceChangerPreset preset) async {
    return await _channel
        .invokeMethod('setVoiceChangerPreset', {'preset': preset.index});
  }

  Future<void> setVoiceChangerParam(ZegoVoiceChangerParam param) async {
    return await _channel.invokeMethod('setVoiceChangerParam', {
      'param': {'pitch': param.pitch}
    });
  }

  Future<void> setReverbPreset(ZegoReverbPreset preset) async {
    return await _channel
        .invokeMethod('setReverbPreset', {'preset': preset.index});
  }

  Future<void> setReverbAdvancedParam(ZegoReverbAdvancedParam param) async {
    return await _channel.invokeMethod('setReverbAdvancedParam', {
      'param': {
        'roomSize': param.roomSize,
        'reverberance': param.reverberance,
        'damping': param.damping,
        'wetOnly': param.wetOnly,
        'wetGain': param.wetGain,
        'dryGain': param.dryGain,
        'toneLow': param.toneLow,
        'toneHigh': param.toneHigh,
        'preDelay': param.preDelay,
        'stereoWidth': param.stereoWidth
      }
    });
  }

  Future<void> setReverbEchoParam(ZegoReverbEchoParam param) async {
    return await _channel.invokeMethod('setReverbEchoParam', {
      'param': {
        'inGain': param.inGain,
        'outGain': param.outGain,
        'numDelays': param.numDelays,
        'delay': param.delay,
        'decay': param.decay
      }
    });
  }

  Future<void> enableVirtualStereo(bool enable, int angle) async {
    return await _channel.invokeMethod(
        'enableVirtualStereo', {'enable': enable, 'angle': angle});
  }

  Future<void> enablePlayStreamVirtualStereo(
      bool enable, int angle, String streamID) async {
    return await _channel.invokeMethod('enablePlayStreamVirtualStereo',
        {'enable': enable, 'angle': angle, 'streamID': streamID});
  }

  Future<void> setElectronicEffects(
      bool enable, ZegoElectronicEffectsMode mode, int tonal) async {
    return await _channel.invokeMethod('setElectronicEffects',
        {'enable': enable, 'mode': mode.index, 'tonal': tonal});
  }

  /* IM */

  static final Map<int, ZegoRealTimeSequentialDataManager>
      realTimeSequentialDataManagerMap = {};

  Future<ZegoRealTimeSequentialDataManager?>
      createRealTimeSequentialDataManager(String roomID) async {
    int index = await _channel.invokeMethod(
        'createRealTimeSequentialDataManager', {'roomID': roomID});

    if (index >= 0) {
      ZegoRealTimeSequentialDataManagerImpl realTimeSequentialDataManagerImpl =
          ZegoRealTimeSequentialDataManagerImpl(index);
      realTimeSequentialDataManagerMap[index] =
          realTimeSequentialDataManagerImpl;
      return realTimeSequentialDataManagerImpl;
    } else {
      return null;
    }
  }

  Future<void> destroyRealTimeSequentialDataManager(
      ZegoRealTimeSequentialDataManager manager) async {
    int index = manager.getIndex();

    await _channel
        .invokeMethod('destroyRealTimeSequentialDataManager', {'index': index});

    realTimeSequentialDataManagerMap.remove(index);
  }

  Future<ZegoIMSendBroadcastMessageResult> sendBroadcastMessage(
      String roomID, String message) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'sendBroadcastMessage', {'roomID': roomID, 'message': message});

    return ZegoIMSendBroadcastMessageResult(map['errorCode'], map['messageID']);
  }

  Future<ZegoIMSendBarrageMessageResult> sendBarrageMessage(
      String roomID, String message) async {
    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'sendBarrageMessage', {'roomID': roomID, 'message': message});

    return ZegoIMSendBarrageMessageResult(map['errorCode'], map['messageID']);
  }

  Future<ZegoIMSendCustomCommandResult> sendCustomCommand(
      String roomID, String command, List<ZegoUser> toUserList) async {
    List<Map<String, dynamic>> userMapList = [];

    for (ZegoUser user in toUserList) {
      userMapList.add({'userID': user.userID, 'userName': user.userName});
    }

    final Map<dynamic, dynamic> map = await _channel.invokeMethod(
        'sendCustomCommand',
        {'roomID': roomID, 'command': command, 'toUserList': userMapList});

    return ZegoIMSendCustomCommandResult(map['errorCode']);
  }

  /* MediaPlayer */

  static final Map<int, ZegoMediaPlayer> mediaPlayerMap = {};

  Future<ZegoMediaPlayer?> createMediaPlayer() async {
    int index = await _channel.invokeMethod('createMediaPlayer');

    if (index >= 0) {
      ZegoMediaPlayerImpl mediaPlayerInstance = ZegoMediaPlayerImpl(index);
      mediaPlayerMap[index] = mediaPlayerInstance;

      return mediaPlayerInstance;
    } else {
      return null;
    }
  }

  Future<void> destroyMediaPlayer(ZegoMediaPlayer mediaPlayer) async {
    int index = mediaPlayer.getIndex();

    await _channel.invokeMethod('destroyMediaPlayer', {'index': index});

    mediaPlayerMap.remove(index);

    return;
  }

  /* AudioEffectPlayer */

  static final Map<int, ZegoAudioEffectPlayer> audioEffectPlayerMap = {};

  Future<ZegoAudioEffectPlayer?> createAudioEffectPlayer() async {
    int index = await _channel.invokeMethod('createAudioEffectPlayer');

    if (index >= 0) {
      ZegoAudioEffectPlayerImpl audioEffectPlayerInstance =
          ZegoAudioEffectPlayerImpl(index);
      audioEffectPlayerMap[index] = audioEffectPlayerInstance;

      return audioEffectPlayerInstance;
    } else {
      return null;
    }
  }

  Future<void> destroyAudioEffectPlayer(
      ZegoAudioEffectPlayer audioEffectPlayer) async {
    int index = audioEffectPlayer.getIndex();

    await _channel.invokeMethod('destroyAudioEffectPlayer', {'index': index});

    audioEffectPlayerMap.remove(index);

    return;
  }

  /* Record */

  Future<void> startRecordingCapturedData(ZegoDataRecordConfig config,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('startRecordingCapturedData', {
      'config': {
        'filePath': config.filePath,
        'recordType': config.recordType.index
      },
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> stopRecordingCapturedData({ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('stopRecordingCapturedData',
        {'channel': channel?.index ?? ZegoPublishChannel.Main.index});
  }

  /* Custom Video Capture */

  Future<void> enableCustomVideoCapture(bool enable,
      {ZegoCustomVideoCaptureConfig? config,
      ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('enableCustomVideoCapture', {
      'enable': enable,
      'config': config != null ? {'bufferType': config.bufferType.index} : {},
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  /* Custom Audio IO */
  Future<void> enableCustomAudioCaptureProcessing(
      bool enable, ZegoCustomAudioProcessConfig config) async {
    return await _channel.invokeMethod('enableCustomAudioCaptureProcessing', {
      'enable': enable,
      'config': {
        'sampleRate': config.sampleRate.value,
        'channel': config.channel.index,
        'samples': config.samples
      }
    });
  }

  Future<void> enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(
      bool enable, ZegoCustomAudioProcessConfig config) async {
    return await _channel.invokeMethod(
        'enableCustomAudioCaptureProcessingAfterHeadphoneMonitor', {
      'enable': enable,
      'config': {
        'sampleRate': config.sampleRate.value,
        'channel': config.channel.index,
        'samples': config.samples
      }
    });
  }

  Future<void> enableAlignedAudioAuxData(
      bool enable, ZegoAudioFrameParam param) async {
    return await _channel.invokeMethod('enableAlignedAudioAuxData', {
      'enable': enable,
      'param': {
        'sampleRate': param.sampleRate.value,
        'channel': param.channel.index,
      }
    });
  }

  Future<void> enableCustomAudioRemoteProcessing(
      bool enable, ZegoCustomAudioProcessConfig config) async {
    return await _channel.invokeMethod('enableCustomAudioRemoteProcessing', {
      'enable': enable,
      'config': {
        'sampleRate': config.sampleRate.value,
        'channel': config.channel.index,
        'samples': config.samples
      }
    });
  }

  Future<void> enableCustomAudioPlaybackProcessing(
      bool enable, ZegoCustomAudioProcessConfig config) async {
    return await _channel.invokeMethod('enableCustomAudioPlaybackProcessing', {
      'enable': enable,
      'config': {
        'sampleRate': config.sampleRate.value,
        'channel': config.channel.index,
        'samples': config.samples
      }
    });
  }

  Future<void> startAudioDataObserver(
      int observerBitMask, ZegoAudioFrameParam param) async {
    return await _channel.invokeMethod('startAudioDataObserver', {
      'observerBitMask': observerBitMask,
      'param': {
        'sampleRate': param.sampleRate.value,
        'channel': param.channel.index
      }
    });
  }

  Future<void> stopAudioDataObserver() async {
    return await _channel.invokeMethod('stopAudioDataObserver', {});
  }

  Future<void> enableCustomAudioIO(bool enable, ZegoCustomAudioConfig config,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('enableCustomAudioIO', {
      'enable': enable,
      'config': {'sourceType': config.sourceType.index},
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> sendCustomAudioCaptureAACData(
      Uint8List data,
      int dataLength,
      int configLength,
      int referenceTimeMillisecond,
      int samples,
      ZegoAudioFrameParam param,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('sendCustomAudioCaptureAACData', {
      'data': data,
      'dataLength': dataLength,
      'configLength': configLength,
      'referenceTimeMillisecond': referenceTimeMillisecond,
      'samples': samples,
      'param': {
        'sampleRate': param.sampleRate.value,
        'channel': param.channel.index
      },
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> sendCustomAudioCapturePCMData(
      Uint8List data, int dataLength, ZegoAudioFrameParam param,
      {ZegoPublishChannel? channel}) async {
    return await _channel.invokeMethod('sendCustomAudioCapturePCMData', {
      'data': data,
      'dataLength': dataLength,
      'param': {
        'sampleRate': param.sampleRate.value,
        'channel': param.channel.index
      },
      'channel': channel?.index ?? ZegoPublishChannel.Main.index
    });
  }

  Future<void> fetchCustomAudioRenderPCMData(
      Uint8List data, int dataLength, ZegoAudioFrameParam param) async {
    return await _channel.invokeMethod('fetchCustomAudioRenderPCMData', {
      'data': data,
      'dataLength': dataLength,
      'param': {
        'sampleRate': param.sampleRate.value,
        'channel': param.channel.index
      }
    });
  }

  /* Range Audio */
  static ZegoRangeAudioImpl? rangeAudioImpl;
  Future<ZegoRangeAudio?> createRangeAudio() async {
    int index = await _channel.invokeMethod('createRangeAudio');

    if (index >= 0) {
      ZegoRangeAudioImpl rangeAudio = ZegoRangeAudioImpl();
      rangeAudioImpl = rangeAudio;
      return rangeAudioImpl;
    } else {
      return null;
    }
  }

  Future<void> destroyRangeAudio(ZegoRangeAudio rangeAudio) async {
    int index = 0;
    await _channel.invokeMethod('destroyRangeAudio', {'index': index});
    rangeAudioImpl = null;
    return;
  }

  /* Utilities */

  Future<void> startPerformanceMonitor({int? millisecond}) async {
    return await _channel.invokeMethod(
        'startPerformanceMonitor', {'millisecond': millisecond ?? 2000});
  }

  Future<void> stopPerformanceMonitor() async {
    return await _channel.invokeMethod('stopPerformanceMonitor');
  }

  Future<ZegoNetworkProbeResult> startNetworkProbe(
      ZegoNetworkProbeConfig config) async {
    final Map<dynamic, dynamic> map =
        await _channel.invokeMethod('startNetworkProbe', {
      'config': {'enableTraceroute': config.enableTraceroute}
    });

    ZegoNetworkProbeHttpResult? httpProbeResult;
    ZegoNetworkProbeTcpResult? tcpProbeResult;
    ZegoNetworkProbeUdpResult? udpProbeResult;
    ZegoNetworkProbeTracerouteResult? tracerouteResult;

    if (map['httpProbeResult'] != null) {
      httpProbeResult = ZegoNetworkProbeHttpResult(
          map['httpProbeResult']['errorCode'],
          map['httpProbeResult']['requestCostTime']);
    }
    if (map['tcpProbeResult'] != null) {
      tcpProbeResult = ZegoNetworkProbeTcpResult(
          map['tcpProbeResult']['errorCode'],
          map['tcpProbeResult']['rtt'],
          map['tcpProbeResult']['connectCostTime']);
    }
    if (map['udpProbeResult'] != null) {
      udpProbeResult = ZegoNetworkProbeUdpResult(
          map['udpProbeResult']['errorCode'], map['udpProbeResult']['rtt']);
    }
    if (map['tracerouteResult'] != null) {
      tracerouteResult = ZegoNetworkProbeTracerouteResult(
          map['tracerouteResult']['errorCode'],
          map['tracerouteResult']['tracerouteCostTime']);
    }

    return ZegoNetworkProbeResult(
        httpProbeResult: httpProbeResult,
        tcpProbeResult: tcpProbeResult,
        udpProbeResult: udpProbeResult,
        tracerouteResult: tracerouteResult);
  }

  Future<void> stopNetworkProbe() async {
    return await _channel.invokeMethod('stopNetworkProbe');
  }

  Future<void> startNetworkSpeedTest(ZegoNetworkSpeedTestConfig config,
      {int? interval}) async {
    return await _channel.invokeMethod('startNetworkSpeedTest', {
      'config': {
        'testUplink': config.testUplink,
        'expectedUplinkBitrate': config.expectedUplinkBitrate,
        'testDownlink': config.testDownlink,
        'expectedDownlinkBitrate': config.expectedDownlinkBitrate
      },
      'interval': interval ?? 3000
    });
  }

  Future<void> stopNetworkSpeedTest() async {
    return await _channel.invokeMethod('stopNetworkSpeedTest');
  }

  Future<ZegoNetworkTimeInfo> getNetworkTimeInfo() async {
    final Map<dynamic, dynamic> map =
        await _channel.invokeMethod('getNetworkTimeInfo');
    return ZegoNetworkTimeInfo(map['timestamp'], map['maxDeviation']);
  }

  static ZegoCopyrightedMusic? copyrightedMusicImpl;
  Future<ZegoCopyrightedMusic?> createCopyrightedMusic() async {
    int errorCode = await _channel.invokeMethod('createCopyrightedMusic');
    if (errorCode == 0) {
      copyrightedMusicImpl = ZegoCopyrightedMusicImpl();
    } else {
      copyrightedMusicImpl = null;
    }
    return copyrightedMusicImpl;
  }

  Future<void> destroyCopyrightedMusic(
      ZegoCopyrightedMusic copyrightedMusic) async {
    await _channel.invokeMethod('destroyCopyrightedMusic');
    copyrightedMusicImpl = null;
    return;
  }

  /* EventHandler */

  static void _registerEventHandler() async {
    _streamSubscription =
        _event.receiveBroadcastStream().listen(_eventListener);
  }

  static void _unregisterEventHandler() async {
    await _streamSubscription?.cancel();
    _streamSubscription = null;
  }

  static void _eventListener(dynamic data) {
    final Map<dynamic, dynamic> map = data;
    switch (map['method']) {
      case 'onDebugError':
        if (ZegoExpressEngine.onDebugError == null) return;

        ZegoExpressEngine.onDebugError!(
            map['errorCode'], map['funcName'], map['info']);
        break;

      case 'onApiCalledResult':
        if (ZegoExpressEngine.onApiCalledResult == null) return;

        ZegoExpressEngine.onApiCalledResult!(
            map['errorCode'], map['funcName'], map['info']);
        break;

      case 'onEngineStateUpdate':
        if (ZegoExpressEngine.onEngineStateUpdate == null) return;

        ZegoExpressEngine
            .onEngineStateUpdate!(ZegoEngineState.values[map['state']]);
        break;

      case 'onNetworkTimeSynchronized':
        if (ZegoExpressEngine.onNetworkTimeSynchronized == null) return;

        ZegoExpressEngine.onNetworkTimeSynchronized!();
        break;

      /* Room */

      case 'onRoomStateUpdate':
        if (ZegoExpressEngine.onRoomStateUpdate == null) return;

        Map<dynamic, dynamic> extendedData = jsonDecode(map['extendedData']);

        ZegoExpressEngine.onRoomStateUpdate!(
            map['roomID'],
            ZegoRoomState.values[map['state']],
            map['errorCode'],
            Map<String, dynamic>.from(extendedData));
        break;

      case 'onRoomStateChanged':
        if (ZegoExpressEngine.onRoomStateChanged == null) return;

        Map<dynamic, dynamic> extendedData = jsonDecode(map['extendedData']);

        ZegoExpressEngine.onRoomStateChanged!(
            map['roomID'],
            ZegoRoomStateChangedReason.values[map['reason']],
            map['errorCode'],
            Map<String, dynamic>.from(extendedData));
        break;

      case 'onRoomUserUpdate':
        if (ZegoExpressEngine.onRoomUserUpdate == null) return;

        List<dynamic> userMapList = map['userList'];
        List<ZegoUser> userList = [];
        for (Map<dynamic, dynamic> userMap in userMapList) {
          ZegoUser user = ZegoUser(userMap['userID'], userMap['userName']);
          userList.add(user);
        }

        ZegoExpressEngine.onRoomUserUpdate!(
            map['roomID'], ZegoUpdateType.values[map['updateType']], userList);
        break;

      case 'onRoomOnlineUserCountUpdate':
        if (ZegoExpressEngine.onRoomOnlineUserCountUpdate == null) return;

        ZegoExpressEngine.onRoomOnlineUserCountUpdate!(
            map['roomID'], map['count']);
        break;

      case 'onRoomStreamUpdate':
        if (ZegoExpressEngine.onRoomStreamUpdate == null) return;

        List<dynamic> streamMapList = map['streamList'];
        List<ZegoStream> streamList = [];
        for (Map<dynamic, dynamic> streamMap in streamMapList) {
          ZegoStream stream = ZegoStream(
              ZegoUser(
                  streamMap['user']['userID'], streamMap['user']['userName']),
              streamMap['streamID'],
              streamMap['extraInfo']);
          streamList.add(stream);
        }

        Map<dynamic, dynamic> extendedData = jsonDecode(map['extendedData']);

        ZegoExpressEngine.onRoomStreamUpdate!(
            map['roomID'],
            ZegoUpdateType.values[map['updateType']],
            streamList,
            Map<String, dynamic>.from(extendedData));
        break;

      case 'onRoomStreamExtraInfoUpdate':
        if (ZegoExpressEngine.onRoomStreamExtraInfoUpdate == null) return;

        List<dynamic> streamMapList = map['streamList'];
        List<ZegoStream> streamList = [];
        for (Map<dynamic, dynamic> streamMap in streamMapList) {
          ZegoStream stream = ZegoStream(
              ZegoUser(
                  streamMap['user']['userID'], streamMap['user']['userName']),
              streamMap['streamID'],
              streamMap['extraInfo']);
          streamList.add(stream);
        }

        ZegoExpressEngine.onRoomStreamExtraInfoUpdate!(
            map['roomID'], streamList);
        break;

      case 'onRoomExtraInfoUpdate':
        if (ZegoExpressEngine.onRoomExtraInfoUpdate == null) return;

        List<dynamic> roomExtraInfoMapList = map['roomExtraInfoList'];
        List<ZegoRoomExtraInfo> roomExtraInfoList = [];
        for (Map<dynamic, dynamic> infoMap in roomExtraInfoMapList) {
          ZegoRoomExtraInfo info = ZegoRoomExtraInfo(
              infoMap['key'],
              infoMap['value'],
              ZegoUser(infoMap['updateUser']['userID'],
                  infoMap['updateUser']['userName']),
              infoMap['updateTime']);
          roomExtraInfoList.add(info);
        }

        ZegoExpressEngine.onRoomExtraInfoUpdate!(
            map['roomID'], roomExtraInfoList);
        break;

      case 'onRoomTokenWillExpire':
        if (ZegoExpressEngine.onRoomTokenWillExpire == null) return;

        ZegoExpressEngine.onRoomTokenWillExpire!(
            map['roomID'], map['remainTimeInSecond']);
        break;

      /* Publisher */

      case 'onPublisherStateUpdate':
        if (ZegoExpressEngine.onPublisherStateUpdate == null) return;

        Map<dynamic, dynamic> extendedData = jsonDecode(map['extendedData']);

        ZegoExpressEngine.onPublisherStateUpdate!(
            map['streamID'],
            ZegoPublisherState.values[map['state']],
            map['errorCode'],
            Map<String, dynamic>.from(extendedData));
        break;

      case 'onPublisherQualityUpdate':
        if (ZegoExpressEngine.onPublisherQualityUpdate == null) return;

        ZegoExpressEngine.onPublisherQualityUpdate!(
            map['streamID'],
            ZegoPublishStreamQuality(
              map['quality']['videoCaptureFPS'],
              map['quality']['videoEncodeFPS'],
              map['quality']['videoSendFPS'],
              map['quality']['videoKBPS'],
              map['quality']['audioCaptureFPS'],
              map['quality']['audioSendFPS'],
              map['quality']['audioKBPS'],
              map['quality']['rtt'],
              map['quality']['packetLostRate'],
              ZegoStreamQualityLevel.values[map['quality']['level']],
              map['quality']['isHardwareEncode'],
              ZegoVideoCodecID.values[map['quality']['videoCodecID']],
              map['quality']['totalSendBytes'],
              map['quality']['audioSendBytes'],
              map['quality']['videoSendBytes'],
            ));
        break;

      case 'onPublisherCapturedAudioFirstFrame':
        if (ZegoExpressEngine.onPublisherCapturedAudioFirstFrame == null)
          return;

        ZegoExpressEngine.onPublisherCapturedAudioFirstFrame!();
        break;

      case 'onPublisherCapturedVideoFirstFrame':
        if (ZegoExpressEngine.onPublisherCapturedVideoFirstFrame == null)
          return;

        ZegoExpressEngine.onPublisherCapturedVideoFirstFrame!(
            ZegoPublishChannel.values[map['channel']]);
        break;

      case 'onPublisherRenderVideoFirstFrame':
        if (ZegoExpressEngine.onPublisherRenderVideoFirstFrame == null) return;

        ZegoExpressEngine.onPublisherRenderVideoFirstFrame!(
            ZegoPublishChannel.values[map['channel']]);
        break;

      case 'onPublisherVideoSizeChanged':
        if (ZegoExpressEngine.onPublisherVideoSizeChanged == null) return;

        ZegoExpressEngine.onPublisherVideoSizeChanged!(map['width'],
            map['height'], ZegoPublishChannel.values[map['channel']]);
        break;

      case 'onPublisherRelayCDNStateUpdate':
        if (ZegoExpressEngine.onPublisherRelayCDNStateUpdate == null) return;

        List<dynamic> infoMapList = map['streamInfoList'];
        List<ZegoStreamRelayCDNInfo> infoList = [];
        for (Map<dynamic, dynamic> infoMap in infoMapList) {
          ZegoStreamRelayCDNInfo info = ZegoStreamRelayCDNInfo(
              infoMap['url'],
              ZegoStreamRelayCDNState.values[infoMap['state']],
              ZegoStreamRelayCDNUpdateReason.values[infoMap['updateReason']],
              infoMap['stateTime']);
          infoList.add(info);
        }

        ZegoExpressEngine.onPublisherRelayCDNStateUpdate!(
            map['streamID'], infoList);
        break;

      case 'onPublisherVideoEncoderChanged':
        if (ZegoExpressEngine.onPublisherVideoEncoderChanged == null) return;

        ZegoExpressEngine.onPublisherVideoEncoderChanged!(
            ZegoVideoCodecID.values[map['fromCodecID']],
            ZegoVideoCodecID.values[map['toCodecID']],
            ZegoPublishChannel.values[map['channel']]);
        break;

      case 'onPublisherStreamEvent':
        if (ZegoExpressEngine.onPublisherStreamEvent == null) return;

        ZegoExpressEngine.onPublisherStreamEvent!(
            ZegoStreamEventExtension.fromValue(map['eventID']),
            map['streamID'],
            map['extraInfo']);
        break;

      /* Player */

      case 'onPlayerStateUpdate':
        if (ZegoExpressEngine.onPlayerStateUpdate == null) return;

        Map<dynamic, dynamic> extendedData = jsonDecode(map['extendedData']);

        ZegoExpressEngine.onPlayerStateUpdate!(
            map['streamID'],
            ZegoPlayerState.values[map['state']],
            map['errorCode'],
            Map<String, dynamic>.from(extendedData));
        break;

      case 'onPlayerQualityUpdate':
        if (ZegoExpressEngine.onPlayerQualityUpdate == null) return;

        ZegoExpressEngine.onPlayerQualityUpdate!(
            map['streamID'],
            ZegoPlayStreamQuality(
                map['quality']['videoRecvFPS'],
                map['quality']['videoDejitterFPS'],
                map['quality']['videoDecodeFPS'],
                map['quality']['videoRenderFPS'],
                map['quality']['videoKBPS'],
                map['quality']['videoBreakRate'],
                map['quality']['audioRecvFPS'],
                map['quality']['audioDejitterFPS'],
                map['quality']['audioDecodeFPS'],
                map['quality']['audioRenderFPS'],
                map['quality']['audioKBPS'],
                map['quality']['audioBreakRate'],
                map['quality']['mos'],
                map['quality']['rtt'],
                map['quality']['packetLostRate'],
                map['quality']['peerToPeerDelay'],
                map['quality']['peerToPeerPacketLostRate'],
                ZegoStreamQualityLevel.values[map['quality']['level']],
                map['quality']['delay'],
                map['quality']['avTimestampDiff'],
                map['quality']['isHardwareDecode'],
                ZegoVideoCodecID.values[map['quality']['videoCodecID']],
                map['quality']['totalRecvBytes'],
                map['quality']['audioRecvBytes'],
                map['quality']['videoRecvBytes']));
        break;

      case 'onPlayerMediaEvent':
        if (ZegoExpressEngine.onPlayerMediaEvent == null) return;

        ZegoExpressEngine.onPlayerMediaEvent!(
            map['streamID'], ZegoPlayerMediaEvent.values[map['event']]);
        break;

      case 'onPlayerRecvAudioFirstFrame':
        if (ZegoExpressEngine.onPlayerRecvAudioFirstFrame == null) return;

        ZegoExpressEngine.onPlayerRecvAudioFirstFrame!(map['streamID']);
        break;

      case 'onPlayerRecvVideoFirstFrame':
        if (ZegoExpressEngine.onPlayerRecvVideoFirstFrame == null) return;

        ZegoExpressEngine.onPlayerRecvVideoFirstFrame!(map['streamID']);
        break;

      case 'onPlayerRenderVideoFirstFrame':
        if (ZegoExpressEngine.onPlayerRenderVideoFirstFrame == null) return;

        ZegoExpressEngine.onPlayerRenderVideoFirstFrame!(map['streamID']);
        break;

      case 'onPlayerVideoSizeChanged':
        if (ZegoExpressEngine.onPlayerVideoSizeChanged == null) return;

        ZegoExpressEngine.onPlayerVideoSizeChanged!(
            map['streamID'], map['width'], map['height']);
        break;

      case 'onPlayerRecvSEI':
        if (ZegoExpressEngine.onPlayerRecvSEI == null) return;

        ZegoExpressEngine.onPlayerRecvSEI!(map['streamID'], map['data']);
        break;

      case 'onPlayerRecvAudioSideInfo':
        if (ZegoExpressEngine.onPlayerRecvAudioSideInfo == null) return;

        ZegoExpressEngine.onPlayerRecvAudioSideInfo!(
            map['streamID'], map['data']);
        break;

      case 'onPlayerLowFpsWarning':
        if (ZegoExpressEngine.onPlayerLowFpsWarning == null) return;

        ZegoExpressEngine.onPlayerLowFpsWarning!(
            ZegoVideoCodecID.values[map['codecID']], map['streamID']);
        break;

      case 'onPlayerStreamEvent':
        if (ZegoExpressEngine.onPlayerStreamEvent == null) return;

        ZegoExpressEngine.onPlayerStreamEvent!(
            ZegoStreamEventExtension.fromValue(map['eventID']),
            map['streamID'],
            map['extraInfo']);
        break;

      case 'onPlayerRenderCameraVideoFirstFrame':
        if (ZegoExpressEngine.onPlayerRenderCameraVideoFirstFrame == null)
          return;

        ZegoExpressEngine.onPlayerRenderCameraVideoFirstFrame!(map['streamID']);
        break;

      case 'onPlayerVideoSuperResolutionUpdate':
        if (ZegoExpressEngine.onPlayerVideoSuperResolutionUpdate == null)
          return;

        ZegoExpressEngine.onPlayerVideoSuperResolutionUpdate!(map['streamID'],
            ZegoSuperResolutionState.values[map['state']], map['errorCode']);
        break;

      /* Mixer*/

      case 'onMixerRelayCDNStateUpdate':
        if (ZegoExpressEngine.onMixerRelayCDNStateUpdate == null) return;

        List<dynamic> infoMapList = map['infoList'];
        List<ZegoStreamRelayCDNInfo> infoList = [];
        for (Map<dynamic, dynamic> infoMap in infoMapList) {
          ZegoStreamRelayCDNInfo info = ZegoStreamRelayCDNInfo(
              infoMap['url'],
              ZegoStreamRelayCDNState.values[infoMap['state']],
              ZegoStreamRelayCDNUpdateReason.values[infoMap['updateReason']],
              infoMap['stateTime']);
          infoList.add(info);
        }

        ZegoExpressEngine.onMixerRelayCDNStateUpdate!(map['taskID'], infoList);
        break;

      case 'onMixerSoundLevelUpdate':
        if (ZegoExpressEngine.onMixerSoundLevelUpdate == null) return;

        Map<dynamic, dynamic> soundLevels = map['soundLevels'];

        ZegoExpressEngine
            .onMixerSoundLevelUpdate!(Map<int, double>.from(soundLevels));
        break;

      case 'onAutoMixerSoundLevelUpdate':
        if (ZegoExpressEngine.onAutoMixerSoundLevelUpdate == null) return;

        Map<dynamic, dynamic> soundLevels = map['soundLevels'];

        ZegoExpressEngine.onAutoMixerSoundLevelUpdate!(
            Map<String, double>.from(soundLevels));
        break;

      /* Device */

      case 'onAudioDeviceStateChanged':
        if (ZegoExpressEngine.onAudioDeviceStateChanged == null) return;

        ZegoDeviceInfo info = ZegoDeviceInfo(
            map['deviceInfo']['deviceID'], map['deviceInfo']['deviceName']);

        ZegoExpressEngine.onAudioDeviceStateChanged!(
            ZegoUpdateType.values[map['updateType']],
            ZegoAudioDeviceType.values[map['deviceType']],
            info);
        break;

      case 'onAudioDeviceVolumeChanged':
        if (ZegoExpressEngine.onAudioDeviceVolumeChanged == null) return;

        ZegoExpressEngine.onAudioDeviceVolumeChanged!(
            ZegoAudioDeviceType.values[map['deviceType']],
            map['deviceID'],
            map['volume']);
        break;

      case 'onVideoDeviceStateChanged':
        if (ZegoExpressEngine.onVideoDeviceStateChanged == null) return;

        ZegoDeviceInfo info = ZegoDeviceInfo(
            map['deviceInfo']['deviceID'], map['deviceInfo']['deviceName']);

        ZegoExpressEngine.onVideoDeviceStateChanged!(
            ZegoUpdateType.values[map['updateType']], info);
        break;

      case 'onCapturedSoundLevelUpdate':
        if (ZegoExpressEngine.onCapturedSoundLevelUpdate == null) return;

        ZegoExpressEngine.onCapturedSoundLevelUpdate!(map['soundLevel']);
        break;

      case 'onCapturedSoundLevelInfoUpdate':
        if (ZegoExpressEngine.onCapturedSoundLevelInfoUpdate == null) return;

        Map<dynamic, dynamic> soundLevelInfo = map['soundLevelInfo'];
        ZegoExpressEngine.onCapturedSoundLevelInfoUpdate!(ZegoSoundLevelInfo(
            soundLevelInfo['soundLevel'], soundLevelInfo['vad']));
        break;

      case 'onRemoteSoundLevelUpdate':
        if (ZegoExpressEngine.onRemoteSoundLevelUpdate == null) return;

        var soundLevels = <dynamic, dynamic>{};
        if (map['soundLevels'] != null) {
          soundLevels = map['soundLevels'];
        }

        ZegoExpressEngine
            .onRemoteSoundLevelUpdate!(Map<String, double>.from(soundLevels));
        break;

      case 'onRemoteSoundLevelInfoUpdate':
        if (ZegoExpressEngine.onRemoteSoundLevelInfoUpdate == null) return;

        Map<String, ZegoSoundLevelInfo> resultMap = {};
        Map<dynamic, dynamic> soundLevelInfos = map['soundLevelInfos'];
        soundLevelInfos.forEach((k, v) =>
            resultMap[k] = ZegoSoundLevelInfo(v['soundLevel'], v['vad']));
        ZegoExpressEngine.onRemoteSoundLevelInfoUpdate!(resultMap);
        break;

      case 'onCapturedAudioSpectrumUpdate':
        if (ZegoExpressEngine.onCapturedAudioSpectrumUpdate == null) return;

        List<double> originAudioSpectrum =
            List<double>.from(map['audioSpectrum']);

        ZegoExpressEngine.onCapturedAudioSpectrumUpdate!(originAudioSpectrum);
        break;

      case 'onRemoteAudioSpectrumUpdate':
        if (ZegoExpressEngine.onRemoteAudioSpectrumUpdate == null) return;

        Map<dynamic, dynamic> originAudioSpectrums = map['audioSpectrums'];
        Map<String, List<double>> audioSpectrums = {};
        for (String streamID in originAudioSpectrums.keys) {
          audioSpectrums[streamID] =
              List<double>.from(originAudioSpectrums[streamID]);
        }

        ZegoExpressEngine.onRemoteAudioSpectrumUpdate!(audioSpectrums);
        break;

      case 'onLocalDeviceExceptionOccurred':
        if (ZegoExpressEngine.onLocalDeviceExceptionOccurred == null) return;

        ZegoExpressEngine.onLocalDeviceExceptionOccurred!(
            ZegoDeviceExceptionType.values[map['exceptionType']],
            ZegoDeviceType.values[map['deviceType']],
            map['deviceID']);
        break;

      case 'onRemoteCameraStateUpdate':
        if (ZegoExpressEngine.onRemoteCameraStateUpdate == null) return;

        ZegoExpressEngine.onRemoteCameraStateUpdate!(
            map['streamID'], ZegoRemoteDeviceState.values[map['state']]);
        break;

      case 'onRemoteSpeakerStateUpdate':
        if (ZegoExpressEngine.onRemoteSpeakerStateUpdate == null) return;

        ZegoExpressEngine.onRemoteSpeakerStateUpdate!(
            map['streamID'], ZegoRemoteDeviceState.values[map['state']]);
        break;

      case 'onRemoteMicStateUpdate':
        if (ZegoExpressEngine.onRemoteMicStateUpdate == null) return;

        ZegoExpressEngine.onRemoteMicStateUpdate!(
            map['streamID'], ZegoRemoteDeviceState.values[map['state']]);
        break;

      case 'onAudioRouteChange':
        if (ZegoExpressEngine.onAudioRouteChange == null) return;

        ZegoExpressEngine
            .onAudioRouteChange!(ZegoAudioRoute.values[map['audioRoute']]);
        break;

      case 'onAudioVADStateUpdate':
        if (ZegoExpressEngine.onAudioVADStateUpdate == null) return;

        ZegoExpressEngine.onAudioVADStateUpdate!(
            ZegoAudioVADStableStateMonitorType.values[map['type']],
            ZegoAudioVADType.values[map['state']]);
        break;

      /* IM */

      case 'onIMRecvBroadcastMessage':
        if (ZegoExpressEngine.onIMRecvBroadcastMessage == null) return;

        List<dynamic> messageMapList = map['messageList'];
        List<ZegoBroadcastMessageInfo> messageList = [];
        for (Map<dynamic, dynamic> messageMap in messageMapList) {
          ZegoBroadcastMessageInfo message = ZegoBroadcastMessageInfo(
              messageMap['message'],
              messageMap['messageID'],
              messageMap['sendTime'],
              ZegoUser(messageMap['fromUser']['userID'],
                  messageMap['fromUser']['userName']));
          messageList.add(message);
        }

        ZegoExpressEngine.onIMRecvBroadcastMessage!(map['roomID'], messageList);
        break;

      case 'onIMRecvBarrageMessage':
        if (ZegoExpressEngine.onIMRecvBarrageMessage == null) return;

        List<dynamic> messageMapList = map['messageList'];
        List<ZegoBarrageMessageInfo> messageList = [];
        for (Map<dynamic, dynamic> messageMap in messageMapList) {
          ZegoBarrageMessageInfo message = ZegoBarrageMessageInfo(
              messageMap['message'],
              messageMap['messageID'],
              messageMap['sendTime'],
              ZegoUser(messageMap['fromUser']['userID'],
                  messageMap['fromUser']['userName']));
          messageList.add(message);
        }

        ZegoExpressEngine.onIMRecvBarrageMessage!(map['roomID'], messageList);
        break;

      case 'onIMRecvCustomCommand':
        if (ZegoExpressEngine.onIMRecvCustomCommand == null) return;

        ZegoExpressEngine.onIMRecvCustomCommand!(
            map['roomID'],
            ZegoUser(map['fromUser']['userID'], map['fromUser']['userName']),
            map['command']);
        break;

      /* Utilities */

      case 'onPerformanceStatusUpdate':
        if (ZegoExpressEngine.onPerformanceStatusUpdate == null) return;

        Map<dynamic, dynamic> statusMap = map['status'];
        ZegoExpressEngine.onPerformanceStatusUpdate!(ZegoPerformanceStatus(
            statusMap['cpuUsageApp'],
            statusMap['cpuUsageSystem'],
            statusMap['memoryUsageApp'],
            statusMap['memoryUsageSystem'],
            statusMap['memoryUsedApp']));
        break;

      case 'onNetworkModeChanged':
        if (ZegoExpressEngine.onNetworkModeChanged == null) return;

        ZegoExpressEngine
            .onNetworkModeChanged!(ZegoNetworkMode.values[map['mode']]);
        break;

      case 'onNetworkSpeedTestError':
        if (ZegoExpressEngine.onNetworkSpeedTestError == null) return;

        ZegoExpressEngine.onNetworkSpeedTestError!(
            map['errorCode'], ZegoNetworkSpeedTestType.values[map['type']]);
        break;

      case 'onNetworkSpeedTestQualityUpdate':
        if (ZegoExpressEngine.onNetworkSpeedTestQualityUpdate == null) return;

        ZegoExpressEngine.onNetworkSpeedTestQualityUpdate!(
            ZegoNetworkSpeedTestQuality(
                map['quality']['connectCost'],
                map['quality']['rtt'],
                map['quality']['packetLostRate'],
                ZegoStreamQualityLevel.values[map['quality']['quality']]),
            ZegoNetworkSpeedTestType.values[map['type']]);
        break;

      case 'onRecvExperimentalAPI':
        if (ZegoExpressEngine.onRecvExperimentalAPI == null) return;

        ZegoExpressEngine.onRecvExperimentalAPI!(map['content']);
        break;

      case 'onNetworkQuality':
        if (ZegoExpressEngine.onNetworkQuality == null) return;

        ZegoExpressEngine.onNetworkQuality!(
            map['userID'],
            ZegoStreamQualityLevel.values[map['upstreamQuality']],
            ZegoStreamQualityLevel.values[map['downstreamQuality']]);
        break;

      /* Process Audio Data */
      case 'onProcessCapturedAudioData':
        if (ZegoExpressEngine.onProcessCapturedAudioData == null) return;

        Map<dynamic, dynamic> paramMap = map['param'];

        ZegoExpressEngine.onProcessCapturedAudioData!(
            map['data'],
            map['dataLength'],
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]),
            map['timestamp']);
        break;

      case 'onProcessCapturedAudioDataAfterUsedHeadphoneMonitor':
        if (ZegoExpressEngine
                .onProcessCapturedAudioDataAfterUsedHeadphoneMonitor ==
            null) return;

        Map<dynamic, dynamic> paramMap = map['param'];

        ZegoExpressEngine.onProcessCapturedAudioDataAfterUsedHeadphoneMonitor!(
            map['data'],
            map['dataLength'],
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]),
            map['timestamp']);
        break;

      case 'onAlignedAudioAuxData':
        if (ZegoExpressEngine.onAlignedAudioAuxData == null) return;

        Map<dynamic, dynamic> paramMap = map['param'];

        ZegoExpressEngine.onAlignedAudioAuxData!(
            map['data'],
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]));
        break;

      case 'onProcessRemoteAudioData':
        if (ZegoExpressEngine.onProcessRemoteAudioData == null) return;

        Map<dynamic, dynamic> paramMap = map['param'];

        ZegoExpressEngine.onProcessRemoteAudioData!(
            map['data'],
            map['dataLength'],
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]),
            map['streamID'],
            map['timestamp']);
        break;

      case 'onProcessPlaybackAudioData':
        if (ZegoExpressEngine.onProcessPlaybackAudioData == null) return;

        Map<dynamic, dynamic> paramMap = map['param'];

        ZegoExpressEngine.onProcessPlaybackAudioData!(
            map['data'],
            map['dataLength'],
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]),
            map['timestamp']);
        break;

      /* MediaPlayer */

      case 'onMediaPlayerStateUpdate':
        if (ZegoExpressEngine.onMediaPlayerStateUpdate == null) return;

        int? mediaPlayerIndex = map['mediaPlayerIndex'];
        ZegoMediaPlayer? mediaPlayer =
            ZegoExpressImpl.mediaPlayerMap[mediaPlayerIndex!];
        if (mediaPlayer != null) {
          ZegoExpressEngine.onMediaPlayerStateUpdate!(mediaPlayer,
              ZegoMediaPlayerState.values[map['state']], map['errorCode']);
        }
        break;

      case 'onMediaPlayerNetworkEvent':
        if (ZegoExpressEngine.onMediaPlayerNetworkEvent == null) return;

        int? mediaPlayerIndex = map['mediaPlayerIndex'];
        ZegoMediaPlayer? mediaPlayer =
            ZegoExpressImpl.mediaPlayerMap[mediaPlayerIndex!];
        if (mediaPlayer != null) {
          ZegoExpressEngine.onMediaPlayerNetworkEvent!(mediaPlayer,
              ZegoMediaPlayerNetworkEvent.values[map['networkEvent']]);
        }
        break;

      case 'onMediaPlayerPlayingProgress':
        if (ZegoExpressEngine.onMediaPlayerPlayingProgress == null) return;

        int? mediaPlayerIndex = map['mediaPlayerIndex'];
        ZegoMediaPlayer? mediaPlayer =
            ZegoExpressImpl.mediaPlayerMap[mediaPlayerIndex!];
        if (mediaPlayer != null) {
          ZegoExpressEngine.onMediaPlayerPlayingProgress!(
              mediaPlayer, map['millisecond']);
        }
        break;

      case 'onMediaPlayerRecvSEI':
        if (ZegoExpressEngine.onMediaPlayerRecvSEI == null) return;

        int? mediaPlayerIndex = map['mediaPlayerIndex'];
        ZegoMediaPlayer? mediaPlayer =
            ZegoExpressImpl.mediaPlayerMap[mediaPlayerIndex!];
        if (mediaPlayer != null) {
          ZegoExpressEngine.onMediaPlayerRecvSEI!(mediaPlayer, map['data']);
        }
        break;

      case 'onMediaPlayerSoundLevelUpdate':
        if (ZegoExpressEngine.onMediaPlayerSoundLevelUpdate == null) return;

        int? mediaPlayerIndex = map['mediaPlayerIndex'];
        ZegoMediaPlayer? mediaPlayer =
            ZegoExpressImpl.mediaPlayerMap[mediaPlayerIndex!];
        if (mediaPlayer != null) {
          ZegoExpressEngine.onMediaPlayerSoundLevelUpdate!(
              mediaPlayer, map['soundLevel']);
        }
        break;

      case 'onMediaPlayerFrequencySpectrumUpdate':
        if (ZegoExpressEngine.onMediaPlayerFrequencySpectrumUpdate == null)
          return;

        int? mediaPlayerIndex = map['mediaPlayerIndex'];
        ZegoMediaPlayer? mediaPlayer =
            ZegoExpressImpl.mediaPlayerMap[mediaPlayerIndex!];
        if (mediaPlayer != null) {
          List<double> spectrumList = List<double>.from(map['spectrumList']);
          ZegoExpressEngine.onMediaPlayerFrequencySpectrumUpdate!(
              mediaPlayer, spectrumList);
        }
        break;
      /* AudioEffectPlayer */

      case 'onAudioEffectPlayStateUpdate':
        if (ZegoExpressEngine.onAudioEffectPlayStateUpdate == null) return;

        int? audioEffectPlayerIndex = map['audioEffectPlayerIndex'];
        ZegoAudioEffectPlayer? audioEffectPlayer =
            ZegoExpressImpl.audioEffectPlayerMap[audioEffectPlayerIndex!];
        if (audioEffectPlayer != null) {
          ZegoExpressEngine.onAudioEffectPlayStateUpdate!(
              audioEffectPlayer,
              map['audioEffectID'],
              ZegoAudioEffectPlayState.values[map['state']],
              map['errorCode']);
        }
        break;

      /* Record */

      case 'onCapturedDataRecordStateUpdate':
        if (ZegoExpressEngine.onCapturedDataRecordStateUpdate == null) return;

        ZegoExpressEngine.onCapturedDataRecordStateUpdate!(
            ZegoDataRecordState.values[map['state']],
            map['errorCode'],
            ZegoDataRecordConfig(map['config']['filePath'],
                ZegoDataRecordType.values[map['config']['recordType']]),
            ZegoPublishChannel.values[map['channel']]);
        break;

      case 'onCapturedDataRecordProgressUpdate':
        if (ZegoExpressEngine.onCapturedDataRecordProgressUpdate == null)
          return;

        ZegoExpressEngine.onCapturedDataRecordProgressUpdate!(
            ZegoDataRecordProgress(map['progress']['duration'],
                map['progress']['currentFileSize']),
            ZegoDataRecordConfig(map['config']['filePath'],
                ZegoDataRecordType.values[map['config']['recordType']]),
            ZegoPublishChannel.values[map['channel']]);
        break;

      case 'onCapturedAudioData':
        if (ZegoExpressEngine.onCapturedAudioData == null) return;

        Uint8List data = map['data'];
        int dataLength = map['dataLength'];
        Map<dynamic, dynamic> paramMap = map['param'];
        ZegoExpressEngine.onCapturedAudioData!(
            data,
            dataLength,
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]));
        break;

      case 'onPlaybackAudioData':
        if (ZegoExpressEngine.onPlaybackAudioData == null) return;

        Uint8List data = map['data'];
        int dataLength = map['dataLength'];
        Map<dynamic, dynamic> paramMap = map['param'];
        ZegoExpressEngine.onPlaybackAudioData!(
            data,
            dataLength,
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]));
        break;

      case 'onMixedAudioData':
        if (ZegoExpressEngine.onMixedAudioData == null) return;

        Uint8List data = map['data'];
        int dataLength = map['dataLength'];
        Map<dynamic, dynamic> paramMap = map['param'];
        ZegoExpressEngine.onMixedAudioData!(
            data,
            dataLength,
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]));
        break;

      case 'onPlayerAudioData':
        if (ZegoExpressEngine.onPlayerAudioData == null) return;

        Uint8List data = map['data'];
        int dataLength = map['dataLength'];
        Map<dynamic, dynamic> paramMap = map['param'];
        String streamID = map['streamID'];

        ZegoExpressEngine.onPlayerAudioData!(
            data,
            dataLength,
            ZegoAudioFrameParam(
                ZegoAudioSampleRateExtension.fromValue(paramMap['sampleRate']),
                ZegoAudioChannel.values[paramMap['channel']]),
            streamID);
        break;

      case 'onRangeAudioMicrophoneStateUpdate':
        if (ZegoExpressEngine.onRangeAudioMicrophoneStateUpdate == null ||
            rangeAudioImpl == null) return;

        ZegoExpressEngine.onRangeAudioMicrophoneStateUpdate!(
            rangeAudioImpl!,
            ZegoRangeAudioMicrophoneState.values[map['state']],
            map['errorCode']);
        break;
      /* Real Time Sequential Data Manager */

      case 'onReceiveRealTimeSequentialData':
        if (ZegoExpressEngine.onReceiveRealTimeSequentialData == null) return;

        int? realTimeSequentialDataManagerIndex =
            map['realTimeSequentialDataManagerIndex'];
        var manager = ZegoExpressImpl.realTimeSequentialDataManagerMap[
            realTimeSequentialDataManagerIndex!];
        if (manager != null) {
          ZegoExpressEngine.onReceiveRealTimeSequentialData!(
              manager, map['data'], map['streamID']);
        }
        break;

      /* Copyrighted Music */
      case 'onDownloadProgressUpdate':
        if (ZegoExpressEngine.onDownloadProgressUpdate == null ||
            copyrightedMusicImpl == null) return;

        ZegoExpressEngine.onDownloadProgressUpdate!(
            copyrightedMusicImpl!, map['resourceID'], map['progressRate']);
        break;

      case 'onCurrentPitchValueUpdate':
        if (ZegoExpressEngine.onCurrentPitchValueUpdate == null ||
            copyrightedMusicImpl == null) return;

        ZegoExpressEngine.onCurrentPitchValueUpdate!(copyrightedMusicImpl!,
            map['resourceID'], map['currentDuration'], map['pitchValue']);
        break;

      default:
        // TODO: Unknown callback
        break;
    }
  }
}

class ZegoMediaPlayerImpl extends ZegoMediaPlayer {
  final int _index;
  ZegoMediaPlayerImpl(int index) : _index = index;

  @override
  Future<ZegoMediaPlayerLoadResourceResult> loadResource(String path) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod(
            'mediaPlayerLoadResource', {'index': _index, 'path': path});

    return ZegoMediaPlayerLoadResourceResult(map['errorCode']);
  }

  @override
  Future<ZegoMediaPlayerLoadResourceResult> loadResourceFromMediaData(
      Uint8List mediaData, int startPosition) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerLoadResourceFromMediaData', {
      'index': _index,
      'mediaData': mediaData,
      'startPosition': startPosition
    });

    return ZegoMediaPlayerLoadResourceResult(map['errorCode']);
  }

  @override
  Future<ZegoMediaPlayerLoadResourceResult> loadResourceWithPosition(
      String path, int startPosition) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerLoadResourceWithPosition',
            {'index': _index, 'path': path, 'startPosition': startPosition});

    return ZegoMediaPlayerLoadResourceResult(map['errorCode']);
  }

  @override
  Future<void> start() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerStart', {'index': _index});
  }

  @override
  Future<void> stop() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerStop', {'index': _index});
  }

  @override
  Future<void> pause() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerPause', {'index': _index});
  }

  @override
  Future<void> resume() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerResume', {'index': _index});
  }

  @override
  Future<ZegoMediaPlayerSeekToResult> seekTo(int millisecond) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod(
            'mediaPlayerSeekTo', {'index': _index, 'millisecond': millisecond});

    return ZegoMediaPlayerSeekToResult(map['errorCode']);
  }

  @override
  Future<void> enableRepeat(bool enable) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerEnableRepeat', {'index': _index, 'enable': enable});
  }

  @override
  Future<void> enableAux(bool enable) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerEnableAux', {'index': _index, 'enable': enable});
  }

  @override
  Future<void> muteLocal(bool mute) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerMuteLocal', {'index': _index, 'mute': mute});
  }

  @override
  Future<void> setPlayerCanvas(ZegoCanvas canvas) async {
    if (!ZegoExpressImpl.shouldUsePlatformView()) {
      ZegoExpressTextureRenderer()
          .setViewMode(canvas.view, canvas.viewMode ?? ZegoViewMode.AspectFit);
      ZegoExpressTextureRenderer()
          .setBackgroundColor(canvas.view, canvas.backgroundColor ?? 0x000000);
    }

    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerSetPlayerCanvas', {
      'index': _index,
      'canvas': {
        'view': canvas.view,
        'viewMode': canvas.viewMode?.index ?? ZegoViewMode.AspectFit.index,
        'backgroundColor': canvas.backgroundColor ?? 0x000000
      }
    });
  }

  @override
  Future<void> setVolume(int volume) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetVolume', {'index': _index, 'volume': volume});
  }

  @override
  Future<void> setPlayVolume(int volume) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetPlayVolume', {'index': _index, 'volume': volume});
  }

  @override
  Future<void> setPublishVolume(int volume) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetPublishVolume', {'index': _index, 'volume': volume});
  }

  @override
  Future<void> setProgressInterval(int millisecond) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetProgressInterval',
        {'index': _index, 'millisecond': millisecond});
  }

  @override
  Future<int> getPlayVolume() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerGetPlayVolume', {'index': _index});
  }

  @override
  Future<int> getPublishVolume() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerGetPublishVolume', {'index': _index});
  }

  @override
  Future<int> getTotalDuration() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerGetTotalDuration', {'index': _index});
  }

  @override
  Future<int> getCurrentProgress() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerGetCurrentProgress', {'index': _index});
  }

  @override
  Future<int> getAudioTrackCount() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerGetAudioTrackCount', {'index': _index});
  }

  @override
  Future<void> setAudioTrackIndex(int index) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetAudioTrackIndex',
        {'index': _index, 'trackIndex': index});
  }

  @override
  Future<void> setVoiceChangerParam(ZegoMediaPlayerAudioChannel audioChannel,
      ZegoVoiceChangerParam param) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerSetVoiceChangerParam', {
      'index': _index,
      'audioChannel': audioChannel.index,
      'param': {'pitch': param.pitch}
    });
  }

  @override
  Future<ZegoMediaPlayerState> getCurrentState() async {
    int state = await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerGetCurrentState', {'index': _index});

    return ZegoMediaPlayerState.values[state];
  }

  @override
  int getIndex() {
    return _index;
  }

  @override
  Future<void> enableAccurateSeek(
      bool enable, ZegoAccurateSeekConfig config) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerEnableAccurateSeek', {
      'index': _index,
      'enable': enable,
      'config': {'timeout': config.timeout}
    });
  }

  @override
  Future<ZegoNetWorkResourceCache> getNetWorkResourceCache() async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerGetNetWorkResourceCache', {'index': _index});

    return ZegoNetWorkResourceCache(map['time'], map['size']);
  }

  @override
  Future<void> setNetWorkBufferThreshold(int threshold) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetNetWorkBufferThreshold',
        {'index': _index, 'threshold': threshold});
  }

  @override
  Future<void> setNetWorkResourceMaxCache(int time, int size) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetNetWorkResourceMaxCache',
        {'index': _index, 'time': time, 'size': size});
  }

  @override
  Future<void> setPlaySpeed(double speed) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetPlaySpeed', {'index': _index, 'speed': speed});
  }

  @override
  Future<ZegoMediaPlayerTakeSnapshotResult> takeSnapshot() async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerTakeSnapshot', {'index': _index});

    return ZegoMediaPlayerTakeSnapshotResult(map['errorCode'],
        image: map['image'] != null ? MemoryImage(map['image']) : null);
  }

  @override
  Future<void> enableFrequencySpectrumMonitor(
      bool enable, int millisecond) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerEnableFrequencySpectrumMonitor',
        {'index': _index, 'enable': enable, 'millisecond': millisecond});
  }

  @override
  Future<void> enableSoundLevelMonitor(bool enable, int millisecond) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerEnableSoundLevelMonitor',
        {'index': _index, 'enable': enable, 'millisecond': millisecond});
  }

  @override
  Future<ZegoMediaPlayerLoadResourceResult>
      loadCopyrightedMusicResourceWithPosition(
          String resourceID, int startPosition) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('mediaPlayerLoadCopyrightedMusicResourceWithPosition', {
      'index': _index,
      'resourceID': resourceID,
      'startPosition': startPosition
    });
    return ZegoMediaPlayerLoadResourceResult(map['errorCode']);
  }

  @override
  Future<void> clearView() {
    return ZegoExpressImpl._channel.invokeMethod('mediaPlayerClearView', {
      'index': _index,
    });
  }

  @override
  Future<void> setActiveAudioChannel(ZegoMediaPlayerAudioChannel audioChannel) {
    return ZegoExpressImpl._channel.invokeMethod(
        'mediaPlayerSetActiveAudioChannel',
        {'index': _index, 'audioChannel': audioChannel.index});
  }

  // @override
  // Future<void> setAudioTrackMode(ZegoMediaPlayerAudioTrackMode mode) {
  //   return ZegoExpressImpl._channel.invokeMethod(
  //       'mediaPlayerSetAudioTrackMode', {'index': _index, 'mode': mode.index});
  // }

  // @override
  // Future<void> setAudioTrackPublishIndex(int index) {
  //   return ZegoExpressImpl._channel.invokeMethod(
  //       'mediaPlayerSetAudioTrackPublishIndex',
  //       {'index': _index, 'index_': index});
  // }
}

class ZegoAudioEffectPlayerImpl extends ZegoAudioEffectPlayer {
  final int _index;

  ZegoAudioEffectPlayerImpl(int index) : _index = index;

  @override
  Future<void> start(int audioEffectID,
      {String? path, ZegoAudioEffectPlayConfig? config}) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerStart', {
      'index': _index,
      'audioEffectID': audioEffectID,
      'path': path ?? '',
      'config': config != null
          ? {'playCount': config.playCount, 'isPublishOut': config.isPublishOut}
          : {}
    });
  }

  @override
  Future<void> stop(int audioEffectID) async {
    return await ZegoExpressImpl._channel.invokeMethod('audioEffectPlayerStop',
        {'index': _index, 'audioEffectID': audioEffectID});
  }

  @override
  Future<void> pause(int audioEffectID) async {
    return await ZegoExpressImpl._channel.invokeMethod('audioEffectPlayerPause',
        {'index': _index, 'audioEffectID': audioEffectID});
  }

  @override
  Future<void> resume(int audioEffectID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'audioEffectPlayerResume',
        {'index': _index, 'audioEffectID': audioEffectID});
  }

  @override
  Future<void> stopAll() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerStopAll', {'index': _index});
  }

  @override
  Future<void> pauseAll() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerPauseAll', {'index': _index});
  }

  @override
  Future<void> resumeAll() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerResumeAll', {'index': _index});
  }

  @override
  Future<ZegoAudioEffectPlayerSeekToResult> seekTo(
      int audioEffectID, int millisecond) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerSeekTo', {
      'index': _index,
      'audioEffectID': audioEffectID,
      'millisecond': millisecond
    });

    return ZegoAudioEffectPlayerSeekToResult(map['errorCode']);
  }

  @override
  Future<void> setVolume(int audioEffectID, int volume) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'audioEffectPlayerSetVolume',
        {'index': _index, 'audioEffectID': audioEffectID, 'volume': volume});
  }

  @override
  Future<void> setVolumeAll(int volume) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'audioEffectPlayerSetVolumeAll', {'index': _index, 'volume': volume});
  }

  @override
  Future<int> getTotalDuration(int audioEffectID) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerGetTotalDuration', {
      'index': _index,
      'audioEffectID': audioEffectID,
    });
  }

  @override
  Future<int> getCurrentProgress(int audioEffectID) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerGetCurrentProgress', {
      'index': _index,
      'audioEffectID': audioEffectID,
    });
  }

  @override
  Future<ZegoAudioEffectPlayerLoadResourceResult> loadResource(
      int audioEffectID, String path) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerLoadResource',
            {'index': _index, 'audioEffectID': audioEffectID, 'path': path});

    return ZegoAudioEffectPlayerLoadResourceResult(map['errorCode']);
  }

  @override
  Future<void> unloadResource(int audioEffectID) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('audioEffectPlayerUnloadResource', {
      'index': _index,
      'audioEffectID': audioEffectID,
    });
  }

  @override
  int getIndex() {
    return _index;
  }

  @override
  Future<void> setPlaySpeed(int audioEffectID, double speed) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'audioEffectPlayerSetPlaySpeed',
        {'index': _index, 'audioEffectID': audioEffectID, 'speed': speed});
  }
}

class ZegoRangeAudioImpl extends ZegoRangeAudio {
  @override
  Future<void> enableMicrophone(bool enable) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioEnableMicrophone', {'enable': enable});
  }

  @override
  Future<void> enableSpatializer(bool enable) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioEnableSpatializer', {'enable': enable});
  }

  @override
  Future<void> enableSpeaker(bool enable) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioEnableSpeaker', {'enable': enable});
  }

  @override
  Future<void> setAudioReceiveRange(double range) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioSetAudioReceiveRange', {'range': range});
  }

  @override
  Future<void> setRangeAudioMode(ZegoRangeAudioMode mode) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioSetMode', {'mode': mode.index});
  }

  @override
  Future<void> setTeamID(String teamID) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioSetTeamID', {'teamID': teamID});
  }

  @override
  Future<void> updateAudioSource(String userID, Float32List position) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'rangeAudioUpdateAudioSource',
        {'userID': userID, 'position': position});
  }

  @override
  Future<void> updateSelfPosition(Float32List position, Float32List axisForward,
      Float32List axisRight, Float32List axisUp) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioUpdateSelfPosition', {
      'position': position,
      'axisForward': axisForward,
      'axisRight': axisRight,
      'axisUp': axisUp
    });
  }

  @override
  Future<void> muteUser(String userID, bool mute) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioMuteUser', {'userID': userID, 'mute': mute});
  }

  @override
  Future<void> setPositionUpdateFrequency(int frequency) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'rangeAudioSetPositionUpdateFrequency', {'frequency': frequency});
  }

  @override
  Future<void> setRangeAudioVolume(int volume) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('rangeAudioSetRangeAudioVolume', {'volume': volume});
  }

  @override
  Future<void> setStreamVocalRange(String streamID, double vocalRange) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'rangeAudioSetStreamVocalRange',
        {'streamID': streamID, 'vocalRange': vocalRange});
  }

  @override
  Future<void> updateStreamPosition(
      String streamID, Float32List position) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'rangeAudioUpdateStreamPosition',
        {'streamID': streamID, 'position': position});
  }
}

class ZegoRealTimeSequentialDataManagerImpl
    extends ZegoRealTimeSequentialDataManager {
  final int _index;

  ZegoRealTimeSequentialDataManagerImpl(int index) : _index = index;

  @override
  int getIndex() {
    return _index;
  }

  @override
  Future<ZegoRealTimeSequentialDataSentResult> sendRealTimeSequentialData(
      Uint8List data, String streamID) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('dataManagerSendRealTimeSequentialData',
            {'index': _index, 'data': data, 'streamID': streamID});

    return ZegoRealTimeSequentialDataSentResult(map['errorCode']);
  }

  @override
  Future<void> startBroadcasting(String streamID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'dataManagerStartBroadcasting',
        {'index': _index, 'streamID': streamID});
  }

  @override
  Future<void> startSubscribing(String streamID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'dataManagerStartSubscribing', {'index': _index, 'streamID': streamID});
  }

  @override
  Future<void> stopBroadcasting(String streamID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'dataManagerStopBroadcasting', {'index': _index, 'streamID': streamID});
  }

  @override
  Future<void> stopSubscribing(String streamID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'dataManagerStopSubscribing', {'index': _index, 'streamID': streamID});
  }
}

class ZegoCopyrightedMusicImpl extends ZegoCopyrightedMusic {
  @override
  Future<void> clearCache() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicClearCache');
  }

  @override
  Future<ZegoCopyrightedMusicDownloadResult> download(String resourceID) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicDownload', {'resourceID': resourceID});
    return ZegoCopyrightedMusicDownloadResult(map['errorCode']);
  }

  @override
  Future<int> getAverageScore(String resourceID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'copyrightedMusicGetAverageScore', {'resourceID': resourceID});
  }

  @override
  Future<int> getCacheSize() async {
    return await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicGetCacheSize');
  }

  @override
  Future<int> getCurrentPitch(String resourceID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'copyrightedMusicGetCurrentPitch', {'resourceID': resourceID});
  }

  @override
  Future<int> getDuration(String resourceID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'copyrightedMusicGetDuration', {'resourceID': resourceID});
  }

  @override
  Future<ZegoCopyrightedMusicGetKrcLyricByTokenResult> getKrcLyricByToken(
      String krcToken) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod(
            'copyrightedMusicGetKrcLyricByToken', {'krcToken': krcToken});
    return ZegoCopyrightedMusicGetKrcLyricByTokenResult(
        map['errorCode'], map['lyrics']);
  }

  @override
  Future<ZegoCopyrightedMusicGetLrcLyricResult> getLrcLyric(
      String songID) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicGetLrcLyric', {'songID': songID});
    return ZegoCopyrightedMusicGetLrcLyricResult(
        map['errorCode'], map['lyrics']);
  }

  @override
  Future<ZegoCopyrightedMusicGetMusicByTokenResult> getMusicByToken(
      String shareToken) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod(
            'copyrightedMusicGetMusicByToken', {'shareToken': shareToken});
    return ZegoCopyrightedMusicGetMusicByTokenResult(
        map['errorCode'], map['resource']);
  }

  @override
  Future<int> getPreviousScore(String resourceID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'copyrightedMusicGetPreviousScore', {'resourceID': resourceID});
  }

  @override
  Future<ZegoCopyrightedMusicGetStandardPitchResult> getStandardPitch(
      String resourceID) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod(
            'copyrightedMusicGetStandardPitch', {'resourceID': resourceID});
    return ZegoCopyrightedMusicGetStandardPitchResult(
        map['errorCode'], map['pitch']);
  }

  @override
  Future<int> getTotalScore(String resourceID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'copyrightedMusicGetTotalScore', {'resourceID': resourceID});
  }

  @override
  Future<ZegoCopyrightedMusicInitResult> initCopyrightedMusic(
      ZegoCopyrightedMusicConfig config) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicInitCopyrightedMusic', {
      'config': {
        'user': {'userID': config.user.userID, 'userName': config.user.userName}
      }
    });
    return ZegoCopyrightedMusicInitResult(map['errorCode']);
  }

  @override
  Future<int> pauseScore(String resourceID) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicPauseScore', {'resourceID': resourceID});
  }

  @override
  Future<bool> queryCache(String songID, ZegoCopyrightedMusicType type) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'copyrightedMusicQueryCache', {'songID': songID, 'type': type.index});
  }

  @override
  Future<ZegoCopyrightedMusicRequestAccompanimentResult> requestAccompaniment(
      ZegoCopyrightedMusicRequestConfig config) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicRequestAccompaniment', {
      'config': {'songID': config.songID, 'mode': config.mode.index}
    });
    return ZegoCopyrightedMusicRequestAccompanimentResult(
        map['errorCode'], map['resource']);
  }

  @override
  Future<ZegoCopyrightedMusicRequestAccompanimentClipResult>
      requestAccompanimentClip(ZegoCopyrightedMusicRequestConfig config) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicRequestAccompanimentClip', {
      'config': {'songID': config.songID, 'mode': config.mode.index}
    });
    return ZegoCopyrightedMusicRequestAccompanimentClipResult(
        map['errorCode'], map['resource']);
  }

  @override
  Future<ZegoCopyrightedMusicRequestSongResult> requestSong(
      ZegoCopyrightedMusicRequestConfig config) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicRequestSong', {
      'config': {'songID': config.songID, 'mode': config.mode.index}
    });
    return ZegoCopyrightedMusicRequestSongResult(
        map['errorCode'], map['resource']);
  }

  @override
  Future<int> resetScore(String resourceID) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicResetScore', {'resourceID': resourceID});
  }

  @override
  Future<int> resumeScore(String resourceID) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'copyrightedMusicResumeScore', {'resourceID': resourceID});
  }

  @override
  Future<ZegoCopyrightedMusicSendExtendedRequestResult> sendExtendedRequest(
      String command, String params) async {
    final Map<dynamic, dynamic> map = await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicSendExtendedRequest',
            {'command': command, 'params': params});
    return ZegoCopyrightedMusicSendExtendedRequestResult(
        map['errorCode'], map['command'], map['result']);
  }

  @override
  Future<int> startScore(String resourceID, int pitchValueInterval) async {
    return await ZegoExpressImpl._channel.invokeMethod(
        'copyrightedMusicStartScore',
        {'resourceID': resourceID, 'pitchValueInterval': pitchValueInterval});
  }

  @override
  Future<int> stopScore(String resourceID) async {
    return await ZegoExpressImpl._channel
        .invokeMethod('copyrightedMusicStopScore', {'resourceID': resourceID});
  }

  // @override
  // Future<int> getFullScore(String resourceID) async {
  //   return await ZegoExpressImpl._channel.invokeMethod(
  //       'copyrightedMusicGetFullScore', {'resourceID': resourceID});
  // }

  // @override
  // Future<ZegoCopyrightedMusicGetSharedResourceResult> getSharedResource(
  //     ZegoCopyrightedMusicGetSharedConfig config,
  //     ZegoCopyrightedMusicResourceType type) async {
  //   var resultMap = await ZegoExpressImpl._channel
  //       .invokeMethod('copyrightedMusicGetSharedResource', {
  //     'config': {'songID': config.songID},
  //     'type': type.index
  //   });
  //   return ZegoCopyrightedMusicGetSharedResourceResult(
  //       resultMap['errorCode'], resultMap['resource']);
  // }

  // @override
  // Future<ZegoCopyrightedMusicRequestResourceResult> requestResource(
  //     ZegoCopyrightedMusicRequestConfig config,
  //     ZegoCopyrightedMusicResourceType type) async {
  //   var resultMap = await ZegoExpressImpl._channel
  //       .invokeMethod('copyrightedMusicRequestResource', {
  //     'config': {'songID': config.songID, 'mode': config.mode.index},
  //     'type': type.index
  //   });
  //   return ZegoCopyrightedMusicRequestResourceResult(
  //       resultMap['errorCode'], resultMap['resource']);
  // }
}
