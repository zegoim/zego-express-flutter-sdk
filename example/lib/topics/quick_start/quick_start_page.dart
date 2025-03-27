//
//  quick_start_page.dart
//  zego-express-example-topics-flutter
//
//  Created by Patrick Fu on 2020/12/04.
//  Copyright © 2020 Zego. All rights reserved.
//
import 'dart:async';
import 'dart:convert';

import 'package:flutter/cupertino.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:zego_express_engine/zego_express_engine.dart';
import 'package:zego_express_engine_example/topics/quick_start/minio.dart';
import 'package:zego_express_engine_example/utils/zego_config.dart';
import 'package:universal_io/io.dart';
import 'dart:typed_data';
import 'dart:js' as js;
import 'dart:js_util';
import 'dart:html' as html;
import 'package:js/js.dart';
import 'dart:convert';
import 'dart:ui' as ui;

@JS('JSON.parse')
external dynamic parseJSON(String str);
@JS('JSON.stringify')
external String stringifyJSON(dynamic obj);

dynamic dartObjToJSON(dynamic obj) {
  String str = jsonEncode(obj);
  return parseJSON(str);
}

Future<String> downloadVideo(String url) async {
  try {
    // 使用 http 包下载视频
    final promise = callMethod(js.context['window'], 'fetch', [url]);
    final jsResponse = await promiseToFuture(promise);
    final isOk = getProperty(jsResponse, 'ok');
    // print(' fff 下jsResponse: ${jsResponse} ${isOk}');

    if (isOk) {
      final blobPromise = callMethod(jsResponse, 'blob', []);
      // print(' fff blob: ${blobPromise}');
      final videoBlob = await promiseToFuture(blobPromise);
      // final urlObj = js.context['URL'];
      // print(' fff videoBlob: ${videoBlob} ');
      final blobUrl = html.Url.createObjectUrlFromBlob(videoBlob);
      // print(' fff 下载成功，状态码: ${blobUrl}');
      return blobUrl;
    }
    return '';
  } catch (e) {
    print('fff 发生错误: $e');
    return '';
  }
}

dynamic stringToEnum(List list, int? value) {
  if (value == null) {
    return null;
  }
  if (value >= 0 && value < list.length) {
    return list[value];
  }
  return null;
}

Future<ZegoMixerStartResult> startMixerTask(dynamic params) async {
  Map<String, dynamic> taskMap = params;
  var task = ZegoMixerTask(taskMap['taskID']);
  var bitrate = taskMap['audioConfig']['bitrate'];
  task.audioConfig = ZegoMixerAudioConfig(
    bitrate,
    stringToEnum(ZegoAudioChannel.values, taskMap['audioConfig']['channel']),
    stringToEnum(ZegoAudioCodecID.values, taskMap['audioConfig']['codecID']),
    stringToEnum(ZegoAudioMixMode.values, taskMap['audioConfig']['mixMode']),
  );

  /// The audio config of the task
  task.videoConfig = ZegoMixerVideoConfig(
    taskMap['videoConfig']['width'],
    taskMap['videoConfig']['height'],
    taskMap['videoConfig']['fps'],
    taskMap['videoConfig']['bitrate'],
    taskMap['videoConfig']['quality'],
    stringToEnum(ZegoVideoRateControlMode.values,
        taskMap['videoConfig']['rateControlMode']),
  );

  for (Map<String, dynamic> item in taskMap['inputList']) {
    ZegoLabelInfo? label;
    if (item.containsKey('label')) {
      label = ZegoLabelInfo(
          item['label']['text'],
          item['label']['left'],
          item['label']['top'],
          ZegoFontStyle(
              stringToEnum(ZegoFontType.values, item['label']['font']['type']),
              item['label']['font']['size'],
              item['label']['font']['color'],
              item['label']['font']['transparency'],
              item['label']['font']['border'],
              item['label']['font']['borderColor']));
    }

    ZegoMixerImageInfo? imageInfo;
    if (item.containsKey('imageInfo')) {
      imageInfo = ZegoMixerImageInfo(item['imageInfo']['url'],
          displayMode: item['imageInfo']['displayMode']);
    }

    task.inputList.add(
      ZegoMixerInput(
          item['streamID'],
          stringToEnum(ZegoMixerInputContentType.values, item['contentType']),
          Rect.fromLTRB(
              double.parse(item['layout']['left'].toString()),
              double.parse(item['layout']['top'].toString()),
              double.parse(item['layout']['right'].toString()),
              double.parse(item['layout']['bottom'].toString())),
          item['soundLevelID'],
          item['volume'] ?? 100,
          item['isAudioFocus'],
          item['audioDirection'],
          label: label,
          renderMode:
              stringToEnum(ZegoMixRenderMode.values, item['renderMode']),
          imageInfo: imageInfo,
          cornerRadius: item['cornerRadius'],
          advancedConfig: item['advancedConfig']),
    );
  }

  for (Map<String, dynamic> item in taskMap['outputList']) {
    ZegoMixerOutputVideoConfig? videoConfig;
    if (item.containsKey('videoConfig')) {
      videoConfig = ZegoMixerOutputVideoConfig(
          stringToEnum(
              ZegoVideoCodecID.values, item['videoConfig']['videoCodecID']),
          item['videoConfig']['bitrate'],
          encodeProfile: stringToEnum(
              ZegoEncodeProfile.values, item['videoConfig']['encodeProfile']),
          encodeLatency: item['videoConfig']['encodeLatency'],
          enableLowBitrateHD: item['videoConfig']['enableLowBitrateHD']);
    }
    task.outputList
        .add(ZegoMixerOutput(item['target'], videoConfig: videoConfig));
  }

  if (taskMap.containsKey('watermark')) {
    task.watermark = ZegoWatermark(
        taskMap['watermark']['imageURL'],
        Rect.fromLTRB(
            double.parse(taskMap['watermark']['layout']['left'].toString()),
            double.parse(taskMap['watermark']['layout']['top'].toString()),
            double.parse(taskMap['watermark']['layout']['right'].toString()),
            double.parse(taskMap['watermark']['layout']['bottom'].toString())));
  }
  task.backgroundImageURL = taskMap['backgroundImageURL'];

  task.backgroundColor = taskMap['backgroundColor'];
  task.enableSoundLevel = taskMap['enableSoundLevel'];
  if (taskMap.containsKey('advancedConfig')) {
    Map<String, dynamic> advancedConfig = taskMap['advancedConfig'];
    task.advancedConfig =
        advancedConfig.map((key, value) => MapEntry(key, value!.toString()));
  }
  if (taskMap.containsKey('streamAlignmentMode')) {
    task.streamAlignmentMode = stringToEnum(
        ZegoStreamAlignmentMode.values, taskMap['streamAlignmentMode']);
  }
  if (taskMap.containsKey('userData')) {
    task.userData = base64Decode(taskMap['userData'].toString());
  }
  if (taskMap.containsKey('minPlayStreamBufferLength')) {
    task.minPlayStreamBufferLength = taskMap['minPlayStreamBufferLength'];
  }
  print('🚀 ffflutter startMixerTask 开始');
  final res = await ZegoExpressEngine.instance.startMixerTask(task);
  print('🚀 ffflutter startMixerTask ${res.errorCode} ${res.extendedData}');
  return res;
  // .then((result) {
  //   var resultMap = {
  //     'errorCode': result.errorCode,
  //     'extendedData': result.extendedData
  //   };
  //   var contentMap = {'eventName': 'onMixerStartResult', 'params': resultMap};
  //   var rsp = RspData.content(msgType: 1, content: jsonEncode(contentMap));
  // });

  // var ret = RspData(msgType: 2, requestID: params.requestID, ret: true);
  // return ret;
}

class QuickStartPage extends StatefulWidget {
  @override
  _QuickStartPageState createState() => _QuickStartPageState();
}

class _QuickStartPageState extends State<QuickStartPage> {
  final String _roomID = '322';

  int _previewViewID = -1;
  int _playViewID = -1;
  Widget? _previewViewWidget;
  Widget? _playViewWidget;
  static const double viewRatio = 3.0 / 4.0;

  ZegoMediaPlayer? mediaPlayer;
  Widget? _mediaplayerWidget;

  bool _isEngineActive = false;
  ZegoRoomState _roomState = ZegoRoomState.Disconnected;
  ZegoPublisherState _publisherState = ZegoPublisherState.NoPublish;
  ZegoPlayerState _playerState = ZegoPlayerState.NoPlay;

  TextEditingController _publishingStreamIDController =
      new TextEditingController();
  TextEditingController _playingStreamIDController =
      new TextEditingController();

  @override
  void initState() {
    super.initState();
    ZegoExpressEngine.getVersion()
        .then((value) => print('🌞 SDK Version: $value'));

    setZegoEventCallback();
  }

  @override
  void dispose() {
    // Can destroy the engine when you don't need audio and video calls
    //
    // Destroy engine will automatically logout room and stop publishing/playing stream.
    ZegoExpressEngine.destroyEngine()
        .then((value) => print('async destroy success'));

    print('🏳️ Destroy ZegoExpressEngine');

    clearZegoEventCallback();

    super.dispose();
  }

  // MARK: - Step 1: CreateEngine

  void createEngine() {
    // ZegoExpressEngine.setRoomMode(ZegoRoomMode.MultiRoom);
    final map = {
      'av_retry_time': '5',
      'room_retry_time': '5',
      // 'unregister_sei_filter': 'zegozegozego',
      'emulation_prevention_byte': 'false'
    };
    final config = ZegoEngineConfig(advancedConfig: map);
    ZegoExpressEngine.setEngineConfig(config);
    ZegoEngineProfile profile = ZegoEngineProfile(
        ZegoConfig.instance.appID, ZegoConfig.instance.scenario,
        enablePlatformView: ZegoConfig.instance.enablePlatformView,
        appSign: ZegoConfig.instance.appSign);
    if (kIsWeb) {
      profile.appSign = null; // Don't use appsign on web
    }
    ZegoExpressEngine.setLogConfig(
        new ZegoLogConfig("", 1024, logLevel: "info"));
    ZegoExpressEngine.createEngineWithProfile(profile);
    // 采用 H.264 的 SEI (nalu type = 6,payload type = 5) 类型打包，因为视频编码器自身会产生 payload type 为 5 的 SEI，或者使用视频文件推流时，视频文件中也可能存在这样的 SEI，所以使用此类型时，用户需要把 uuid + content 当作 buffer 塞给 SEI 发送接口；此时为了区别视频编码器自身产生的 SEI， App 在发送此类型 SEI 时，可以填写业务特定的 uuid（uuid 长度为 16 字节），接收方使用 SDK 解析 payload type 为 5 的 SEI 时，会根据设置的过滤字符串过滤出 uuid 相符的 SEI 抛给业务，如果没有设置过滤字符串，SDK 会把所有收到的 SEI 都抛给开发者。
    var seiConfig = ZegoSEIConfig(ZegoSEIType.ZegoDefined);
    ZegoExpressEngine.instance.setSEIConfig(seiConfig);

    // Notify View that engine state changed
    setState(() => _isEngineActive = true);

    print('🚀 Create ZegoExpressEngine');
  }

  // MARK: - Step 2: LoginRoom

  void loginRoom() async {
    // Instantiate a ZegoUser object
    ZegoUser user = ZegoUser(
        ZegoConfig.instance.userID,
        ZegoConfig.instance.userName.isEmpty
            ? ZegoConfig.instance.userID
            : ZegoConfig.instance.userName);

    if (kIsWeb) {
      final inConfig = await ZegoExpressEngine.instance.getVideoConfig();
      // final jsObj = dartObjToJSON(inConfig);
      print(' fff 📥 setVideoConfig, streamID: ${inConfig.encodeWidth}');

      var audioConfig1 =
          ZegoAudioConfig.preset(ZegoAudioConfigPreset.HighQualityStereo);
      ZegoExpressEngine.instance.setAudioConfig(audioConfig1);

      final audioConfig = await ZegoExpressEngine.instance
          .getAudioConfig(channel: ZegoPublishChannel.Aux);
      print(
          ' fff 📥 getAudioConfig, birate: ${audioConfig.bitrate}, ${audioConfig.channel}');

      ZegoRoomConfig config = ZegoRoomConfig.defaultConfig();
      config.token = ZegoConfig.instance.token;
      // config.isEnableAudioFilter = false;
      config.maxMemberCount = 10;
      config.isUserStatusNotify = true;

      // Login Room WEB only supports token;
      ZegoRoomLoginResult result = await ZegoExpressEngine.instance
          .loginRoom(_roomID, user, config: config);
      print('🚪 ffflutter login room result: $_roomID ${result.errorCode}');
      ZegoCopyrightedMusic? copyMusic =
          await ZegoExpressEngine.instance.createCopyrightedMusic();
      print('🚪 ffflutter createCopyrightedMusic');
      if (copyMusic != null) {
        ZegoExpressEngine.onMediaPlayerStateUpdate =
            (mediaPlayer, state, errorCode) {
          print(
              '🚩 🚪 ffflutter onMediaPlayerStateUpdate, state: $state, errorCode: $errorCode, player: $mediaPlayer');
          if (state == ZegoMediaPlayerState.Pausing) {}
          if (state == ZegoMediaPlayerState.NoPlay) {}
        };
        ZegoExpressEngine.onMediaPlayerPlayingProgress = (mediaPlayer, ms) {
          print(
              '🚩 🚪 ffflutter onMediaPlayerProgerssUpdate, ms: $ms now: ${DateTime.now().millisecondsSinceEpoch}');
        };
        ZegoCopyrightedMusicConfig initConfig =
            new ZegoCopyrightedMusicConfig(user);
        print(
            'ffflutter initCopyrightedMusic start: ${initConfig.user.userID}');
        ZegoCopyrightedMusicInitResult initRes =
            await copyMusic.initCopyrightedMusic(initConfig);
        int code = initRes.errorCode;
        // print('ffflutter initCopyrightedMusic result: $_roomID $code');

        // ZegoCopyrightedMusicSendExtendedRequestResult extentdResp =
        //     await copyMusic.sendExtendedRequest('/search/song',
        //         '{"keyword":"晴天","page":1,"filter":[1],"vendor_id":1}');
        // int extendCode = extentdResp.errorCode;
        // String content = extentdResp.result;
        // print('🚪 ffflutter sendExtendedRequest resp: $extendCode $content');

        final config = ZegoCopyrightedMusicGetSharedConfig('147175',
            mode: 1,
            vendorID: ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendor2);
        final resp = await copyMusic.getSharedResource(
            config,
            ZegoCopyrightedMusicResourceType
                .ZegoCopyrightedMusicResourceAccompaniment);

        print(
            '🚪 ffflutter getSharedResource resp: ${resp.errorCode} ${resp.resource}');

        // final reqResConfig = ZegoCopyrightedMusicRequestConfigV2('147175', 0,
        //     vendorID: 2, resourceType: 0, masterID: "");

        // final requestResourceResult = await copyMusic.requestResourceV2(
        //   reqResConfig,
        // );
        // print(
        //     '🚪 ffflutter requestResource resp: ${requestResourceResult.errorCode} ${requestResourceResult.resource}');

        // final downloadResp = await copyMusic.download('z_2_147175_1_normal');
        // print(     '🚪 ffflutter downloadResult resp: ${downloadResp.errorCode}');
        // for (var i = 0; i < 5; i++) {
        //   ZegoMediaPlayer? mediaPlayer =
        //       await ZegoExpressEngine.instance.createMediaPlayer();
        //   print(
        //       'fff createMediaPlayer ${mediaPlayer}');
        // }

        // var playCode = -1;
        // if (mediaPlayer != null) {
        //   final mediaplayerWidget =
        //       await ZegoExpressEngine.instance.createCanvasView((viewID) async {
        //     print('🚪 ffflutter createCanvasView resp: ${viewID}');
        //     await mediaPlayer.setPlayerCanvas(ZegoCanvas(viewID,
        //         alphaBlend: false,
        //         viewMode: ZegoViewMode.AspectFit,
        //         backgroundColor: 0));
        //     print('🚪 ffflutter setPlayerCanvas resp: ${viewID}');
        //     // final loadResp =
        //     //     await mediaPlayer.loadCopyrightedMusicResourceWithPosition(
        //     //         'z_2_147175_1_normal', 0);
        //     final loadResp =
        //         await mediaPlayer.loadResource("http://zego-public.oss-cn-shanghai.aliyuncs.com/videotest/1-av_codec_error.mkv");
        //     playCode = loadResp.errorCode;

        //     print('🚪 ffflutter start: ${viewID}');
        //     mediaPlayer.start();
        //     // Future.delayed(Duration(seconds: 5), () {
        //     //   // mediaPlayer.setAudioTrackIndex(1);
        //     //   print('🚪 ffflutter pause: ${viewID}');
        //     //   mediaPlayer.pause();
        //     //   // print('🚪 ffflutter stop');
        //     //   // mediaPlayer.stop();
        //     //   // print('🚪 ffflutter clear');
        //     //   // copyMusic.clearCache();
        //     //   // ZegoExpressEngine.instance.logoutRoom();

        //     // });
        //     // Future.delayed(Duration(seconds: 40), () {
        //     //   mediaPlayer.setAudioTrackIndex(0);
        //     // });
        //   });
        //   setState(() {
        //     _mediaplayerWidget = mediaplayerWidget;
        //   });
        // }

        // final getKrcLyricByTokenResult = await copyMusic.getLrcLyric(
        //     '33801622512313',
        //     vendorID: ZegoCopyrightedMusicVendorID.ZegoCopyrightedMusicVendor1);
        // print(
        //     '🚪 ffflutter getKrcLyricByToken resp: ${getKrcLyricByTokenResult.lyrics}');

        // final getKrcLyricByTokenResult = await copyMusic
        //     .getKrcLyricByToken('WwQGAwNVNzVkYjZmNmY3NQMhfwQwNzFlEFA=');
        // print(
        //     '🚪 ffflutter getKrcLyricByToken resp: ${getKrcLyricByTokenResult.lyrics}');

        // var getStandardPitchResult =
        //     await copyMusic.getStandardPitch('z_1_338016225_2_normal');
        // var getCurrentPitchResult =
        //     await copyMusic.getCurrentPitch('z_1_338016225_2_normal');

        //  copyMusic.setScoringLevel(3);
        // print(
        //     '🚪 ffflutter getStandardPitchResult resp: ${getStandardPitchResult.errorCode} ${getCurrentPitchResult}');
      }

      // final url = await downloadVideo(
      //     'https://zego-public.oss-cn-shanghai.aliyuncs.com/videotest/ad.mp4');
      // ZegoMediaPlayer? mediaPlayer =
      //     await ZegoExpressEngine.instance.createMediaPlayer();
      // if (mediaPlayer != null) {
      //   // 加载资源，可传本地资源的绝对路径或者网络资源的 URL
      //   final mediaplayerWidget =
      //       await ZegoExpressEngine.instance.createCanvasView((viewID) {
      //     mediaPlayer.setPlayerCanvas(ZegoCanvas(viewID,
      //         alphaBlend: false,
      //         viewMode: ZegoViewMode.AspectFit,
      //         backgroundColor: 0));
      //     mediaPlayer
      //         // .loadResource("https://zego-public.oss-cn-shanghai.aliyuncs.com/videotest/%E6%B1%9F%E5%8D%97StyleMV(1920x1080%2016.9).rmvb")
      //         // .loadResource("http://zego-public.oss-cn-shanghai.aliyuncs.com/videotest/1-av_codec_error.flv")
      //         .loadResource(
      //             "https://zego-public.oss-cn-shanghai.aliyuncs.com/sdk-doc/assets/big_buck_bunny.mp4")
      //         .then((ZegoMediaPlayerLoadResourceResult result) {
      //       print('ffflutter loadResource result: ${result.errorCode}');
      //       if (result.errorCode == 0) {
      //         // loadResource success
      //       } else {
      //         // loadResource errorcode: errorcode
      //       }
      //     });
      //     mediaPlayer.setPlaySpeed(4);
      //     mediaPlayer.start();
      //     print('ffflutter start play');
      //   });
      //   setState(() {
      //     _mediaplayerWidget = mediaplayerWidget;
      //   });
      // }
    } else {
      // Login Room
      ZegoExpressEngine.instance.loginRoom(_roomID, user);
    }

    print('🚪 Start login room, roomID: $_roomID');
  }

  void logoutRoom() async {
    // Logout room will automatically stop publishing/playing stream.
    //
    // But directly logout room without destroying the [PlatformView]
    // or [TextureRenderer] may cause a memory leak.
    final result = await ZegoExpressEngine.instance.logoutRoom(_roomID);
    print(
        '🚪 logout room, roomID: $_roomID ${result.errorCode} ${result.extendedData}');

    clearPreviewView();
    clearPlayView();
  }

  // MARK: - Step 3: StartPublishingStream
  void startPreview() {
    Future<void> _startPreview(int viewID) async {
      ZegoCanvas canvas = ZegoCanvas.view(viewID);
      await ZegoExpressEngine.instance.startPreview(canvas: canvas);
      print('🔌 Start preview, viewID: $viewID');
    }

    if (Platform.isIOS ||
        Platform.isAndroid ||
        Platform.isWindows ||
        Platform.isMacOS ||
        Platform.isLinux ||
        kIsWeb) {
      ZegoExpressEngine.instance.createCanvasView((viewID) {
        _previewViewID = viewID;
        _startPreview(viewID);
      }).then((widget) {
        setState(() {
          _previewViewWidget = widget;
        });
      });
    } else {
      ZegoExpressEngine.instance.startPreview();
    }
  }

  void stopPreview() {
    if (!Platform.isAndroid &&
        !Platform.isIOS &&
        !Platform.isMacOS &&
        !kIsWeb) {
      return;
    }

    if (_previewViewWidget == null) {
      return;
    }

    ZegoExpressEngine.instance.stopPreview();
    clearPreviewView();
  }

  void startPublishingStream(String streamID) async {
    // final config = new ZegoVideoConfig(
    //     1920, 1080, 1920, 1080, 25, 3000, ZegoVideoCodecID.Default,
    //     keyFrameInterval: 2);
    // await ZegoExpressEngine.instance.enableHardwareEncoder(true);
    // await ZegoExpressEngine.instance.setVideoConfig(config);
    // final inConfig = await ZegoExpressEngine.instance.getVideoConfig();
    // final jsObj = dartObjToJSON(inConfig);
    // print(' fff 📥 setVideoConfig, streamID: ${inConfig.encodeWidth}');

    await ZegoExpressEngine.instance.mutePublishStreamVideo(true);

    ZegoExpressEngine.instance.startPublishingStream(streamID);
    // Future.delayed(new Duration(seconds: 3), () async {
    //   print(' fff 📥 addPublishCdnUrl, start: $streamID');
    //   var result = await ZegoExpressEngine.instance.addPublishCdnUrl(
    //       streamID, "rtmp://publish-ws-pressure.zego.im/pressure/" + streamID);
    //   var result2 = await ZegoExpressEngine.instance.removePublishCdnUrl(
    //       streamID, "rtmp://publish-ws-pressure.zego.im/pressure/" + streamID);
    //   print(
    //       ' fff 📥 addPublishCdnUrl, : ${result.errorCode}   ${result2.errorCode}');
    //   // ZegoExpressEngine.instance.enableCamera(false);
    //   // print(' fff 📥 mutePublishStreamVideo, streamID: $streamID, false')
    // });

    Timer.periodic(Duration(seconds: 3), (timer) async {
      final config =
          '{"enableSoundLevel":false,"outputList":[{"videoConfig":{"bitrate":600,"videoCodecID":0,"encodeProfile":0,"encodeLatency":0,"enableLowBitrateHD":false},"target":"mix_stream_2024_09_02_17_07_17_529424_79044"}],"backgroundColor":0,"inputList":[{"layout":{"top":0,"left":0,"bottom":320,"x":0,"width":0,"y":0,"right":180,"height":0},"volume":100,"renderMode":0,"streamID":"322","soundLevelID":28045,"contentType":0,"isAudioFocus":false,"audioDirection":-1,"cornerRadius":0}],"minPlayStreamBufferLength":0,"userDataLength":0,"streamAlignmentMode":0,"videoConfig":{"width":360,"fps":15,"bitrate":600,"rateControlMode":0,"height":640,"quality":23},"taskID":"mixer_task_2024_09_02_17_07_17_532170_87602","audioConfig":{"codecID":0,"channel":1,"bitrate":48,"mixMode":0},"backgroundImageURL":""}';
      final res = await startMixerTask(jsonDecode(config));
      print('fff startMixerTask ${res.errorCode}');
      // setState(() {
      //   var seiData = Uint8List.fromList(utf8.encode('zegozegozego12345'));
      //   ZegoExpressEngine.instance.sendSEI(seiData, seiData.length);
      // });
    });
    print('📤 Start publishing stream, streamID: $streamID');
  }

  void stopPublishingStream() {
    ZegoExpressEngine.instance.stopPublishingStream();
  }

  // MARK: - Step 4: StartPlayingStream

  void startPlayingStream(String streamID) async {
    // await ZegoExpressEngine.instance.mutePlayStreamAudio(streamID, true);

    // print(' fff 📥 mute playing stream, streamID: $streamID, true');
    void _startPlayingStream(int viewID, String streamID) async {
      ZegoCanvas canvas =
          new ZegoCanvas(viewID, viewMode: ZegoViewMode.ScaleToFill);
      await ZegoExpressEngine.instance
          .startPlayingStream(streamID, canvas: canvas);
      print(
          ' fff 📥 Start playing stream, streamID: $streamID, viewID: $viewID');

      // Future.delayed(new Duration(seconds: 10), () {
      //   ZegoExpressEngine.instance.mutePlayStreamAudio(streamID, false);

      //   print(' fff 📥 mute playing stream, streamID: $streamID, false');
      // });
      // Future.delayed(new Duration(seconds: 2), () {
      //   ZegoExpressEngine.instance
      //       .takePlayStreamSnapshot(streamID)
      //       .then((result) async {
      //     int width = 0;
      //     int height = 0;
      //     String? imageUrl;
      //     if (result.errorCode == 0 && result.image != null) {
      //       String bucketName = 'snapshots';
      //       String imageSuffix = Platform.isAndroid ||
      //               Platform.isIOS ||
      //               Platform.isMacOS ||
      //               kIsWeb
      //           ? '.jpg'
      //           : '.bmp';
      //       var now = DateTime.now();
      //       String twoDigits(int n) => n.toString().padLeft(2, '0');
      //       String formattedDate =
      //           '${now.year}${twoDigits(now.month)}${twoDigits(now.day)}${twoDigits(now.hour)}${twoDigits(now.minute)}${twoDigits(now.second)}';
      //       String objectName =
      //           '${'666'?.replaceAll('#', '_')}_player_$formattedDate$imageSuffix';
      //       try {
      //         String uploadResult = await MinioHelper.instance.uploadImage(
      //             bucketName,
      //             objectName,
      //             Stream.fromIterable([result.image!.bytes]));
      //         imageUrl =
      //             '${MinioHelper.instance.getUploadBaseUrl()}/$bucketName/$objectName';
      //         print(
      //             'fff upload image. $uploadResult $imageUrl ${result.image!.bytes.length} ${result.image!.scale}');
      //       } catch (e) {
      //         print('fff upload image catch. ${e.toString()}');
      //       }
      //       var completer = Completer();

      //       Future<Map<String, int>> getImageDimensions(Uint8List bytes) async {
      //         // 创建一个 Blob 对象
      //         final blob = html.Blob([bytes]);
      //         // 创建一个 URL
      //         final url = html.Url.createObjectUrlFromBlob(blob);

      //         // 创建一个 ImageElement
      //         final imageElement = html.ImageElement(src: url);

      //         // 返回一个 Future，直到图片加载完成
      //         final completer = Completer<Map<String, int>>();

      //         imageElement.onLoad.listen((event) {
      //           // 获取图像宽高
      //           final width = imageElement.width ?? 0;
      //           final height = imageElement.height ?? 0;

      //           // 释放 URL
      //           html.Url.revokeObjectUrl(url);

      //           // 完成 completer 并返回宽高
      //           completer.complete({'width': width, 'height': height});
      //         });

      //         imageElement.onError.listen((event) {
      //           print('Error loading image');

      //           // 释放 URL
      //           html.Url.revokeObjectUrl(url);

      //           // 完成 completer 以处理错误
      //           completer.completeError('Error loading image');
      //         });

      //         // 设置 src 以开始加载图像
      //         imageElement.src = url;

      //         return completer.future; // 返回 Future<Map<String, int>>
      //       }

      //       Map<String, int> dimensions =
      //           await getImageDimensions(result.image!.bytes);

      //       print(
      //           'fff upload image info: $dimensions.width $dimensions.height');
      //     }
      //   });
      // });
    }

    if (Platform.isIOS ||
        Platform.isAndroid ||
        Platform.isWindows ||
        Platform.isMacOS ||
        Platform.isLinux ||
        kIsWeb) {
      ZegoExpressEngine.instance.createCanvasView((viewID) {
        _playViewID = viewID;
        print(
            ' fff 📥 createCanvasView 111, streamID: $streamID, viewID: $viewID');
        _startPlayingStream(viewID, streamID);
      }).then((widget) {
        setState(() {
          _playViewWidget = widget;
        });
      });
    } else {
      ZegoExpressEngine.instance.startPlayingStream(streamID);
    }
  }

  void stopPlayingStream(String streamID) {
    ZegoExpressEngine.instance.stopPlayingStream(streamID);

    clearPlayView();
  }

  // MARK: - Exit

  void destroyEngine() async {
    stopPreview();
    clearPreviewView();
    clearPlayView();

    // Can destroy the engine when you don't need audio and video calls
    //
    // Destroy engine will automatically logout room and stop publishing/playing stream.
    ZegoExpressEngine.destroyEngine()
        .then((ret) => print('already destroy engine'));

    print('🏳️ Destroy ZegoExpressEngine');

    // Notify View that engine state changed
    setState(() {
      _isEngineActive = false;
      _roomState = ZegoRoomState.Disconnected;
      _publisherState = ZegoPublisherState.NoPublish;
      _playerState = ZegoPlayerState.NoPlay;
    });
  }

  Timer? _timer;
  // MARK: - Zego Event

  void setZegoEventCallback() {
    ZegoExpressEngine.onRoomStateUpdate = (String roomID, ZegoRoomState state,
        int errorCode, Map<String, dynamic> extendedData) {
      print(
          '🚩 🚪 Room state update, state: $state, errorCode: $errorCode, roomID: $roomID');
      setState(() => _roomState = state);
    };
    ZegoExpressEngine.onRoomStateChanged = (String roomID,
        ZegoRoomStateChangedReason state,
        int errorCode,
        Map<String, dynamic> extendedData) {
      print(
          '🚩 🚪 Room state update, state: $state, errorCode: $errorCode, roomID: $roomID');
    };
    ZegoExpressEngine.onLocalDeviceExceptionOccurred =
        (ZegoDeviceExceptionType exceptionType, ZegoDeviceType deviceType,
            String deviceID) {
      print(
          '🚩 🚪 fff onLocalDeviceExceptionOccurred, exceptionType: $exceptionType, deviceType: $deviceType, deviceID: $deviceID');
    };
    ZegoExpressEngine.onDebugError = (int code, String msg, String msg2) {
      print('🚩 🚪 fff onDebugError, code: $code, msg: $msg ; $msg2');
    };
    // ZegoExpressEngine.onPlayerRecvSEI = (String streamID, Uint8List data) {
    //   print('ffflutter onPlayerRecvSEI $streamID $data');
    // };
    ZegoExpressEngine.onPublisherStateUpdate = (String streamID,
        ZegoPublisherState state,
        int errorCode,
        Map<String, dynamic> extendedData) {
      print(
          '🚩 📤 Publisher state update, state: $state, errorCode: $errorCode, streamID: $streamID');
      setState(() => _publisherState = state);
      if (_publisherState == ZegoPublisherState.Publishing) {
        // _timer = Timer.periodic(Duration(seconds: 3), (timer) async {
        //   final config = '{"enableSoundLevel":false,"outputList":[{"videoConfig":{"bitrate":600,"videoCodecID":0,"encodeProfile":0,"encodeLatency":0,"enableLowBitrateHD":false},"target":"mix_stream_2024_09_02_17_07_17_529424_79044"}],"backgroundColor":0,"inputList":[{"layout":{"top":0,"left":0,"bottom":320,"x":0,"width":0,"y":0,"right":180,"height":0},"volume":100,"renderMode":0,"streamID":"322","soundLevelID":28045,"contentType":0,"isAudioFocus":false,"audioDirection":-1,"cornerRadius":0}],"minPlayStreamBufferLength":0,"userDataLength":0,"streamAlignmentMode":0,"videoConfig":{"width":360,"fps":15,"bitrate":600,"rateControlMode":0,"height":640,"quality":23},"taskID":"mixer_task_2024_09_02_17_07_17_532170_87602","audioConfig":{"codecID":0,"channel":1,"bitrate":48,"mixMode":0},"backgroundImageURL":""}';
        //   final res = await startMixerTask(jsonDecode(config));
        //   print('fff startMixerTask ${res.errorCode}');
        //   // setState(() {
        //   //   var seiData = Uint8List.fromList(utf8.encode('zegozegozego12345'));
        //   //   ZegoExpressEngine.instance.sendSEI(seiData, seiData.length);
        //   // });
        // });
      } else {
        _timer?.cancel();
        _timer = null;
      }
    };
    ZegoExpressEngine.onPublisherQualityUpdate =
        (String streamId, ZegoPublishStreamQuality quality) {
      print(
          '🚩 📤 fff onPublisherQualityUpdate, streamid: $streamId, ishard: ${quality.isHardwareEncode}');
    };
    ZegoExpressEngine.onPlayerStateUpdate = (String streamID,
        ZegoPlayerState state,
        int errorCode,
        Map<String, dynamic> extendedData) {
      print(
          '🚩 📥 fff Player state update, state: $state, errorCode: $errorCode, streamID: $streamID');
      setState(() => _playerState = state);
    };

    ZegoExpressEngine.onRoomUserUpdate = (roomID, updateType, userList) {
      userList.forEach((e) {
        var userID = e.userID;
        var userName = e.userName;
        print(
            '🚩 🚪 Room user update, roomID: $roomID, updateType: $updateType userID: $userID userName: $userName');
      });
    };

    ZegoExpressEngine.onRoomStreamUpdate =
        ((roomID, updateType, streamList, extendedData) {
      streamList.forEach((stream) {
        var streamID = stream.streamID;
        print(
            '🚩 🚪 Room stream update, roomID: $roomID, updateType: $updateType streamID:$streamID');

        if (updateType == ZegoPlayerState.NoPlay) {
          stopPlayingStream(_playingStreamIDController.text.trim());
        }
      });
    });
    ZegoExpressEngine.onRemoteCameraStateUpdate = ((streamID, state) {
      print(
          '🚩 🚪 ffflutter onRemoteCameraStateUpdate, streamID: $streamID, state: $state ');
    });
    ZegoExpressEngine.onRemoteMicStateUpdate = ((streamID, state) {
      print(
          '🚩 🚪 ffflutter onRemoteMicStateUpdate, streamID: $streamID, state: $state ');
    });
  }

  void clearZegoEventCallback() {
    ZegoExpressEngine.onRoomStateUpdate = null;
    ZegoExpressEngine.onPublisherStateUpdate = null;
    ZegoExpressEngine.onPlayerStateUpdate = null;
  }

  void clearPreviewView() {
    if (!Platform.isAndroid &&
        !Platform.isIOS &&
        !Platform.isMacOS &&
        !Platform.isWindows &&
        !kIsWeb) {
      return;
    }

    if (_previewViewWidget == null) {
      return;
    }

    // Developers should destroy the [CanvasView] after
    // [stopPublishingStream] or [stopPreview] to release resource and avoid memory leaks
    ZegoExpressEngine.instance.destroyCanvasView(_previewViewID);
    setState(() => _previewViewWidget = null);
  }

  void clearPlayView() {
    if (!Platform.isAndroid &&
        !Platform.isIOS &&
        !Platform.isMacOS &&
        !Platform.isWindows &&
        !kIsWeb) {
      return;
    }

    if (_playViewWidget == null) {
      return;
    }

    // Developers should destroy the [CanvasView]
    // after [stopPlayingStream] to release resource and avoid memory leaks
    ZegoExpressEngine.instance.destroyCanvasView(_playViewID);
    setState(() => _playViewWidget = null);
  }

  // MARK: Widget

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('QuickStart')),
      body: SafeArea(
          child: GestureDetector(
        child: mainContent(),
        behavior: HitTestBehavior.translucent,
        onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),
      )),
    );
  }

  Widget mainContent() {
    return SingleChildScrollView(
        child: Column(children: [
      Divider(),
      Padding(
        padding: const EdgeInsets.symmetric(horizontal: 20.0),
        child: Column(children: [
          viewsWidget(),
          stepOneCreateEngineWidget(),
          stepTwoLoginRoomWidget(),
          stepThreeStartPublishingStreamWidget(),
          stepFourStartPlayingStreamWidget(),
          Padding(padding: const EdgeInsets.only(bottom: 20.0)),
          CupertinoButton.filled(
            child: Text(
              'DestroyEngine',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: destroyEngine,
            padding: EdgeInsets.symmetric(vertical: 10, horizontal: 50),
          ),
          Container(
            color: Colors.grey,
            width: MediaQuery.of(context).size.width,
            height: MediaQuery.of(context).size.height * 0.2,
            child: _mediaplayerWidget,
          ),
        ]),
      ),
    ]));
  }

  Widget viewsWidget() {
    return Container(
      height: MediaQuery.of(context).size.width * 0.65,
      child: GridView(
        padding: const EdgeInsets.all(10.0),
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: 2,
          mainAxisSpacing: 10.0,
          crossAxisSpacing: 10.0,
          childAspectRatio: viewRatio,
        ),
        children: [
          Stack(children: [
            Container(
              color: Colors.grey,
              child: _previewViewWidget,
            ),
            Text('Local Preview View', style: TextStyle(color: Colors.white))
          ], alignment: AlignmentDirectional.topCenter),
          Stack(children: [
            Container(
              color: Colors.grey,
              child: _playViewWidget,
            ),
            Text('Remote Play View', style: TextStyle(color: Colors.white))
          ], alignment: AlignmentDirectional.topCenter),
        ],
      ),
    );
  }

  Widget stepOneCreateEngineWidget() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          'Step1:',
          style: TextStyle(fontWeight: FontWeight.bold),
        ),
        Row(children: [
          Column(
            children: [
              Text('AppID: ${ZegoConfig.instance.appID}',
                  style: TextStyle(fontSize: 10)),
            ],
            crossAxisAlignment: CrossAxisAlignment.start,
          ),
          Spacer(),
          Container(
            width: MediaQuery.of(context).size.width / 2.5,
            child: CupertinoButton.filled(
              child: Text(
                _isEngineActive ? '✅ CreateEngine' : 'CreateEngine',
                style: TextStyle(fontSize: 14.0),
              ),
              onPressed: createEngine,
              padding: EdgeInsets.all(10.0),
            ),
          )
        ]),
        Divider(),
      ],
    );
  }

  Widget stepTwoLoginRoomWidget() {
    return Column(crossAxisAlignment: CrossAxisAlignment.start, children: [
      Text(
        'Step2:',
        style: TextStyle(fontWeight: FontWeight.bold),
      ),
      Row(children: [
        Column(
          children: [
            Text('RoomID: $_roomID', style: TextStyle(fontSize: 10)),
            Text('UserID: ${ZegoConfig.instance.userID}',
                style: TextStyle(fontSize: 10)),
          ],
          crossAxisAlignment: CrossAxisAlignment.start,
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 2.5,
          child: CupertinoButton.filled(
            child: Text(
              _roomState == ZegoRoomState.Connected
                  ? '✅ LoginRoom'
                  : 'LoginRoom',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: _roomState == ZegoRoomState.Disconnected
                ? loginRoom
                : logoutRoom,
            padding: EdgeInsets.all(10.0),
          ),
        )
      ]),
      Divider(),
    ]);
  }

  Widget stepThreeStartPublishingStreamWidget() {
    return Column(crossAxisAlignment: CrossAxisAlignment.start, children: [
      Text(
        'Step3:',
        style: TextStyle(fontWeight: FontWeight.bold),
      ),
      SizedBox(height: 10),
      Row(children: [
        Container(
          width: MediaQuery.of(context).size.width / 2.5,
          child: TextField(
            controller: _publishingStreamIDController,
            decoration: InputDecoration(
                contentPadding: const EdgeInsets.all(10.0),
                isDense: true,
                labelText: 'Publish StreamID:',
                labelStyle: TextStyle(color: Colors.black54, fontSize: 14.0),
                hintText: 'Please enter streamID',
                hintStyle: TextStyle(color: Colors.black26, fontSize: 10.0),
                enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Colors.grey)),
                focusedBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Color(0xff0e88eb)))),
          ),
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 5,
          child: CupertinoButton.filled(
            child: Text(
              _previewViewWidget != null ? '✅ StartPreview' : 'StartPreview',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: _previewViewWidget == null
                ? () {
                    startPreview();
                  }
                : () {
                    stopPreview();
                  },
            padding: EdgeInsets.all(10.0),
          ),
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 5,
          child: CupertinoButton.filled(
            child: Text(
              _publisherState == ZegoPublisherState.Publishing
                  ? '✅ StartPublishing'
                  : 'StartPublishing',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: _publisherState == ZegoPublisherState.NoPublish
                ? () {
                    startPublishingStream(
                        _publishingStreamIDController.text.trim());
                  }
                : () {
                    stopPublishingStream();
                  },
            padding: EdgeInsets.all(10.0),
          ),
        )
      ]),
      Divider(),
    ]);
  }

  Widget stepFourStartPlayingStreamWidget() {
    return Column(crossAxisAlignment: CrossAxisAlignment.start, children: [
      Text(
        'Step4:',
        style: TextStyle(fontWeight: FontWeight.bold),
      ),
      SizedBox(height: 10),
      Row(children: [
        Container(
          width: MediaQuery.of(context).size.width / 2.5,
          child: TextField(
            controller: _playingStreamIDController,
            decoration: InputDecoration(
                contentPadding: const EdgeInsets.all(10.0),
                isDense: true,
                labelText: 'Play StreamID:',
                labelStyle: TextStyle(color: Colors.black54, fontSize: 14.0),
                hintText: 'Please enter streamID',
                hintStyle: TextStyle(color: Colors.black26, fontSize: 10.0),
                enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Colors.grey)),
                focusedBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Color(0xff0e88eb)))),
          ),
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 2.5,
          child: CupertinoButton.filled(
            child: Text(
              _playerState == ZegoPlayerState.Playing
                  ? '✅ StartPlaying'
                  : 'StartPlaying',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: _playerState == ZegoPlayerState.NoPlay
                ? () {
                    startPlayingStream(_playingStreamIDController.text.trim());
                  }
                : () {
                    stopPlayingStream(_playingStreamIDController.text.trim());
                  },
            padding: EdgeInsets.all(10.0),
          ),
        )
      ]),
      Divider(),
    ]);
  }
}
